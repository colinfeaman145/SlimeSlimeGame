#include "Grid.hpp"

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