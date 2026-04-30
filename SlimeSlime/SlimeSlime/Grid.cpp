#include "Grid.hpp"
#include "Structure.hpp"
#include "Renderer.hpp"
#include <algorithm>
#include <cmath>

Grid::Grid(int worldWidth, int worldHeight, int cellSize)
    : cellSize(cellSize)
    , gridWidth((int)ceil((float)worldWidth / cellSize))
    , gridHeight((int)ceil((float)worldHeight / cellSize))
    , cells(nullptr) {
}

Grid::~Grid() {
    if (!cells) return;

    for (int row = 0; row < gridHeight; ++row) {
        if (cells[row]) {
            for (int col = 0; col < gridWidth; ++col) {
                delete cells[row][col];
                cells[row][col] = nullptr;
            }
            delete[] cells[row];
            cells[row] = nullptr;
        }
    }
    delete[] cells;
    cells = nullptr;
}

//TODO accept file path and construct grid accordingly
bool Grid::Initialize(SDL_Texture* cellTexture) {
    cells = new GridCell** [gridHeight];

    for (int row = 0; row < gridHeight; ++row) {
        cells[row] = new GridCell* [gridWidth];
        for (int col = 0; col < gridWidth; ++col) {
            Sprite* spr = new Sprite();
            spr->Initialize(cellTexture, 740, 740, 0, 0, cellSize, cellSize);
            spr->SetColor({ 220, 255, 220 });

            GridCell* cell = new GridCell(spr);
            cell->SetPosition(GridToWorld({ col, row }));
            cell->GetSprite()->SetDrawLayer(RenderLayer::GROUND);
            cells[row][col] = cell;
        }
    }
    return true;
}

void Grid::Draw(Renderer* renderer) {
    for (int row = 0; row < gridHeight; ++row)
        for (int col = 0; col < gridWidth; ++col)
            cells[row][col]->Draw(renderer);
}

void Grid::Process(float deltaTime, GameContext& context) {
    for (int row = 0; row < gridHeight; ++row)
        for (int col = 0; col < gridWidth; ++col)
            cells[row][col]->Process(deltaTime, context);
    return;
}


bool Grid::IsValidCoord(GridCoord coord) const {
    return coord.col >= 0 && coord.col < gridWidth && coord.row >= 0 && coord.row < gridHeight;
}

GridCell* Grid::GetCell(GridCoord coord) {
    if (!IsValidCoord(coord)) return nullptr;
    return cells[coord.row][coord.col];
}

GridCell* Grid::GetCell(int col, int row) {
    return GetCell({ col, row });
}


//CORDINATE COVERSION
GridCoord Grid::WorldToGrid(Vector2 worldPos) const {
    return {
        (int)floor(worldPos.x / cellSize),
        (int)floor(worldPos.y / cellSize)
    };
}

Vector2 Grid::GridToWorld(GridCoord coord) const {
    return Vector2(
        (float)(coord.col * cellSize),
        (float)(coord.row * cellSize)
    );
}

Vector2 Grid::SnapToGrid(Vector2 worldPos) const {
    GridCoord coord = WorldToGrid(worldPos);

    coord.col = max(0, min(coord.col, gridWidth - 1));
    coord.row = max(0, min(coord.row, gridHeight - 1));

    return GridToWorld(coord);
}


//STRUCTURE MANAGEMENT
bool Grid::CanPlaceStructure(GridCoord coord) const {
    if (!IsValidCoord(coord)) return false;
    return !cells[coord.row][coord.col]->HasStructure();
}

bool Grid::PlaceStructure(Structure* structure, GridCoord coord) {
    if (!CanPlaceStructure(coord)) return false;
    Structure* s = new Structure(*structure);

    s->GetSprite()->SetDrawSize(cellSize, cellSize);
    cells[coord.row][coord.col]->AddStructure(s);
    s->SetPosition(GridToWorld(coord));
    s->GetSprite()->SetDrawLayer(RenderLayer::STRUCTURES);
    return true;
}

bool Grid::RemoveStructure(GridCoord coord) {
    if (!IsValidCoord(coord)) return false;

    GridCell* cell = cells[coord.row][coord.col];
    if (!cell->HasStructure()) return false;

    cell->RemoveStructure();
    return true;
}


//ENTITY MANAGEMENT
void Grid::UpdateEntity(Entity* entity) {
    if (!entity || !entity->IsAlive()) return;

    Vector2 pos = entity->GetPosition();
    float r = entity->GetRadius();

    GridOccupancy next = {
        (int)floor((pos.x - r) / cellSize),
        (int)floor((pos.x + r) / cellSize),
        (int)floor((pos.y - r) / cellSize),
        (int)floor((pos.y + r) / cellSize)
    };

    const GridOccupancy& prev = entity->GetOccupancy();
    if (next == prev) return; //if no change, dont update anything

    //Remove from old cells
    for (int row = prev.minRow; row <= prev.maxRow; ++row) {
        for (int col = prev.minCol; col <= prev.maxCol; ++col) {
            if (GridCell* cell = GetCell(col, row))
                cell->RemoveEntity(entity);
        }
    }

    //Insert into new cells
    for (int row = next.minRow; row <= next.maxRow; ++row) {
        for (int col = next.minCol; col <= next.maxCol; ++col) {
            if (GridCell* cell = GetCell(col, row))
                cell->AddEntity(entity);
        }
    }

    entity->SetOccupancy(next);
}

