#include "WorldScene.hpp"

//HOVERING EFFECT
void WorldScene::UpdateCurrentHoveredCell(bool canAfford) {
    if (!buildMode) {//if no longer building, remove holograms, reset, end
        RemoveHoverEffect(currentHoveredCellCoords);
        currentHoveredCellCoords = { -1, -1 };
        lastHoveredStructure = -1;
        return;
    }

    Vector2 pos = context.im->GetMouseWorldPosition(context.renderer->cam);
    GridCoord newCoord = context.grid->WorldToGrid(pos);
    GridCell* cell = context.grid->GetCell(newCoord);

    if (!cell) {
        RemoveHoverEffect(currentHoveredCellCoords);
        currentHoveredCellCoords = { -1, -1 };
        lastHoveredStructure = -1;
        return;
    }

    if (currentHoveredCellCoords != newCoord || lastHoveredStructure != currentStructure) {//if on a new cell — place hologram
        RemoveHoverEffect(currentHoveredCellCoords);
        currentHoveredCellCoords = newCoord;
        ApplyHoverEffect(cell, canAfford);
    }
    else {//same cell — just recolor, no place/remove
        UpdateHoverColor(cell, canAfford);
    }
}

void WorldScene::UpdateHoverColor(GridCell* cell, bool canAfford) {
    Color c = canAfford ? Color{ 100, 100, 255 } : Color{ 255, 100, 100 };
    int alpha = canAfford ? 100 : 200;

    //walls
    if (currentStructure == 1 || currentStructure == 2) {
        WallDirection dir = currentStructure == 1 ? WallDirection::NORTH : WallDirection::WEST;
        if (cell->HasWall(dir)) {
            cell->GetWall(dir)->GetSprite()->SetColor(c);
            cell->GetWall(dir)->GetSprite()->SetAlpha(alpha);
        }
    }
    else {//structures
        if (!context.grid->CanPlaceStructure(cell->GetCoords())) {
            cell->GetStructure()->GetSprite()->SetColor(c);
            cell->GetStructure()->GetSprite()->SetAlpha(alpha);
        }
    }
}

void WorldScene::ApplyHoverEffect(GridCell* cell, bool canAfford) {
    //walls
    if (currentStructure == 1 || currentStructure == 2) {
        WallDirection dir;
        if (currentStructure == 1)  dir = WallDirection::NORTH;
        else dir = WallDirection::WEST;

        if (cell->HasWall(dir))//if wall, make red
            cell->GetWall(dir)->GetSprite()->SetColor({ 255, 100, 100 });
        else {
            cell->SetHoldingHologramWall(true, dir);
            PlaceStructure(true);
            if (!cell->GetWall(dir)) { // placement failed(map edge)
                cell->SetHoldingHologramWall(false, dir);
                return;
            }
            cell->GetWall(dir)->GetSprite()->SetColor({ 100, 100, 255 });
            cell->GetWall(dir)->GetSprite()->SetAlpha(100);
        }

        if (!canAfford) {
            cell->GetWall(dir)->GetSprite()->SetColor({ 255, 100, 100 });
            cell->GetWall(dir)->GetSprite()->SetAlpha(200);
        }
    }
    else { //structures
        if (cell->HasStructure()) {
            cell->GetStructure()->GetSprite()->SetColor({ 255, 100, 100 });
        }
        else {
            bool blocked = !context.grid->CanPlaceStructure(cell->GetCoords());
            cell->SetHoldingHologramStruct(true);
            PlaceStructure(true); // places hologram even if blocked by nature
            if (!cell->GetStructure()) {
                cell->SetHoldingHologramStruct(false);
                return;
            }
            if (blocked || !canAfford) {
                cell->GetStructure()->GetSprite()->SetColor({ 255, 100, 100 }); //highlight red
                cell->GetStructure()->GetSprite()->SetAlpha(200);
            }
            else {
                cell->GetStructure()->GetSprite()->SetColor({ 100, 100, 255 });
                cell->GetStructure()->GetSprite()->SetAlpha(100);
            }
        }
    }


    return;
}

//remove hologram/red effect
void WorldScene::RemoveHoverEffect(GridCoord coord) {
    GridCell* cell = context.grid->GetCell(currentHoveredCellCoords);
    if (!cell) return;
    //walls
    if (currentStructure == 1 || currentStructure == 2) {
        WallDirection dir;
        if (currentStructure == 1)  dir = WallDirection::NORTH;
        else dir = WallDirection::WEST;

        if (cell->HasWall(dir)) {
            cell->GetWall(dir)->GetSprite()->SetColor({ 255, 255, 255 });
            cell->GetWall(dir)->GetSprite()->SetAlpha(255);
        }
        else {
            cell->SetHoldingHologramWall(false, dir);
            cell->RemoveWall(dir);
        }
    }
    else {//structures
        if (cell->HasStructure()) {//trying to place structure where there already is one
            cell->GetStructure()->GetSprite()->SetColor({ 255, 255, 255 }); //highlight red
            cell->GetStructure()->GetSprite()->SetAlpha(255);
        }
        else {//no structure
            cell->SetHoldingHologramStruct(false);
            cell->RemoveStructure();
        }
    }
    return;
}