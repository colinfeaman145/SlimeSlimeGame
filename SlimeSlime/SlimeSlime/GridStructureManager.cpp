#include "Grid.hpp"
#include "Foliage.hpp"

bool Grid::CanPlaceStructure(GridCoord coord) const {
    if (!IsValidCoord(coord)) return false;
    return !cells[coord.row][coord.col]->HasStructure();
}

bool Grid::PlaceStructure(Structure* structure, GridCoord coord) {
    if (!CanPlaceStructure(coord)) return false;

    Structure* s = structure->Clone();
    s->GetSprite()->SetDrawSize(cellSize, cellSize);
    cells[coord.row][coord.col]->AddStructure(s);
    s->SetPosition(GridToWorld(coord));
    s->GetSprite()->SetDrawLayer(RenderLayer::STRUCTURES);

    vector<Collidable*> collidables = GetNearbyCollidables(coord, 2);
    for (Collidable* c : collidables) {//remove folaige where wall placed
        if (Foliage* f = dynamic_cast<Foliage*>(c)) {
            ResolveCollisions(f);
        }
    }

    return true;
}

bool Grid::RemoveStructure(GridCoord coord) {
    if (!IsValidCoord(coord)) return false;

    GridCell* cell = cells[coord.row][coord.col];
    if (!cell->HasStructure()) return false;

    cell->RemoveStructure();
    return true;
}

bool Grid::RemoveStructure(Structure* structure) {
    for (int row = 0; row < gridHeight; ++row) {
        for (int col = 0; col < gridWidth; ++col) {
            if (cells[row][col]->GetStructure() == structure)
                return RemoveStructure({ col, row });
            else if (cells[row][col]->GetWall(WallDirection::NORTH) == structure)
                return RemoveWall({ col, row }, WallDirection::NORTH);
            else if (cells[row][col]->GetWall(WallDirection::WEST) == structure)
                return RemoveWall({ col, row }, WallDirection::WEST);
        }
    }
    return false;
}

vector<Structure*> Grid::GetNearbyStructures(GridCoord coord, int radius) {
    vector<Structure*> result;
    for (GridCell* cell : GetNeighbourCells(coord, radius)) {
        result.push_back(cell->GetStructure());
        result.push_back(cell->GetWall(WallDirection::NORTH));
        result.push_back(cell->GetWall(WallDirection::WEST));
    }
    return result;
}