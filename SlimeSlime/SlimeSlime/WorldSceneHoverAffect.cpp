#include "WorldScene.hpp"

//HOVERING EFFECT
void WorldScene::UpdateCurrentHoveredCell(GameContext& context) {
    if (!buildMode) {//if no longer building, remove holograms, reset, end
        RemoveHoverEffect(currentHoveredCellCoords, context);
        currentHoveredCellCoords = { -1, -1 };
        return;
    }

    Vector2 pos = context.im->GetMouseWorldPosition(context.renderer->cam);
    GridCell* cell = context.grid->GetCell(context.grid->WorldToGrid(pos));
    if (!cell) {
        RemoveHoverEffect(currentHoveredCellCoords, context);
        currentHoveredCellCoords = { -1, -1 };
        return;
    }

    if (currentHoveredCellCoords != context.grid->WorldToGrid(pos)) {//if on a new cell
        RemoveHoverEffect(currentHoveredCellCoords, context);
        currentHoveredCellCoords = context.grid->WorldToGrid(pos);
    }

    ApplyHoverEffect(cell, context);
}

void WorldScene::ApplyHoverEffect(GridCell* cell, GameContext& context) {
    //walls
    if (currentStructure == 1 || currentStructure == 2) {
        WallDirection dir;
        if (currentStructure == 1)  dir = WallDirection::NORTH;
        else dir = WallDirection::WEST;

        if (cell->HasWall(dir))//if wall, make red
            cell->GetWall(dir)->GetSprite()->SetColor({ 255, 100, 100 });
        else {
            cell->SetHoldingHologramWall(true, dir);
            PlaceStructure(context, true);
            if (!cell->GetWall(dir)) { // placement failed(map edge)
                cell->SetHoldingHologramWall(false, dir);
                return;
            }
            cell->GetWall(dir)->GetSprite()->SetColor({ 100, 100, 255 });
            cell->GetWall(dir)->GetSprite()->SetAlpha(100);
        }
    }
    else {//structures
        if (cell->HasStructure())//trying to place structure where there already is one
            cell->GetStructure()->GetSprite()->SetColor({ 255, 100, 100 }); //highlight red
        else {//no structure
            cell->SetHoldingHologramStruct(true);
            PlaceStructure(context, true);
            cell->GetStructure()->GetSprite()->SetColor({ 100, 100, 255 });
            cell->GetStructure()->GetSprite()->SetAlpha(100);
        }
    }

    return;
}

//remove hologram/red effect
void WorldScene::RemoveHoverEffect(GridCoord coord, GameContext& context) {
    GridCell* cell = context.grid->GetCell(currentHoveredCellCoords);
    if (!cell) return;
    //walls
    if (currentStructure == 1 || currentStructure == 2) {
        WallDirection dir;
        if (currentStructure == 1)  dir = WallDirection::NORTH;
        else dir = WallDirection::WEST;

        if (cell->HasWall(dir)) {
            cell->GetWall(dir)->GetSprite()->SetColor({ 255, 255, 255 });
        }
        else {
            cell->SetHoldingHologramWall(false, dir);
            cell->RemoveWall(dir);
        }
    }
    else {//structures
        if (cell->HasStructure())//trying to place structure where there already is one
            cell->GetStructure()->GetSprite()->SetColor({ 255, 255, 255 }); //highlight red
        else {//no structure
            cell->SetHoldingHologramStruct(false);
            cell->RemoveStructure();
        }
    }
    return;
}