void Grid::RemoveEntity(Entity* entity) {
    if (!entity) return;

    const GridOccupancy& occ = entity->GetOccupancy();
    for (int row = occ.minRow; row <= occ.maxRow; ++row) {
        for (int col = occ.minCol; col <= occ.maxCol; ++col) {
            if (GridCell* cell = GetCell(col, row))
                cell->RemoveEntity(entity);
        }
    }

    entity->SetOccupancy({ -1, -1, -1, -1 });
}


//NEIGHBOR CHECKING
//returns cells in a radius of cell at input coordinates
//likely to only be used as helper in GetNearbyEntities
vector<GridCell*> Grid::GetNeighbourCells(GridCoord coord, int radius) {
    vector<GridCell*> result;

    for (int row = coord.row - radius; row <= coord.row + radius; ++row)
        for (int col = coord.col - radius; col <= coord.col + radius; ++col)
            if (GridCell* cell = GetCell(col, row))
                result.push_back(cell);

    return result;
}

vector<Entity*> Grid::GetNearbyEntities(GridCoord coord, int radius) {
    vector<Entity*> result;
    for (GridCell* cell : GetNeighbourCells(coord, radius)) {
        const auto& entities = cell->GetEntities();
        result.insert(result.end(), entities.begin(), entities.end());//add all entities individually to result
    }
    return result;
}

vector<Collidable*> Grid::GetNearbyCollidables(GridCoord coord, int radius) {
    vector<Collidable*> result;
    for (GridCell* cell : GetNeighbourCells(coord, radius)) {
        auto c = cell->GetCollidables();
        result.insert(result.end(), c.begin(), c.end());
    }
    return result;
}

//vector<Structure*> Grid::GetNearbyStructures(GridCoord coord, int radius) {
//    vector<Structure*> result;
//    for (GridCell* cell : GetNeighbourCells(coord, radius))
//        if (cell->HasStructure())
//            result.push_back(cell->GetStructure());
//    return result;
//}

//check if entity collides with anything. Act if they do
void Grid::ResolveCollisions(Entity* entity, GameContext& context) {

    if (!entity) return;

    //find search area
    const GridOccupancy& occ = entity->GetOccupancy();
    int searchRadius = max(occ.maxCol - occ.minCol, occ.maxRow - occ.minRow) + 3;
    GridCoord center = {(occ.minCol + occ.maxCol) / 2, (occ.minRow + occ.maxRow) / 2};

    //get entity to check for collision
    vector<Collidable*> candidates = GetNearbyCollidables(center, searchRadius);

    //check entities for collision
    for (Collidable* other : candidates) {
        if (other == entity) continue;//if self

        Vector2 penetration;
        if (Collision::TestShapes(
            entity->GetCollisionBound(), entity->GetPosition(),
            other->GetCollisionBound(), other->GetPosition(),
            penetration)) 
        {
            entity->HandleCollision(other, penetration, context);
            other->HandleCollision(entity, -penetration, context);
        }
    }
}


//WALLS
bool Grid::PlaceWall(GridCoord coord, EdgeDirection dir, Structure* w) {
    GridCell* cell = GetCell(coord);
    Structure* wall = new Structure(*w);

    //get neighbor cell
    GridCoord borderCellCoord = coord;
    if (dir == EdgeDirection::NORTH) borderCellCoord.row--;
    else if (dir == EdgeDirection::WEST) borderCellCoord.col--;
    else return false;//invalid direciton

    GridCell* borderCell = GetCell(borderCellCoord);//nullptr if invalid

    if (!cell) return false;
    if (borderCell == nullptr) return false;//trying to add wall to edge of map
    if (cell->HasWall(dir)) return false;

    // Calculate wall world position and rotation
    Vector2 cellWorld = GridToWorld(coord);
    Vector2 wallPos;

    switch (dir) {
        case EdgeDirection::NORTH:
            wall->ChangeSize(cellSize * 1.25, cellSize * 0.25);
            wallPos = Vector2(cellWorld.x - (cellSize * 0.125), cellWorld.y - (cellSize * 0.125));
            break;
        case EdgeDirection::WEST:
            wall->ChangeSize(cellSize * 0.25, cellSize * 1.22);
            wallPos = Vector2(cellWorld.x - (cellSize * 0.125), cellWorld.y - (cellSize * 0.125));
            break;
    }

    wall->SetPosition(wallPos);
    wall->GetSprite()->SetDrawLayer(RenderLayer::STRUCTURES);
    cell->PlaceWall(dir, wall);

    return true;
}

bool Grid::RemoveWall(GridCoord coord, EdgeDirection dir) {
    GridCell* cell = GetCell(coord);
    if (!cell || !cell->HasWall(dir)) return false;

    cell->RemoveWall(dir);
    return true;
}