#include "WorldScene.hpp"
#include "Nature.hpp"

WorldScene::WorldScene() : time(0.0f) {
}

WorldScene::~WorldScene() {
}

bool WorldScene::Initialize() {
    currentStructure = 1;
    isStone = false;
    buildMode = false;
    currentHoveredCellCoords = { -1, -1 };
    lastHoveredStructure = -1;
    spawnCooldown = 2.0f;
    currentSpawnTime = spawnCooldown;

    ////make explosion animated entity
    //SDL_Texture* t = context.txm->LoadTexture(context.renderer, "../../assets/explosion.png");
    //AnimatedSprite* s = new AnimatedSprite();
    //s->SetFrameDuration(explosionFrameTime);
    //s->SetLooping(true);
    //s->Initialize(t, 66, 66, 0, 0, 100, 100, 5, 5);
    //explosion = new Entity();
    //explosion->Initialize(s, Vector2(100, 50), Vector2(0, 0));
    //explosion->GetSprite()->SetColor(color(0, 255, 0));
    //elements.push_back(explosion);

    //explosion2 = new Entity();
    //SDL_Texture* t2 = context.txm->LoadTexture(context.renderer, "../../assets/ball.png");
    //Sprite* s2 = new Sprite();
    //s2->Initialize(t2, 307, 307, 0, 0, 100, 100);
    //explosion2->Initialize(s2, Vector2(200, 150), Vector2(5, 0));
    //elements.push_back(explosion2);

    //make particle emitter
    //SDL_Texture* t2 = context.txm->LoadTexture(context.renderer, "../../assets/sad.png");
    //ParticleEmitter* pe = new ParticleEmitter(t2, color(255, 255, 255), color(255, 255, 255), 278, 278);
    //pe->Initialize(Vector2(-200, -500), Vector2(200, -200), -5, 5, 200, 255, 10, 25, 3, 5);
    //pe->SetPosition(Vector2(300, 400));
    //pe->StartSpawning(.2);
    ////pe->Burst(100);
    //elements.push_back(pe);

    //make text
    //text = new Text();
    //text->Initialize(&renderer, txm, fm, "Fart", "../../fonts/PROXON.ttf", 32);
    //text->SetPosition(500, 300);
    //text->SetRotation(30);
    //text->SetColor(struct color(255, 150, 0)); 
    //elements.push_back(text);

    //make sound
    AudioManager* am = context.am;

    LoadSounds();
    //am->SetGroupPitch("Default", 2);
    //am->LoadSound("../../assets/perfect-fart.ogg", "Fart");
    //am->PlaySound("Fart", "Default", {100, 100, 0}, {0, 0, 0});

    //push grid
    elements.push_back(context.grid);

    SDL_Texture* squareTex = context.txm->LoadTexture(context.renderer, "../../assets/square.jpg");

    //make strucutre
    st = new Structure();
    SDL_Texture* structTex = context.txm->LoadTexture(context.renderer, "../../assets/explosion.png");
    AnimatedSprite* structSpr = new AnimatedSprite();
    structSpr->SetFrameDuration(1);
    structSpr->SetLooping(true);
    structSpr->Initialize(structTex, 66, 66, 0, 0, 100, 100, 5, 5);
    st->Initialize(structSpr, false);

    context.grid->SetAtlas(st);
    st->SetPosition(Vector2(7750, 5300));
    elements.push_back(st);

    //make walls
    wallHwood = new Structure();
    wallVwood = new Structure();
    wallHstone = new Structure();
    wallVstone = new Structure();
    Sprite* woodWallSprite = new Sprite();
    Sprite* stoneWallSprite = new Sprite();
    Sprite* woodWallSpriteV = new Sprite();
    Sprite* stoneWallSpriteV = new Sprite();
    SDL_Texture* woodWallTex = context.txm->LoadTexture(context.renderer, "../../assets/woodWallTexture.PNG");
    SDL_Texture* stoneWallTex = context.txm->LoadTexture(context.renderer, "../../assets/stoneWallTexture.PNG");
    SDL_Texture* woodWallTexV = context.txm->LoadTexture(context.renderer, "../../assets/woodWallTextureV.PNG");
    SDL_Texture* stoneWallTexV = context.txm->LoadTexture(context.renderer, "../../assets/stoneWallTextureV.PNG");
    woodWallSprite->Initialize(woodWallTex, 430, 231, 0, 0, 40, 40);
    stoneWallSprite->Initialize(stoneWallTex, 287, 103, 0, 0, 40, 40);
    woodWallSpriteV->Initialize(woodWallTexV, 231, 430, 0, 0, 40, 40);
    stoneWallSpriteV->Initialize(stoneWallTexV, 103, 287, 0, 0, 40, 40);
    wallHwood->Initialize(woodWallSprite, true);
    wallVwood->Initialize(woodWallSpriteV, true);
    wallHstone->Initialize(stoneWallSprite, true);
    wallVstone->Initialize(stoneWallSpriteV, true);
    wallHwood->SetDurability(40);
    wallVwood->SetDurability(40);
    wallVstone->SetDurability(120);
    wallVstone->SetDurability(120);

    //make wall recipes
    unordered_map<ResourceType, int> recWood;
    unordered_map<ResourceType, int> recStone;
    recWood.insert({ ResourceType::WOOD, 3 });
    recStone.insert({ ResourceType::WOOD, 2 });
    recStone.insert({ ResourceType::STONE, 5 });

    wallHwood->SetRecipe(recWood);
    wallVwood->SetRecipe(recWood);
    wallHstone->SetRecipe(recStone);
    wallVstone->SetRecipe(recStone);

    //make traps
    pushTrap = new PushTrap();
    pushTrap->Initialize(0);
    spikeTrap = new SpikeTrap();
    spikeTrap->Initialize();
    fireTrap = new FireTrap();
    fireTrap->Initialize();
    freezeTrap = new FreezeTrap();
    freezeTrap->Initialize();
    explosionTrap = new ExplosionTrap();
    explosionTrap->Initialize();

    //make player
    player = new Player();
    SDL_Texture* playerBTex = context.txm->LoadTexture(context.renderer, "../../assets/player_back.png");
    AnimatedSprite* playerBSprite = new AnimatedSprite();
    playerBSprite->Initialize(playerBTex, 235, 174, 0, 0, 75, 75, 2, 2);
    playerBSprite->SetDrawLayer(RenderLayer::PLAYER);
    playerBSprite->SetFrameDuration(0.25);
    playerBSprite->SetLooping(true);
    playerBSprite->SetLeaveOnLastFrame(true);
    player->Initialize(Vector2(7500, 5000), 100, Vector2(0, 0), playerBSprite);
    player->SetMovementSpeed(300);
    context.grid->UpdateOccupancy((Entity*)player, &GridCell::AddOther, &GridCell::RemoveOther);
    elements.push_back(player);

    //make attackCone
    AttackCone* attackCone = new AttackCone(15, 200, PI / 5, 0.75);
    SDL_Texture* swooshTex = context.txm->LoadTexture(context.renderer, "../../assets/swoosh.png");
    AnimatedSprite* swooshSpr = new AnimatedSprite();
    swooshSpr->SetFrameDuration(0.02);
    swooshSpr->SetLooping(false);
    swooshSpr->Initialize(swooshTex, 307, 259, 0, 0, context.grid->GetCellSize(), context.grid->GetCellSize(), 5, 10);
    attackCone->Initialize(player->GetPosition(), swooshSpr);
    context.grid->UpdateOccupancy((Entity*)player->GetAttackCone(), &GridCell::AddOther, &GridCell::RemoveOther);

    player->SetAttackCone(attackCone);

    //make enemy spawner
    spawner = new EnemySpawner();
    spawner->Initialize("../../data/enemyStats.txt", "../../data/spawnPools.json");
    elements.push_back(spawner);

    //UI
    //wood
    SDL_Texture* woodTex = context.txm->LoadTexture(context.renderer, "../../assets/log.png");
    Sprite* woodIcon = new Sprite();
    woodIcon->Initialize(woodTex, 410, 261, 0, 0, 35, 35);
    woodIcon->SetDrawLayer(RenderLayer::UI);
    woodIcon->SetPosition(Vector2(10, 10));
    UI.push_back(woodIcon);

    woodCount = new Text();
    woodCount->Initialize(context, "0", "../../fonts/PROXON.ttf", 35);
    woodCount->SetPosition(50, 10);
    UI.push_back(woodCount);

    //stone
    SDL_Texture* stoneTex = context.txm->LoadTexture(context.renderer, "../../assets/stone.png");
    Sprite* stoneIcon = new Sprite();
    stoneIcon->Initialize(stoneTex, 404, 334, 0, 0, 35, 35);
    stoneIcon->SetDrawLayer(RenderLayer::UI);
    stoneIcon->SetPosition(Vector2(10, 50));
    UI.push_back(stoneIcon);

    stoneCount = new Text();
    stoneCount->Initialize(context, "0", "../../fonts/PROXON.ttf", 35);
    stoneCount->SetPosition(50, 50);
    UI.push_back(stoneCount);

    //coin
    SDL_Texture* coinTex = context.txm->LoadTexture(context.renderer, "../../assets/coin.png");
    Sprite* coinIcon = new Sprite();
    coinIcon->Initialize(coinTex, 2195, 2195, 0, 0, 35, 35);
    coinIcon->SetDrawLayer(RenderLayer::UI);
    coinIcon->SetPosition(Vector2(10, 90));
    UI.push_back(coinIcon);

    coinCount = new Text();
    coinCount->Initialize(context, "0", "../../fonts/PROXON.ttf", 35);
    coinCount->SetPosition(50, 90);
    UI.push_back(coinCount);


    return true;
}

