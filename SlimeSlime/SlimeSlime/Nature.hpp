#ifndef NATURE_HPP
#define NATURE_HPP

#include "GameContext.hpp"
#include "Entity.hpp"
#include "ResourceType.hpp"
#include "Sprite.hpp"
#include "Grid.hpp"
#include "NatureType.hpp"

class Nature : public Entity {
public:
		void Initialize(Sprite* spr, NatureType t, bool canCollide);
		NatureType GetNatureType() const { return type; }
		void SetPosition(Vector2 pos);
		Vector2 GetPosition() const override;
		void Draw(Renderer* renderer) override;
		void Process(float deltaTime) override;

		void Damage(float amount) override;
		void Break();
		ResourceType GetDropType() const override;
		int GetDropAmount() const override;
		void SpawnDrops();

		void HandleCollision(Collidable* other, Vector2 penetration) override;
	protected:
		NatureType type;
};

static uniform_int_distribution<int> treeSpriteGen(1, 4);
static uniform_int_distribution<int> rockSpriteGen(1, 6);
static uniform_int_distribution<int> stumpSpriteGen(1, 2);
static uniform_int_distribution<int> bushSpriteGen(1, 4);
static uniform_int_distribution<int> foliageSpriteGen(1, 7);
static uniform_real_distribution<double> widthGen(0.9, 1.1);
static uniform_real_distribution<double> heightGen(.85, 1.15);
static uniform_real_distribution<double> treeScaler(2, 4);
static uniform_real_distribution<double> rockScaler(.5, 2.5);
static uniform_real_distribution<double> stumpScaler(.5, 1);
static uniform_real_distribution<double> bushScaler(.5, 1);

//calculates nature durability based on generated size and type
static int CalculateDurability(int w, int h, int cellSize, NatureType type) {
	double width = (double)w / cellSize; //adjusted based on cellSize
	double height = (double)h / cellSize;
	double area = width * height;

	//NOTE: These numbers are made up. Math could be done, but
	//I failed the first time, so chat stepped in.
	switch (type) {
		case NatureType::TREE:
			return 75 + (area * 4.5);//95 - 165
		case NatureType::ROCK:
			return 130 + (area * 13.5);//132 - 236
		case NatureType::STUMP:
			return 65 + (area * 50);//77 - 129
		case NatureType::BUSH:
			return 48 + (area * 28);//51 - 83
		case NatureType::FOLIAGE:
			return 1;
	}
}

static Nature* GetRandomTree() {
	int i = treeSpriteGen(gen);
	int cellSize = context.grid->GetCellSize();

	int scaler = treeScaler(gen);
	int width = cellSize * widthGen(gen) * scaler;
	int height = cellSize * heightGen(gen) * scaler * 1.25;

	SDL_Texture* tex;
	Sprite* sprite = new Sprite();
	Nature* n = new Nature();
	CollisionShape cs;
	switch (i) {
		case(1):
			width *= 1.25;
			tex = context.txm->LoadTexture(context.renderer, "../../assets/nature/tree/tree1.png");
			sprite->Initialize(tex, 150, 191, 0, 0, width, height, RenderLayer::NATURE);
			cs = CollisionShape::MakeAABB(width * 0.2, height * 0.1, Vector2(width * 0.4, height * 0.9));
			break;
		case(2):
			tex = context.txm->LoadTexture(context.renderer, "../../assets/nature/tree/tree2.png");
			sprite->Initialize(tex, 147, 168, 0, 0, width, height, RenderLayer::NATURE);
			cs = CollisionShape::MakeAABB(width * 0.2, height * 0.1, Vector2(width * 0.5, height * 0.9));
			break;
		case(3):
			tex = context.txm->LoadTexture(context.renderer, "../../assets/nature/tree/tree3.png");
			sprite->Initialize(tex, 111, 188, 0, 0, width, height, RenderLayer::NATURE);
			cs = CollisionShape::MakeAABB(width * 0.2, height * 0.1, Vector2(width * 0.4, height * 0.9));
			break;
		case(4):
			height *= 1.25;
			tex = context.txm->LoadTexture(context.renderer, "../../assets/nature/tree/tree4.png");
			sprite->Initialize(tex, 61, 183, 0, 0, width, height, RenderLayer::NATURE);
			cs = CollisionShape::MakeAABB(width * 0.2, height * 0.1, Vector2(width * 0.4, height * 0.9));
			break;
	}
	n->Initialize(sprite, NatureType::TREE, true);
	n->Collidable::SetCollisionBound(cs);
	n->SetHealth(CalculateDurability(width, height, cellSize, NatureType::TREE));

	//tree health bar apepears differently
	PercentageBar* healthBar = new PercentageBar(n->GetHealth(), n->GetMaxHealth(), cs.box.width * 1.2, cs.box.height * 0.25, { 255, 50, 50 }, { 150, 50, 50 });
	healthBar->SetPosition(n->GetPosition().x, n->GetPosition().y);
	healthBar->SetOffset(cs.offset.x - (cs.box.width * 0.1), (sprite->GetHeight() * 0.9));
	n->SetHealthBar(healthBar);
	
	return n;
}

