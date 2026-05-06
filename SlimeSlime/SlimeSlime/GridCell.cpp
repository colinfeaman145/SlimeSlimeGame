#include "GridCell.hpp"
#include "Entity.hpp"
#include "Structure.hpp"
#include "Renderer.hpp"
#include <algorithm>
#include "Foliage.hpp"
#include "Nature.hpp"

GridCell::GridCell(Sprite* spr){
    coords = { -1, -1 };
    sprite = spr;
    strctr = nullptr;
    enemies.reserve(16);
    for (int i = 0; i < 2; ++i) {
        walls[i] = nullptr;
        holdingHologramWall[i] = false;
    }
    holdingHologramStruct = false;
}

void GridCell::SetSprite(Sprite* spr) {
    sprite = spr;
}

void GridCell::SetPosition(Vector2 worldPos) {
    if (sprite) sprite->SetPosition((int)worldPos.x, (int)worldPos.y);
    position = worldPos;
}

void GridCell::SetCoords(GridCoord gridPos) {
    coords = gridPos;
}

void GridCell::Draw(Renderer* renderer) {
    if (sprite) sprite->Draw(renderer);
    if (strctr) strctr->Draw(renderer);
    DrawWalls(renderer);
    DrawNature(renderer);
    DrawDrops(renderer);
}

void GridCell::Process(float deltaTime, GameContext& context, bool isRendered) {
    if (sprite) sprite->Process(deltaTime, context);
    if (strctr) strctr->Process(deltaTime, context);
    ProcessWalls(deltaTime, context);
    if (isRendered) ProcessNature(deltaTime, context);
    if (isRendered) ProcessDrops(deltaTime, context);
}

void GridCell::DrawWalls(Renderer* renderer) {
    if (walls[(int)WallDirection::NORTH])  walls[(int)WallDirection::NORTH]->Draw(renderer);
    if (walls[(int)WallDirection::WEST]) walls[(int)WallDirection::WEST]->Draw(renderer);
}

void GridCell::ProcessWalls(float deltaTime, GameContext& context) {
    if (walls[(int)WallDirection::NORTH])  walls[(int)WallDirection::NORTH]->Process(deltaTime, context);
    if (walls[(int)WallDirection::WEST]) walls[(int)WallDirection::WEST]->Process(deltaTime, context);
}

void GridCell::DrawNature(Renderer* renderer) {
    for (Nature* n : nature)
        n->Draw(renderer);
}

void GridCell::ProcessNature(float deltaTime, GameContext& context) {
    for (auto it = nature.begin(); it != nature.end(); ) {//iterate through nature
        Nature* n = *it;
        if (n->IsAlive()) {
            n->Process(deltaTime, context);
            ++it;
        }
        else {//if set dead, remove it
            it = nature.erase(it);
            context.grid->RemoveNature(n);
        }
    }
}

void GridCell::DrawDrops(Renderer* renderer) {
    for (Resource* d : drops)
        d->Draw(renderer);
}

void GridCell::ProcessDrops(float deltaTime, GameContext& context) {
    for (auto it = drops.begin(); it != drops.end(); ) {//iterate through nature
        Resource* d = *it;
        if (d->IsAlive()) {
            d->Process(deltaTime, context);
            ++it;
        }
        else {//if set dead, remove it
            it = drops.erase(it);
            context.grid->RemoveDrop(d);
        }
    }
    //safe to modify vector
    for (Resource* d : drops)
        context.grid->UpdateDropOccupancy(d);
}


vector<Collidable*> GridCell::GetCollidables() const {
    vector<Collidable*> result;
    //enemies, 2 walls, 1 structure, nature, drops, other
    result.reserve(enemies.size() + 3 + nature.size() + entities.size() + drops.size());

    if (strctr && !holdingHologramStruct)
        result.push_back(strctr);

    for (int i = 0; i < 2; ++i)
        if (walls[i] && !holdingHologramWall[i])
            result.push_back(walls[i]);

    for (Enemy* e : enemies)
        result.push_back(e);

    for (Nature* n : nature)
        result.push_back(n);

    for (Resource* d : drops)
        result.push_back(d);

    for (Entity* e : entities)
        result.push_back(e);

    return result;
}


//ENTITIES
void GridCell::AddEnemy(Enemy* enemy) {
    enemies.push_back(enemy);
}

void GridCell::RemoveEnemy(Enemy* enemy) {
    auto it = std::find(enemies.begin(), enemies.end(), enemy);
    if (it != enemies.end()) {
        *it = enemies.back();
        enemies.pop_back();
    }
}

void GridCell::ClearEnemies() {
    enemies.clear();
}

//STRUCTURES
void GridCell::AddStructure(Structure* structure) {
    strctr = structure;
}

void GridCell::RemoveStructure() {
    if (strctr == nullptr) return;//nothing to delete
    delete strctr;
    strctr = nullptr;
}

bool GridCell::HasStructure() const{
    if (holdingHologramStruct) return false; //available to place real structure
    return (strctr != nullptr);
}

void GridCell::SetHoldingHologramStruct(bool b) {
    holdingHologramStruct = b;
}


//WALLS
//assuming wall is free(checked in grid)
bool GridCell::PlaceWall(WallDirection dir, Structure* wall) {
    int i = (int)dir;
    if (walls[i]) return false;//if wall there, return
    walls[i] = wall;
    return true;
}

bool GridCell::RemoveWall(WallDirection dir) {
    int i = (int)dir;
    if (!walls[i]) return false;//if no wall, return
    delete walls[i];
    walls[i] = nullptr;
    return true;
}

bool GridCell::HasWall(WallDirection dir) const {
    int i = (int)dir;
    if (holdingHologramWall[i]) return false; //available to place real wall
    else return walls[i] != nullptr;
}

Structure* GridCell::GetWall(WallDirection dir) const {
    return walls[(int)dir];
}

void GridCell::SetHoldingHologramWall(bool b, WallDirection dir) {
    int i = (int)dir;
    holdingHologramWall[i] = b;
}


//NATURE
void GridCell::SetNaturePosition(Nature* n) {
    int cellWidth = sprite->GetWidth();//this is ghetto but it works
    uniform_int_distribution<int> localLocationGen(cellWidth * 0.1, cellWidth * 0.9);
    Vector2 localOffset = Vector2(localLocationGen(gen), localLocationGen(gen));
    Vector2 worldPosition = position + localOffset;
    n->SetPosition(worldPosition);
}

void GridCell::PlaceNature(Nature* n) {
    nature.push_back(n);
}

void GridCell::RemoveNature(Nature* n) {
    n->SetDead();//if not done alr
    auto it = find(nature.begin(), nature.end(), n);
    if (it != nature.end()) {
        *it = nature.back();
        nature.pop_back();
    }
}


//DROPS
void GridCell::AddDrop(Resource* drop) {
    drops.push_back(drop);
}

void GridCell::RemoveDrop(Resource* drop) {
    auto it = find(drops.begin(), drops.end(), drop);
    if (it != drops.end()) {
        *it = drops.back();
        drops.pop_back();
    }
}


//PLAYER
void GridCell::AddOther(Entity* e) {
    entities.push_back(e);
}

void GridCell::RemoveOther(Entity* e) {
    auto it = find(entities.begin(), entities.end(), e);
    if (it != entities.end()) {
        *it = entities.back();
        entities.pop_back();
    }
}