void WorldScene::Process(float deltaTime) {

    ReadInputs(deltaTime);

    currentSpawnTime -= deltaTime;
    time += deltaTime;
    context.gameProgress = time / 500;
    for (Element* e : elements) {
        e->Process(deltaTime);
    }

    //spawn enemies
    if (currentSpawnTime < 0) {
        spawner->SpawnEnemies();
        currentSpawnTime = spawnCooldown;
    }

    //update ui
    woodCount->SetText(to_string(player->GetWood()));
    stoneCount->SetText(to_string(player->GetStone()));
    coinCount->SetText(to_string(player->GetCoins()));

    //updateHover
    UpdateCurrentHoveredCell(player->CanMakeRecipe(GetCurrentStructure()->GetRecipe()));

    //collision updates
    context.grid->UpdateOccupancy((Entity*)player, &GridCell::AddOther, &GridCell::RemoveOther);
    context.grid->UpdateOccupancy((Entity*)player->GetAttackCone(), &GridCell::AddOther, &GridCell::RemoveOther);
    context.grid->ResolveCollisions(player); //collison updates

    context.renderer->cam->Follow(player->GetPosition());//follow player
}

void WorldScene::Draw(Renderer* renderer) {
    for (Element* e : elements) {
        e->Draw(renderer);
    }
    for (Sprite* s : UI) {
        s->Draw(renderer);
    }
}

