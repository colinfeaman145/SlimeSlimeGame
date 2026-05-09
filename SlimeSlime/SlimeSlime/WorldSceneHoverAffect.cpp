#include "WorldScene.hpp"

//HOVERING EFFECT
void WorldScene::UpdateCurrentHoveredCell(bool canAfford) {
    if (!buildMode) {//if no longer building, remove holograms, reset, end
        RemoveHoverEffect(currentHoveredCellCoords);
        currentHoveredCellCoords = { -1, -1 };
        return;
    }

    Vector2 pos = context.im->GetMouseWorldPosition(context.renderer->cam);
    GridCell* cell = context.grid->GetCell(context.grid->WorldToGrid(pos));
    if (!cell) {
        RemoveHoverEffect(currentHoveredCellCoords);
        currentHoveredCellCoords = { -1, -1 };
        return;
    }

    if (currentHoveredCellCoords != context.grid->WorldToGrid(pos)) {//if on a new cell
        RemoveHoverEffect(currentHoveredCellCoords);
        currentHoveredCellCoords = context.grid->WorldToGrid(pos);
    }

    ApplyHoverEffect(cell, canAfford);
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
    else {//structures
        if (cell->HasStructure())//trying to place structure where there already is one
            cell->GetStructure()->GetSprite()->SetColor({ 255, 100, 100 }); //highlight red
        else {//no structure
            cell->SetHoldingHologramStruct(true);
            PlaceStructure(true);
            cell->GetStructure()->GetSprite()->SetColor({ 100, 100, 255 });
            cell->GetStructure()->GetSprite()->SetAlpha(100);
        }

        if (!canAfford) {
            cell->GetStructure()->GetSprite()->SetColor({ 255, 100, 100 });
            cell->GetStructure()->GetSprite()->SetAlpha(100);
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