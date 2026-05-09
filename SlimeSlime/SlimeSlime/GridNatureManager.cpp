#include "Grid.hpp"
#include "Nature.hpp"
#include "Foliage.hpp"

void Grid::PlaceNature(uniform_int_distribution<int> spreadChance, NatureType type, GridCell* cell) {
    uniform_int_distribution<int> changeType(1, 10);

    if (spreadChance(gen) != 3) return;//only spread when chance hits

    Nature* nature = nullptr;
    //Are we changing nature type?
    switch (changeType(gen)) {
    case(1)://yes 
        nature = GetRandomTree();
        break;
    case(2):
        nature = GetRandomRock();
        break;
    case(3):
        nature = GetRandomBush();
        break;
    case(4):
        nature = GetRandomStump();
        break;
    default://no
        switch (type) {//fuck everything, nested switch
        case(NatureType::TREE):
            nature = GetRandomTree();
            break;
        case(NatureType::ROCK):
            nature = GetRandomRock();
            break;
        case(NatureType::BUSH):
            nature = GetRandomBush();
            break;
        case(NatureType::STUMP):
            nature = GetRandomStump();
            break;
        case(NatureType::FOLIAGE):
            nature = GetRandomFoliage();
            break;
        }
    };
    cell->SetNaturePosition(nature);//set so World Position is accurate

    //place in cells it occupies
    UpdateOccupancy(nature, &GridCell::PlaceNature, &GridCell::RemoveNature);
    if (HasCollision(nature)) {// overlaps something, don't place it
        RemoveNature(nature);
        return;
    }

    //recurse
    vector<GridCell*> neighbors = GetNeighbourCells(cell->GetCoords(), 1);
    for (GridCell* c : neighbors) {
        PlaceNature(uniform_int_distribution<int>(spreadChance.a(), spreadChance.b() + 3), nature->GetNatureType(), c);
    }
}

void Grid::PlaceFoliage(uniform_int_distribution<int> spreadChance, GridCell* cell) {

    if (spreadChance(gen) != 3) return;//only spread when chance hits

    Nature* nature = nullptr;
    for (int i = 0; i < 3; i++) {
        nature = GetRandomFoliage();

        cell->SetNaturePosition(nature);
        UpdateOccupancy(nature, &GridCell::PlaceNature, &GridCell::RemoveNature);
        if (HasCollision(nature)) {// overlaps something, don't place it
            RemoveNature(nature);
            return;
        }
    }

    //recurse
    vector<GridCell*> neighbors = GetNeighbourCells(cell->GetCoords(), 1);
    for (GridCell* c : neighbors) {
        PlaceFoliage(uniform_int_distribution<int>(spreadChance.a(), spreadChance.b() + 1), c);
    }
}

void Grid::RemoveNature(Nature* n) {
    if (!n) return;

    vector<GridCell*> cells = FindEntityCells(n);
    for (GridCell* cell : cells)
        cell->RemoveNature(n);

    delete n;
}

NatureType Grid::GetRandomNatureType() {
    uniform_int_distribution<int> natureTypeGen(1, 20);//30% tree 5% stump 40% rock 15% bush

    int nt = natureTypeGen(gen);
    NatureType type;
    if (1 <= nt && nt <= 6)
        type = NatureType::TREE;
    else if (7 <= nt && nt <= 7)
        type = NatureType::STUMP;
    else if (8 <= nt && nt <= 15)
        type = NatureType::ROCK;
    else if (16 <= nt && nt <= 20)
        type = NatureType::BUSH;

    return type;
}