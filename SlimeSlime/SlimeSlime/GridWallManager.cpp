#include "Grid.hpp"
#include "Foliage.hpp"

bool Grid::CanPlaceWall(GridCoord coord, WallDirection dir) {
    GridCell* cell = GetCell(coord);

    //get neighbor cell
    GridCoord borderCellCoord = coord;
    if (dir == WallDirection::NORTH) borderCellCoord.row--;
    else if (dir == WallDirection::WEST) borderCellCoord.col--;
    else return false;//invalid direciton

    GridCell* borderCell = GetCell(borderCellCoord);//nullptr if invalid

    if (!cell) return false;
    if (borderCell == nullptr) return false;//trying to add wall to edge of map
    if (cell->HasWall(dir)) return false;

    return true;
}

bool Grid::PlaceWall(GridCoord coord, WallDirection dir, Structure* w) {

    if (!CanPlaceWall(coord, dir)) return false;

    GridCell* cell = GetCell(coord);
    Structure* wall = new Structure(*w);


    // Calculate wall world position and rotation
    Vector2 cellWorld = GridToWorld(coord);
    Vector2 wallPos;

    switch (dir) {
    case WallDirection::NORTH:
        wall->ChangeSize(cellSize * 1.25, cellSize * 0.25);
        wall->SetCollisionBound(CollisionShape::MakeAABB(cellSize , cellSize * 0.25, Vector2(cellSize * 0.125, 0)));
        wallPos = Vector2(cellWorld.x - (cellSize * 0.125), cellWorld.y - (cellSize * 0.125));
        break;
    case WallDirection::WEST:
        wall->ChangeSize(cellSize * 0.25, cellSize * 1.22);
        wall->SetCollisionBound(CollisionShape::MakeAABB(cellSize * 0.25, cellSize, Vector2(0, cellSize * 0.125)));
        wallPos = Vector2(cellWorld.x - (cellSize * 0.125), cellWorld.y - (cellSize * 0.125));
        break;
    }

    wall->SetPosition(wallPos);
    wall->GetSprite()->SetDrawLayer(RenderLayer::STRUCTURES);
    cell->PlaceWall(dir, wall);
    vector<Collidable*> collidables = GetNearbyCollidables(coord, 2);
    for (Collidable* c : collidables) {//remove folaige where wall placed
        if (Foliage* f = dynamic_cast<Foliage*>(c)) {
            ResolveCollisions(f);
        }
    }

    return true;
}

bool Grid::RemoveWall(GridCoord coord, WallDirection dir) {
    GridCell* cell = GetCell(coord);
    if (!cell || !cell->HasWall(dir)) return false;

    cell->RemoveWall(dir);
    return true;
}