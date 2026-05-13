#include "Grid.hpp"
#include "Foliage.hpp"

bool Grid::CanPlaceStructure(GridCoord coord) {
    if (!IsValidCoord(coord)) return false;
    if (cells[coord.row][coord.col]->HasStructure()) return false;

    //build an AABB for the target cell(some tolerance)
    CollisionShape cellShape = CollisionShape::MakeAABB(cellSize * 0.85, cellSize * 0.85, Vector2(cellSize * 0.125, cellSize * 0.125));
    Vector2 cellPos = GridToWorld(coord);

    for (Collidable* n : context.grid->GetNearbyCollidables(coord, 1)) {
        if (n->GetCollidableType() != CollidableType::NATURE) continue;
        if (dynamic_cast<Foliage*>(n)) continue;//ignore foliage

        Vector2 penetration;
        if (Collision::TestShapes(cellShape, cellPos, n->GetCollisionBound(), n->GetPosition(), penetration))
            return false; //collision box actually overlaps the cell
    }
    return true;
}

Structure* Grid::PlaceStructure(Structure* structure, GridCoord coord, bool hologram) {
    if (!IsValidCoord(coord)) return nullptr;
    if (cells[coord.row][coord.col]->HasStructure()) return nullptr; //always block if structure present
    if (!hologram && !CanPlaceStructure(coord)) return nullptr; //only collidable check for real placements

    Structure* s = structure->Clone();
    s->GetSprite()->SetDrawSize(cellSize, cellSize);
    cells[coord.row][coord.col]->AddStructure(s);
    s->SetPosition(GridToWorld(coord));
    s->GetSprite()->SetDrawLayer(RenderLayer::STRUCTURES);

    if (!hologram) {
        vector<Collidable*> nearby(GetNearbyCollidables(coord, 2)); //remove foliage when placed
        for (Collidable* c : nearby) {
            if (Foliage* f = dynamic_cast<Foliage*>(c))
                ResolveCollisions(f);
        }
    }
    return s;
}

bool Grid::RemoveStructure(GridCoord coord) {
    if (!IsValidCoord(coord)) return false;

    GridCell* cell = cells[coord.row][coord.col];
    if (!cell->HasStructure()) return false;

    bool isAtlas = cell->GetStructure() == atlas;

    cell->RemoveStructure();
    if (isAtlas) atlas = nullptr;
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
        if (auto s = cell->GetStructure()) result.push_back(s);
        if (auto s = cell->GetWall(WallDirection::NORTH)) result.push_back(s);
        if (auto s = cell->GetWall(WallDirection::WEST)) result.push_back(s);
    }
    return result;
}