Entity* WorldScene::GetPlayer() {
    return player;
}

void WorldScene::Attack() {
    AttackCone* cone = player->GetAttackCone();
    if (cone->CanAttack()) {
        cone->PlayAttack();
        context.grid->ResolveCollisions(cone);
        cone->ClearAttack();
    }
}


//STRUCTURE PLACEMENT
void WorldScene::PlaceStructure(bool isHologram) {
    if (!buildMode) return;
    Vector2 vec2 = context.im->GetMouseWorldPosition(context.renderer->cam);
    GridCoord coord = context.grid->WorldToGrid(vec2);
    Structure* s = GetCurrentStructure();
    bool occupied = false;

    if (!isHologram) RemoveHoverEffect(coord);
    if (currentStructure == 1) {
        occupied = context.grid->GetCell(coord)->HasWall(WallDirection::NORTH);
        context.grid->PlaceWall(coord, WallDirection::NORTH, s);
    }
    else if (currentStructure == 2) {
        occupied = context.grid->GetCell(coord)->HasWall(WallDirection::WEST);
        context.grid->PlaceWall(coord, WallDirection::WEST, s);
    }
    else if (currentStructure >= 3) {
        occupied = context.grid->GetCell(coord)->HasStructure();
        context.grid->PlaceStructure(s, coord);
    }

    if(occupied) context.am->PlaySound("CantPlace", "Default", { player->GetPosition().x, 100, player->GetPosition().y }, { 0, 0, 0 }, Vector2(1, 1));
    if (isHologram || occupied) { //only take payment if real and can place
        return;
    }

    if (!s) return;//something fucked up
    auto recipe = s->GetRecipe();
    if (!player->CanMakeRecipe(recipe)) {//cant make recipe
        RemoveStructure(true);
        context.am->PlaySound("CantPlace", "Default", { player->GetPosition().x, 100, player->GetPosition().y }, { 0, 0, 0 }, Vector2(1, 1));
        return;
    }
    context.am->PlaySound("BuildPlace", "Default", { player->GetPosition().x, 100, player->GetPosition().y }, { 0, 0, 0 }, Vector2(0.85, 1.15));
    player->RemoveRecipeCost(recipe);

    //play sound

    context.grid->InvalidateFlowFieldsNear(coord, 10);
}

//only called by player, never hologram
void WorldScene::RemoveStructure(bool couldntAfford) {
    if (!buildMode) return;
    Vector2 vec2 = context.im->GetMouseWorldPosition(context.renderer->cam);
    GridCoord coord = context.grid->WorldToGrid(vec2);
    bool structRemoved = false;

    if (currentStructure == 1) {
        structRemoved = context.grid->RemoveWall(coord, WallDirection::NORTH);
    }
    else if (currentStructure == 2) {
        structRemoved = context.grid->RemoveWall(coord, WallDirection::WEST);
    }
    else {
        structRemoved = context.grid->RemoveStructure(coord);
    }

    if(structRemoved && !couldntAfford)
        context.am->PlaySound("BreakStructure", "Default", { player->GetPosition().x, 100, player->GetPosition().y}, {0, 0, 0}, Vector2(0.85, 1.15));
    context.grid->InvalidateFlowFieldsNear(coord, 10);
}

Structure* WorldScene::GetCurrentStructure() {
    Trap* s = nullptr;
    switch (currentStructure) {
        case(1)://horizontal wall
             return isStone ? wallHstone : wallHwood;
        case(2):
            return isStone ? wallVstone : wallVwood;
        case(3):
            return nullptr;
        case(4):
            pushTrap->SetDirection(pushTrapDirection);
            return pushTrap;
        case(5):
            return spikeTrap;
        case(6):
            return fireTrap;
        case(7):
            return freezeTrap;
        case(8):
            return explosionTrap;
    }
    return nullptr;
}


