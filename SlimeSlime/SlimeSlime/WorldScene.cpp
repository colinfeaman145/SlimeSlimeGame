#include "WorldScene.hpp"
#include "Nature.hpp"

WorldScene::WorldScene() : time(0.0f) {
}

WorldScene::~WorldScene() {
}

bool WorldScene::Initialize(GameContext& context) {
    currentStructure = 1;
    buildMode = false;
    currentHoveredCellCoords = { -1, -1 };

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
    //AudioManager* am = context.am;
    //am->AddGroup("Default");
    //am->SetGroupPitch("Default", 2);
    //am->LoadSound("../../assets/perfect-fart.ogg", "Fart");
    //am->PlaySound("Fart", "Default", {100, 100, 0}, {0, 0, 0});

    //make grid
    Grid* grid = new Grid(30000, 20000, 150);
    SDL_Texture* squareTex = context.txm->LoadTexture(context.renderer, "../../assets/square.jpg");
    SDL_Texture* grassTex = context.txm->LoadTexture(context.renderer, "../../assets/grass.png");
    grid->Initialize(grassTex, context);
    elements.push_back(grid);

    //make strucutre
    st = new Structure();
    SDL_Texture* structTex = context.txm->LoadTexture(context.renderer, "../../assets/explosion.png");
    AnimatedSprite* structSpr = new AnimatedSprite();
    structSpr->SetFrameDuration(1);
    structSpr->SetLooping(true);
    structSpr->Initialize(structTex, 66, 66, 0, 0, 100, 100, 5, 5);
    st->Initialize(structSpr, false);

    //make walls
    wallH = new Structure();
    wallV = new Structure();
    Sprite* wallSprite = new Sprite();
    wallSprite->Initialize(squareTex, 200, 200, 0, 0, 40, 40);
    wallH->Initialize(wallSprite, true);
    wallV->Initialize(wallSprite, true);

    //make player
    player = new Player();
    SDL_Texture* playerTex = context.txm->LoadTexture(context.renderer, "../../assets/ball.png");
    Sprite* playerSprite = new Sprite();
    playerSprite->Initialize(playerTex, 307, 307, 0, 0, 75, 75);
    player->Initialize(Vector2(15000, 10000), 100, Vector2(0, 0), playerSprite);
    playerSprite->SetDrawLayer(RenderLayer::PLAYER);
    player->SetMovementSpeed(300);
    context.grid->UpdateOccupancy((Entity*)player, &GridCell::AddOther, &GridCell::RemoveOther);
    elements.push_back(player);

    //make attackCone
    AttackCone* attackCone = new AttackCone(15, 200, PI / 5, 0.75);
    SDL_Texture* swooshTex = context.txm->LoadTexture(context.renderer, "../../assets/swoosh.png");
    AnimatedSprite* swooshSpr = new AnimatedSprite();
    swooshSpr->SetFrameDuration(0.02);
    swooshSpr->SetLooping(false);
    swooshSpr->Initialize(swooshTex, 307, 259, 0, 0, grid->GetCellSize(), grid->GetCellSize(), 5, 10);
    attackCone->Initialize(player->GetPosition(), swooshSpr);
    context.grid->UpdateOccupancy((Entity*)player->GetAttackCone(), &GridCell::AddOther, &GridCell::RemoveOther);

    player->SetAttackCone(attackCone);

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

void WorldScene::Process(GameContext& context, float deltaTime) {
    time += deltaTime;
    for (Element* e : elements) {
        e->Process(deltaTime, context);
    }

    woodCount->SetText(to_string(player->GetWood()));
    stoneCount->SetText(to_string(player->GetStone()));
    coinCount->SetText(to_string(player->GetCoins()));

    context.grid->UpdateOccupancy((Entity*)player, &GridCell::AddOther, &GridCell::RemoveOther);
    context.grid->UpdateOccupancy((Entity*)player->GetAttackCone(), &GridCell::AddOther, &GridCell::RemoveOther);
    context.grid->ResolveCollisions(player, context); //collison updates
    context.renderer->cam->Follow(player->GetPosition());//follow player

    //text->SetText(to_string((int)time));

}

void WorldScene::Draw(Renderer* renderer) {
    for (Element* e : elements) {
        e->Draw(renderer);
    }
    for (Sprite* s : UI) {
        s->Draw(renderer);
    }
}

void WorldScene::ChangeStructure(int s, GameContext& context) {
    if (s == currentStructure) return;
    if (buildMode)
        RemoveHoverEffect(currentHoveredCellCoords, context); // clean up old hologram if changed while in build mode
    currentStructure = s;
}

void WorldScene::ToggleBuildMode() {
    buildMode = !buildMode;
}

void WorldScene::LeftMouseClick(GameContext& context) {
    if (buildMode)
        PlaceStructure(context);
    else
        Attack(context);
}

void WorldScene::PlaceStructure(GameContext& context, bool isHologram) {
    if (!buildMode) return;
    Vector2 vec2 = context.im->GetMouseWorldPosition(context.renderer->cam);
    GridCoord coord = context.grid->WorldToGrid(vec2);

    if (!isHologram) RemoveHoverEffect(coord, context);
    if (currentStructure == 1) {
        context.grid->PlaceWall(coord, EdgeDirection::NORTH, wallH);

        if (!player->HasEnoughWood(wallH->GetBuildCost()) || isHologram) return;//if no have money, die
        player->RemoveWood(wallH->GetBuildCost());
    }
    else if (currentStructure == 2) {
        context.grid->PlaceWall(coord, EdgeDirection::WEST, wallV);

        if (!player->HasEnoughStone(wallV->GetBuildCost()) || isHologram) return;//if no have money, die
        player->RemoveStone(wallV->GetBuildCost());
    }
    else if (currentStructure == 3) {
        //grid->PlaceStructure(GetRandomTree(context, grid->GetCellSize()), coord);
    }
}

void WorldScene::RemoveStructure(GameContext& context) {
    if (!buildMode) return;
    Vector2 vec2 = context.im->GetMouseWorldPosition(context.renderer->cam);
    GridCoord coord = context.grid->WorldToGrid(vec2);

    if (currentStructure == 1) {
        context.grid->RemoveWall(coord, EdgeDirection::NORTH);
    }
    else if (currentStructure == 2) {
        context.grid->RemoveWall(coord, EdgeDirection::WEST);
    }
    else {
        context.grid->RemoveStructure(coord);
    }
}

Entity* WorldScene::GetPlayer() {
    return player;
}

void WorldScene::MovePlayer(MovementDir dir, float deltaTime) {
    player->Move(dir, deltaTime);
}

void WorldScene::Attack(GameContext& context) {
    AttackCone* cone = player->GetAttackCone();
    if (cone->CanAttack()) {
        context.grid->ResolveCollisions(cone, context);
        cone->PlayAttack();
    }
}


//HOVERING EFFECT
void WorldScene::UpdateCurrentHoveredCell(GameContext& context) {
    if (!buildMode) {//if no longer building, remove holograms, reset, end
        RemoveHoverEffect(currentHoveredCellCoords, context);
        currentHoveredCellCoords = { -1, -1 };
        return;
    }

    Vector2 pos = context.im->GetMouseWorldPosition(context.renderer->cam);
    GridCell* cell = context.grid->GetCell(context.grid->WorldToGrid(pos));
    if (!cell) {
        RemoveHoverEffect(currentHoveredCellCoords, context);
        currentHoveredCellCoords = { -1, -1 };
        return;
    }

    if (currentHoveredCellCoords != context.grid->WorldToGrid(pos)) {//if on a new cell
        RemoveHoverEffect(currentHoveredCellCoords, context);
        currentHoveredCellCoords = context.grid->WorldToGrid(pos);
    }

    ApplyHoverEffect(cell, context);
}

void WorldScene::ApplyHoverEffect(GridCell* cell, GameContext& context) {
    //walls
    if (currentStructure == 1 || currentStructure == 2) {
        EdgeDirection dir;
        if(currentStructure == 1)  dir = EdgeDirection::NORTH;
        else dir = EdgeDirection::WEST;

        if (cell->HasWall(dir))//if wall, make red
            cell->GetWall(dir)->GetSprite()->SetColor({ 255, 100, 100 });
        else {
            cell->SetHoldingHologramWall(true, dir);
            PlaceStructure(context, true);
            if (!cell->GetWall(dir)) { // placement failed(map edge)
                cell->SetHoldingHologramWall(false, dir);
                return;
            }
            cell->GetWall(dir)->GetSprite()->SetColor({ 100, 100, 255 });
            cell->GetWall(dir)->GetSprite()->SetAlpha(100);
        }
    }
    else {//structures
        if (cell->HasStructure())//trying to place structure where there already is one
            cell->GetStructure()->GetSprite()->SetColor({ 255, 100, 100 }); //highlight red
        else {//no structure
            cell->SetHoldingHologramStruct(true);
            PlaceStructure(context, true);
            cell->GetStructure()->GetSprite()->SetColor({ 100, 100, 255 });
            cell->GetStructure()->GetSprite()->SetAlpha(100);
        }
    }

    return;
}

//remove hologram/red effect
void WorldScene::RemoveHoverEffect(GridCoord coord, GameContext& context) {
    GridCell* cell = context.grid->GetCell(currentHoveredCellCoords);
    if (!cell) return;
    //walls
    if (currentStructure == 1 || currentStructure == 2) {
        EdgeDirection dir;
        if (currentStructure == 1)  dir = EdgeDirection::NORTH;
        else dir = EdgeDirection::WEST;

        if (cell->HasWall(dir)) {
            cell->GetWall(dir)->GetSprite()->SetColor({ 255, 255, 255 });
        }
        else {
            cell->SetHoldingHologramWall(false, dir);
            cell->RemoveWall(dir);
        }
    }
    else {//structures
        if (cell->HasStructure())//trying to place structure where there already is one
            cell->GetStructure()->GetSprite()->SetColor({ 255, 255, 255 }); //highlight red
        else {//no structure
            cell->SetHoldingHologramStruct(false);
            cell->RemoveStructure();
        }
    }
    return;
}