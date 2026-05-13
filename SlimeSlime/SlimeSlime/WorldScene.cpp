#include "WorldScene.hpp"
#include "Nature.hpp"

WorldScene::WorldScene() : time(0.0f) {
}

WorldScene::~WorldScene() {
}

bool WorldScene::Initialize() {
    elements.clear();
    UI.clear();
    gameRunning = true;
    currentStructure = 1;
    isStone = false;
    buildMode = false;
    currentHoveredCellCoords = { -1, -1 };
    lastHoveredStructure = -1;
    spawnCooldown = 5.0f;
    currentSpawnTime = context.gameDifficulty / 4;
    gamePaused = false;

    context.grid = new Grid(GRID_WIDTH, GRID_HEIGHT, CELL_SIZE);
    SDL_Texture* grassTex = context.txm->LoadTexture(context.renderer, "../../assets/grass.png");
    context.grid->Initialize(grassTex);

    uniform_int_distribution<int> spawnXGen(GRID_WIDTH * 0.25, GRID_WIDTH * 0.75);
    uniform_int_distribution<int> spawnYGen(GRID_HEIGHT * 0.25, GRID_HEIGHT * 0.75);
    Vector2 spawnPos = Vector2(spawnXGen(gen), spawnYGen(gen));

    context.am->StopMusic();
    context.am = new AudioManager();
    context.am->Initialize({ 0, 0, 0 }, 7);


    //make sound
    AudioManager* am = context.am;
    LoadSounds();

    //push grid
    elements.push_back(context.grid);

    SDL_Texture* squareTex = context.txm->LoadTexture(context.renderer, "../../assets/square.jpg");

    //make atlas
    atlas = new Structure();
    SDL_Texture* atlasTex = context.txm->LoadTexture(context.renderer, "../../assets/traps/atlas.png");
    AnimatedSprite* atlasSpr = new AnimatedSprite();
    atlasSpr->Animate();
    atlasSpr->SetFrameDuration(0.35);
    atlasSpr->SetLooping(true);
    atlasSpr->Initialize(atlasTex, 125, 125, 0, 0, 100, 100, 4, 16);
    atlas->Initialize(atlasSpr, false);
    atlas->SetHealth(500);
    atlas->SetCanCollide(true);
    Structure* temp;
    while (!(temp = context.grid->PlaceStructure(atlas, context.grid->WorldToGrid(Vector2(spawnPos.x, spawnPos.y - CELL_SIZE)), false))) {
        spawnPos = Vector2(spawnXGen(gen), spawnYGen(gen));
    }
    atlas = temp;
    context.grid->SetAtlas(atlas);
    int size = atlas->GetSprite()->GetWidth();
    atlas->SetCollisionBound(CollisionShape::MakeAABB(size * 0.9, size * 0.9, Vector2(size * 0.05, size * 0.05)));

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
    recWood.insert({ ResourceType::STONE, 0 });
    recWood.insert({ ResourceType::COIN, 0 });
    recStone.insert({ ResourceType::WOOD, 2 });
    recStone.insert({ ResourceType::STONE, 5 });
    recStone.insert({ ResourceType::COIN, 0 });

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
    player->Initialize(Vector2(spawnPos.x, spawnPos.y + CELL_SIZE), 100, Vector2(0, 0), playerBSprite);
    player->SetMovementSpeed(300);
    context.grid->UpdateOccupancy((Entity*)player, &GridCell::AddOther, &GridCell::RemoveOther);
    elements.push_back(player);

    //make attackCone
    AttackCone* attackCone = new AttackCone(17, 200, PI / 5, 0.75);
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

    InitializeUI();

    return true;
}


void WorldScene::Process(float deltaTime) {

    if (gamePaused) {
        ReadInputs(deltaTime);//only reads for unpause
        pauseScreen->Process(deltaTime);
        return;//dont process when paused
    }

    //is game over
    if ((!atlas || atlas->IsBroken()) && gameRunning) {
        player->SetDead();
        gameOverScreen->Toggle();
        if (playerDeathScreen->IsVisible()) playerDeathScreen->Toggle();
        if (structureHUD->IsVisible()) structureHUD->Toggle();
        atlas = nullptr;
        gameRunning = false;
        timerText->SetText("You lasted " + to_string((int)time / 60) + " minutes and " + to_string((int) time % 60) + " seconds");
        gameOverScreen->AddText(timerText, (gameOverScreen->GetWidth() - timerText->GetWidth()) / 2, gameOverScreen->GetHeight() * 0.35);
 }
    else if(gameRunning){
        atlas->Process(deltaTime);
    }

    //is player dead
    if (!player->IsAlive() && gameRunning && respawnTimer <= 0) {
        playerDeathScreen->Toggle();
        respawnTimer = 20;
        player->RemoveCoins(player->GetCoins() / 2);//remove half of coins
        player->ClearTargetedBy();
    }
    if (respawnTimer > 0) {
        respawnTimer -= deltaTime;
        respawnTimerText->SetText(to_string((int)respawnTimer) + " seconds");
    }
    if (respawnTimer <= 0 && !player->IsAlive() && gameRunning) {
        playerDeathScreen->Toggle();
        player->Heal(player->GetMaxHealth());
        player->SetAlive();
    }

    currentSpawnTime -= deltaTime;
    time += deltaTime;
    context.gameProgress = time / context.gameDifficulty;

    //update ui
    woodCount->SetText(to_string(player->GetWood()));
    stoneCount->SetText(to_string(player->GetStone()));
    coinCount->SetText(to_string(player->GetCoins()));
    for (Sprite* s : UI) {
        s->Process(deltaTime);
    }
    UpdateUpgradeLabelColor();

    if(player->IsAlive()) ReadInputs(deltaTime);
    context.im->SetIsMouseOverUI(false);

    for (Element* e : elements) {
        e->Process(deltaTime);
    }

    //spawn enemies
    if (currentSpawnTime < 0 && gameRunning) {
        spawner->SpawnEnemies();
        currentSpawnTime = spawnCooldown;
    }


    //updateHover
    UpdateCurrentHoveredCell(player->CanMakeRecipe(GetCurrentStructure(currentStructure)->GetRecipe()));

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
    if (atlas && !atlas->IsBroken())
        atlas->Draw(renderer);
    else
        atlas = nullptr;
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
    Structure* s = GetCurrentStructure(currentStructure);
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
        occupied = !context.grid->CanPlaceStructure(coord);
        Structure* placed = context.grid->PlaceStructure(s, coord, isHologram);
        if (!placed) occupied = true;
    }

    if(!isHologram && occupied) context.am->PlaySound("CantPlace", "Default", { player->GetPosition().x, 100, player->GetPosition().y }, { 0, 0, 0 }, Vector2(1, 1));
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

Structure* WorldScene::GetCurrentStructure(int currentStruct) {
    Trap* s = nullptr;
    switch (currentStruct) {
        case(1)://horizontal wall
             return isStone ? wallHstone : wallHwood;
        case(2):
            return isStone ? wallVstone : wallVwood;
        case(3):
            pushTrap->SetDirection(pushTrapDirection);
            return pushTrap;
        case(4):
            return spikeTrap;
        case(5):
            return fireTrap;
        case(6):
            return freezeTrap;
        case(7):
            return explosionTrap;
    }
    return nullptr;
}


