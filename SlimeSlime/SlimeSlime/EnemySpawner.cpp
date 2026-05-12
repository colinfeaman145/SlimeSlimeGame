#include "EnemySpawner.hpp"
#include "Grid.hpp"

void EnemySpawner::Initialize(const string& enemyStats, const string& spawnPools) {
	InitEnemyStats(enemyStats);
	InitSpawnPools(spawnPools);
	InitSprites();
}

void EnemySpawner::InitEnemyStats(const string& enemyStatsFile) {
	ifstream file(enemyStatsFile);

	if (!file.is_open()) {
		throw runtime_error("Failed to open file: " + enemyStatsFile);
	}

	string colorName;

	while (file >> colorName) {

		int damageMin, damageMax;
		int healthMin, healthMax;
		float sizeMin, sizeMax;
		float speedMin, speedMax;
		float attackSpeedMin, attackSpeedMax;

		file >> damageMin >> damageMax;
		file >> healthMin >> healthMax;
		file >> sizeMin >> sizeMax;
		file >> speedMin >> speedMax;
		file >> attackSpeedMin >> attackSpeedMax;

		EnemyType type = StringToEnemyType(colorName);

		damage[type] = uniform_int_distribution<int>(damageMin, damageMax);
		health[type] = uniform_int_distribution<int>(healthMin, healthMax);
		size[type] = uniform_real_distribution<float>(sizeMin, sizeMax);
		moveSpeed[type] = uniform_real_distribution<float>(speedMin, speedMax);
		attackSpeed[type] = uniform_real_distribution<float>(attackSpeedMin, attackSpeedMax);
	}

	file.close();
}

void EnemySpawner::InitSpawnPools(const string& spawnPoolsFile) {
	ifstream file(spawnPoolsFile);

	if (!file.is_open()) {
		throw runtime_error("Failed to open file: " + spawnPoolsFile);
	}

	json j;
	file >> j;

	if (!j.contains("ranges") || !j["ranges"].is_array()) {
		throw runtime_error("invalid json: missing ranges array");
	}

	for (const auto& r : j["ranges"]) {

		SpawnPool pool;

		pool.min = r.at("min").get<float>();
		pool.max = r.at("max").get<float>();

		const auto& weightsJson = r.at("weights");

		for (auto it = weightsJson.begin(); it != weightsJson.end(); ++it) {
			string color = it.key();
			float weight = it.value().get<float>();

			EnemyType type = StringToEnemyType(color);
			pool.weights[type] = weight;
		}

		spawnPools.push_back(pool);
	}

	file.close();
}

void EnemySpawner::InitSprites() {

	int totalRows = 7;

	vector<EnemyType> rowToType = {
		EnemyType::RANDOM,
		EnemyType::EXPLOSIVE,
		EnemyType::ATLAS_FOCUS,
		EnemyType::PLAYER_FOCUS,
		EnemyType::NORMAL,
		EnemyType::FAST,
		EnemyType::WALL_FOCUS
	};

	//moving sprites
	for (int row = 0; row < totalRows; ++row) {

		SDL_Texture* tex = context.txm->LoadTexture(context.renderer, "../../assets/Slimes/slime_move.png");
		int frameWidth = 560 / 7; //7 frames
		int frameHeight = 505 / totalRows;

		EnemyType type = rowToType[row];
		AnimatedSprite* spr = new AnimatedSprite();

		int srcX = 0;
		int srcY = row * frameHeight;

		spr->Initialize(tex, frameWidth, frameHeight, srcX, srcY, 100, 100, 7, 7);
		spr->SetDrawLayer(RenderLayer::ENEMIES);
		spr->SetFrameDuration(0.15);
		spr->SetLooping(true);
		spr->SetLeaveOnLastFrame(true);

		moving[type] = spr;
	}

	//attacking sprites
	for (int row = 0; row < totalRows; ++row) {

		SDL_Texture* tex = context.txm->LoadTexture(context.renderer, "../../assets/Slimes/slime_attack.png");
		int frameWidth = 1120 / 14; //14 frames
		int frameHeight = 504 / totalRows;

		EnemyType type = rowToType[row];
		AnimatedSprite* spr = new AnimatedSprite();

		int srcX = 0;
		int srcY = row * frameHeight;

		spr->Initialize(tex, frameWidth, frameHeight, srcX, srcY, 100, 100, 14, 14);
		spr->SetDrawLayer(RenderLayer::ENEMIES);
		spr->SetFrameDuration(0.075);
		spr->SetLooping(false);
		spr->SetLeaveOnLastFrame(true);

		attacking[type] = spr;
	}

	//death sprite
	for (int row = 0; row < totalRows; ++row) {

		SDL_Texture* tex = context.txm->LoadTexture(context.renderer, "../../assets/Slimes/slime_die.png");
		int frameWidth = 1040 / 13; //13 frames
		int frameHeight = 504 / totalRows;

		EnemyType type = rowToType[row];
		AnimatedSprite* spr = new AnimatedSprite();

		int srcX = 0;
		int srcY = row * frameHeight;

		spr->Initialize(tex, frameWidth, frameHeight, srcX, srcY, 100, 100, 13, 13);
		spr->SetDrawLayer(RenderLayer::ENEMIES);
		spr->SetFrameDuration(0.05);
		spr->SetLooping(false);
		spr->SetLeaveOnLastFrame(true);

		death[type] = spr;
	}
}

