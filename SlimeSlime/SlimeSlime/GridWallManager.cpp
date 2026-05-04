#include "Grid.hpp"

bool Grid::CanPlaceWall(GridCoord coord, EdgeDirection dir) {
    GridCell* cell = GetCell(coord);

    //get neighbor cell
    GridCoord borderCellCoord = coord;
    if (dir == EdgeDirection::NORTH) borderCellCoord.row--;
    else if (dir == EdgeDirection::WEST) borderCellCoord.col--;
    else return false;//invalid direciton

    GridCell* borderCell = GetCell(borderCellCoord);//nullptr if invalid

    if (!cell) return false;
    if (borderCell == nullptr) return false;//trying to add wall to edge of map
    if (cell->HasWall(dir)) return false;

    return true;
}

bool Grid::PlaceWall(GridCoord coord, EdgeDirection dir, Structure* w) {

    if (!CanPlaceWall(coord, dir)) return false;

    GridCell* cell = GetCell(coord);
    Structure* wall = new Structure(*w);


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