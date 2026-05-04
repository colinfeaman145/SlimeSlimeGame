#include "Grid.hpp"
#include "Nature.hpp"
#include "Foliage.hpp"

void Grid::PlaceNature(uniform_int_distribution<int> spreadChance, NatureType type, GridCell* cell, GameContext& context) {
    uniform_int_distribution<int> changeType(1, 10);

    if (spreadChance(gen) != 3) return;//only spread when chance hits

    Nature* nature = nullptr;
    //Are we changing nature type?
    switch (changeType(gen)) {
    case(1)://yes 
        nature = GetRandomTree(context);
        break;
    case(2):
        nature = GetRandomRock(context);
        break;
    case(3):
        nature = GetRandomBush(context);
        break;
    case(4):
        nature = GetRandomStump(context);
        break;
    default://no
        switch (type) {//fuck everything, nested switch
        case(NatureType::TREE):
            nature = GetRandomTree(context);
            break;
        case(NatureType::ROCK):
            nature = GetRandomRock(context);
            break;
        case(NatureType::BUSH):
            nature = GetRandomBush(context);
            break;
        case(NatureType::STUMP):
            nature = GetRandomStump(context);
            break;
        case(NatureType::FOLIAGE):
            nature = GetRandomFoliage(context);
            break;
        }
    };
    cell->SetNaturePosition(nature);//set so World Position is accurate

    ////ensure placed in cell where bounding box is(holy fuck this took so long to discover I needed this)
    //Vector2 csPos = nature->GetCollisionBound().WorldPosition(nature->GetPosition());
    //GridCoord baseCoord = WorldToGrid(csPos);
    //GridCell* baseCell = GetCell(baseCoord);

    //if (baseCell && baseCell != cell) {
    //    baseCell->SetNaturePosition(nature);//adjust location
    //}

    //place in cells it occupies
    UpdateOccupancy(nature, &GridCell::PlaceNature, &GridCell::RemoveNature);
    if (ResolveCollisions(nature, context)) {// overlaps something, don't place it
        RemoveNature(nature);
        return;
    }

    //recurse
    vector<GridCell*> neighbors = GetNeighbourCells(cell->GetCoords(), 1);
    for (GridCell* c : neighbors) {
        PlaceNature(uniform_int_distribution<int>(spreadChance.a(), spreadChance.b() + 3), nature->GetNatureType(), c, context);
    }
}

void Grid::PlaceFoliage(uniform_int_distribution<int> spreadChance, GridCell* cell, GameContext& context) {

    if (spreadChance(gen) != 3) return;//only spread when chance hits

    Nature* nature = nullptr;
    for (int i = 0; i < 3; i++) {
        nature = GetRandomFoliage(context);

        cell->SetNaturePosition(nature);
        UpdateOccupancy(nature, &GridCell::PlaceNature, &GridCell::RemoveNature);
        if (ResolveCollisions(nature, context)) {// overlaps something, don't place it
            RemoveNature(nature);
            return;
        }
    }

    //recurse
    vector<GridCell*> neighbors = GetNeighbourCells(cell->GetCoords(), 1);
    for (GridCell* c : neighbors) {
        PlaceFoliage(uniform_int_distribution<int>(spreadChance.a(), spreadChance.b() + 1), c, context);
    }
}

void Grid::RemoveNature(Nature* n) {
    if (!n) return;

    vector<GridCell*> cells = FindEntityCells(n);
    for (GridCell* cell : cells)
        cell->RemoveNature(n);

    delete n;
}