void EnemySpawner::Draw(Renderer* renderer) {
	for (Enemy* e : enemies)
		e->Draw(renderer);
}

void EnemySpawner::Process(float deltaTime) {
	for (auto it = enemies.begin(); it != enemies.end(); ) {//iterate through enemies
		Enemy* e = *it;
		if (e->IsAlive()) {
			e->Process(deltaTime);
			context.grid->ResolveCollisions(e);
			++it;
		}
		else {
			if (e->IsDying()) {//if still animating death, skip for now
				e->GetSprite()->Process(deltaTime);//process after death so animation plays
				if (e->GetExplosion()) e->GetExplosion()->Process(deltaTime);
				++it;
				continue;
			}
			it = enemies.erase(it);
			context.grid->RemoveEnemy(e);
		}
	}
}

void EnemySpawner::SpawnEnemies() {

	//limits number of enemy spawns
	if (enemies.size() > min((context.gameProgress + 1) * 50, 300.0f)) return;

	//get spawn pool
	float progress = context.gameProgress;
	SpawnPool pool;
	for (const SpawnPool& s : spawnPools) {
		if (s.min <= progress && progress < s.max) {
			pool = s;
			break;
		}
	}

	//give padding to spawn bounds so enemy doesnt get lost outside grid
	int cellSize = context.grid->GetCellSize();
	uniform_int_distribution<int> xGen(cellSize, (context.grid->GetGridWidth() * cellSize) - cellSize);
	uniform_int_distribution<int> yGen(cellSize, (context.grid->GetGridHeight() * cellSize) - cellSize);

	uniform_int_distribution<int> numSpawnsGen(0, roundf(progress) + 3);
	for (int i = 0; i < numSpawnsGen(gen); i++) {
		Enemy* e = GetRandomEnemy(pool, progress, cellSize);
		e->SetPosition(Vector2(xGen(gen), yGen(gen)));
		e->SetTarget(e->FindNewTarget());
		enemies.push_back(e);
	}
}

Enemy* EnemySpawner::GetRandomEnemy(SpawnPool pool, float gameProg, int cellSize) {

	//determine type
	EnemyType type = GetRandomEnemyType(pool);

	int scale = (gameProg / 2) + 1;

	Enemy* e = new Enemy();
	e->SetSprites(moving.at(type), attacking.at(type), death.at(type));//Random enemy only looks purple, acts like other
	while(type == EnemyType::RANDOM)
		type = GetRandomEnemyType(pool);

	uniform_real_distribution<float> targetGen(2, 4);
	int targetRadius = type == EnemyType::WALL_FOCUS ? 10 : 5;
	float atlasTarget = type == EnemyType::ATLAS_FOCUS ? 1.25 : targetGen(gen);
	float playerTarget = type == EnemyType::PLAYER_FOCUS ? 1.25 : targetGen(gen);

	e->Initialize(Vector2(), moving.at(type), 15 ,targetRadius, atlasTarget, playerTarget);
	e->SetType(type);
	e->SetCollisionBound(CollisionShape::MakeCircle(e->GetRadius() * 0.75));//slime sprite has a bit of white space
	e->SetExplosion(); //returns if not needed
	e->SetMovementSpeed(moveSpeed.at(type)(gen) * ((scale * 0.5) + 1));
	//e->SetMovementSpeed(200);
	e->SetHealth(health.at(type)(gen) * scale);
	e->SetDamage(damage.at(type)(gen) * scale);
	e->SetAttackCooldown(attackSpeed.at(type)(gen) * ((scale * 0.5) + 1));
	e->SetSpritesDrawSize(size.at(type)(gen) * cellSize * 2);

	return e;
}

EnemyType EnemySpawner::GetRandomEnemyType(SpawnPool pool) {
	float sum = 0;
	for (auto m : pool.weights) {
		sum += m.second;
	}
	uniform_real_distribution<float> typeGen(0, sum);
	float target = typeGen(gen);
	EnemyType type = EnemyType::NORMAL;
	for (auto m : pool.weights) {
		if (target < m.second) {
			type = m.first;
			break;
		}
		else
			target -= m.second;
	}
	return type;
}

EnemyType EnemySpawner::StringToEnemyType(const string& str) {
	if (str == "Grey") return EnemyType::WALL_FOCUS;
	if (str == "Green") return EnemyType::NORMAL;
	if (str == "Blue") return EnemyType::PLAYER_FOCUS;
	if (str == "Peach") return EnemyType::FAST;
	if (str == "Orange") return EnemyType::ATLAS_FOCUS;
	if (str == "Red") return EnemyType::EXPLOSIVE;
	if (str == "Pink") return EnemyType::RANDOM;

	throw runtime_error("unknown enemy type: " + str);
}

AnimatedSprite* EnemySpawner::GetSprite(EnemyType type) {
	return moving.at(type);
}