#include "GridCell.hpp"
#include "Entity.hpp"
#include "Structure.hpp"
#include "Renderer.hpp"
#include <algorithm>

GridCell::GridCell(Sprite* spr){
    coords = { -1, -1 };
    sprite = spr;
    strctr = nullptr;
    entities.reserve(16);
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
}

void GridCell::Process(float deltaTime, GameContext& context) {
    if (sprite) sprite->Process(deltaTime, context);
    if (strctr) strctr->Process(deltaTime, context);
    ProcessWalls(deltaTime, context);
}

void GridCell::DrawWalls(Renderer* renderer) {
    if (walls[(int)EdgeDirection::NORTH])  walls[(int)EdgeDirection::NORTH]->Draw(renderer);
    if (walls[(int)EdgeDirection::WEST]) walls[(int)EdgeDirection::WEST]->Draw(renderer);
}

void GridCell::ProcessWalls(float deltaTime, GameContext& context) {
    if (walls[(int)EdgeDirection::NORTH])  walls[(int)EdgeDirection::NORTH]->Process(deltaTime, context);
    if (walls[(int)EdgeDirection::WEST]) walls[(int)EdgeDirection::WEST]->Process(deltaTime, context);
}

void GridCell::DrawNature(Renderer* renderer) {
    for (Nature* n : nature) {
        n->Structure::Draw(renderer);
    }
}


void GridCell::AddEntity(Entity* entity) {
    entities.push_back(entity);
}

void GridCell::RemoveEntity(Entity* entity) {
    auto it = std::find(entities.begin(), entities.end(), entity);
    if (it != entities.end()) {
        *it = entities.back();
        entities.pop_back();
    }
}

void GridCell::ClearEntities() {
    entities.clear();
}

vector<Collidable*> GridCell::GetCollidables() const {
    vector<Collidable*> result;
    result.reserve(entities.size() + 3);//entities, 2 walls, 1 structure

    for (Entity* e : entities)
        result.push_back(e);

    if (strctr && !holdingHologramStruct)
        result.push_back(strctr);

    for (int i = 0; i < 2; ++i)
        if (walls[i] && !holdingHologramWall[i])
            result.push_back(walls[i]);

    for (Structure* n : nature)
        result.push_back(n);

    return result;
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



//assuming wall is free(checked in grid)
bool GridCell::PlaceWall(EdgeDirection dir, Structure* wall) {
    int i = (int)dir;
    if (walls[i]) return false;//if wall there, return
    walls[i] = wall;
    return true;
}

bool GridCell::RemoveWall(EdgeDirection dir) {
    int i = (int)dir;
    if (!walls[i]) return false;//if no wall, return
    delete walls[i];
    walls[i] = nullptr;
    return true;
}

bool GridCell::HasWall(EdgeDirection dir) const {
    int i = (int)dir;
    if (holdingHologramWall[i]) return false; //available to place real wall
    else return walls[i] != nullptr;
}

Structure* GridCell::GetWall(EdgeDirection dir) const {
    return walls[(int)dir];
}

void GridCell::SetHoldingHologramWall(bool b, EdgeDirection dir) {
    int i = (int)dir;
    holdingHologramWall[i] = b;
}


//NATURE
void GridCell::PlaceNature(Nature* structure) {
    nature.push_back(structure);

    int cellWidth = sprite->GetWidth();//this is ghetto but it works
    uniform_int_distribution<int> localLocationGen(cellWidth * 0.1, cellWidth * 0.9);
    Vector2 localOffset = Vector2(localLocationGen(gen), localLocationGen(gen));
    Vector2 worldPosition = position + localOffset;
    structure->SetPosition(worldPosition);
}

void GridCell::RemoveNature(Nature* structure) {
    nature.erase(remove(nature.begin(), nature.end(), structure), nature.end());
    delete structure;
}