static Nature* GetRandomRock() {
	int i = rockSpriteGen(gen);
	int cellSize = context.grid->GetCellSize();

	double scaler = rockScaler(gen);
	int width = cellSize * widthGen(gen) * scaler;
	int height = cellSize * heightGen(gen) * scaler;

	SDL_Texture* tex;
	Sprite* sprite = new Sprite();
	Nature* n = new Nature();
	switch (i) {
	case(1):
		tex = context.txm->LoadTexture(context.renderer, "../../assets/nature/rock/rock1.png");
		sprite->Initialize(tex, 118, 74, 0, 0, width, height * 0.9, RenderLayer::NATURE);
		break;
	case(2):
		tex = context.txm->LoadTexture(context.renderer, "../../assets/nature/rock/rock2.png");
		sprite->Initialize(tex, 98, 82, 0, 0, width, height * 0.9, RenderLayer::NATURE);
		break;
	case(3):
		tex = context.txm->LoadTexture(context.renderer, "../../assets/nature/rock/rock3.png");
		sprite->Initialize(tex, 106, 58, 0, 0, width, height * 0.9, RenderLayer::NATURE);
		break;
	case(4):
		tex = context.txm->LoadTexture(context.renderer, "../../assets/nature/rock/rock4.png");
		sprite->Initialize(tex, 100, 62, 0, 0, width, height * 0.9, RenderLayer::NATURE);
		break;
	case(5):
		tex = context.txm->LoadTexture(context.renderer, "../../assets/nature/rock/rock5.png");
		sprite->Initialize(tex, 65, 43, 0, 0, width, height * 0.9, RenderLayer::NATURE);
		break;
	case(6):
		tex = context.txm->LoadTexture(context.renderer, "../../assets/nature/rock/rock6.png");
		sprite->Initialize(tex, 69, 43, 0, 0, width, height * 0.9, RenderLayer::NATURE);
		break;
	}
	n->Initialize(sprite, NatureType::ROCK, true);
	CollisionShape cs = CollisionShape::MakeAABB(width, height * 0.5, Vector2(0, height * 0.4));
	n->SetCollisionBound(cs);
	n->SetHealth(CalculateDurability(width, height, cellSize, NatureType::ROCK));

	return n;
}

static Nature* GetRandomStump() {
	int i = stumpSpriteGen(gen);
	int cellSize = context.grid->GetCellSize();

	double scaler = stumpScaler(gen);
	int width = cellSize * widthGen(gen) * scaler;
	int height = cellSize * heightGen(gen) * scaler;

	SDL_Texture* tex;
	Sprite* sprite = new Sprite();
	Nature* n = new Nature();
	CollisionShape cs;
	switch (i) {
		case(1):
			tex = context.txm->LoadTexture(context.renderer, "../../assets/nature/stump/stump1.png");
			sprite->Initialize(tex, 66, 84, 0, 0, width, height, RenderLayer::NATURE);
			cs = CollisionShape::MakeAABB(width, height * 0.3, Vector2(0, height * 0.7));
			break;
		case(2):
			tex = context.txm->LoadTexture(context.renderer, "../../assets/nature/stump/stump2.png");
			sprite->Initialize(tex, 121, 51, 0, 0, width * 1.5, height, RenderLayer::NATURE);
			cs = CollisionShape::MakeAABB(width * 1.5, height * 0.7, Vector2(0, height * 0.3));
			break;
	}
	n->Initialize(sprite, NatureType::STUMP, true);

	n->SetCollisionBound(cs);
	n->SetHealth(CalculateDurability(width, height, cellSize, NatureType::STUMP));

	return n;
}

static Nature* GetRandomBush() {
	int i = bushSpriteGen(gen);
	int cellSize = context.grid->GetCellSize();

	double scaler = bushScaler(gen);
	int width = cellSize * widthGen(gen) * scaler;
	int height = cellSize * heightGen(gen) * scaler;

	SDL_Texture* tex;
	Sprite* sprite = new Sprite();
	Nature* n = new Nature();
	CollisionShape cs;
	switch (i) {
	case(1):
		tex = context.txm->LoadTexture(context.renderer, "../../assets/nature/bush/bush1.png");
		sprite->Initialize(tex, 104, 72, 0, 0, width, height, RenderLayer::NATURE);
		cs = CollisionShape::MakeAABB(width, height * 0.5, Vector2(0, height * 0.5));
		break;
	case(2):
		tex = context.txm->LoadTexture(context.renderer, "../../assets/nature/bush/bush2.png");
		sprite->Initialize(tex, 112, 73, 0, 0, width, height, RenderLayer::NATURE);
		cs = CollisionShape::MakeAABB(width, height * 0.5, Vector2(0, height * 0.5));
		break;
	case(3):
		tex = context.txm->LoadTexture(context.renderer, "../../assets/nature/bush/bush3.png");
		sprite->Initialize(tex, 135, 49, 0, 0, width, height, RenderLayer::NATURE);
		cs = CollisionShape::MakeAABB(width, height * 0.8, Vector2(0, height * 0.2));
		break;
	case(4):
		tex = context.txm->LoadTexture(context.renderer, "../../assets/nature/bush/bush4.png");
		sprite->Initialize(tex, 88, 77, 0, 0, width, height, RenderLayer::NATURE);
		cs = CollisionShape::MakeAABB(width, height * 0.5, Vector2(0, height * 0.5));
		break;
	}
	n->Initialize(sprite, NatureType::BUSH, true);
	
	n->SetCollisionBound(cs);
	n->SetHealth(CalculateDurability(width, height, cellSize, NatureType::BUSH));

	return n;
}


#endif