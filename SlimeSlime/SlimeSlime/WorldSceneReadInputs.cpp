#include "WorldScene.hpp"

void WorldScene::ReadInputs(float deltaTime) {
    if (context.im->IsKeyDown("move_left")) {
        MovePlayer(MovementDir::WEST, deltaTime);
    }
    if (context.im->IsKeyDown("move_right")) {
        MovePlayer(MovementDir::EAST, deltaTime);
    }
    if (context.im->IsKeyDown("move_up")) {
        MovePlayer(MovementDir::NORTH, deltaTime);
    }
    if (context.im->IsKeyDown("move_down")) {
        MovePlayer(MovementDir::SOUTH, deltaTime);
    }
    if (context.im->IsKeyPressed("build_mode")) {
        ToggleBuildMode();
    }
    if (context.im->IsKeyPressed("change_material")) {
        ChangeMaterial();
    }
    if (context.im->IsKeyPressed("wall")) {
        if (GetCurrentStructureNumber() == 1)
            ChangeStructure(2);
        else
            ChangeStructure(1);
    }
    if (context.im->IsKeyPressed("barbed_wire")) {
        ChangeStructure(3);
    }
    if (context.im->IsKeyPressed("push_trap")) {
        if (GetCurrentStructureNumber() == 4) {
            pushTrapDirection++;
            if (pushTrapDirection >= 4)
                pushTrapDirection = 0;
        }
        ChangeStructure(4);
    }
    if (context.im->IsKeyPressed("spike_trap")) {
        ChangeStructure(5);
    }
    if (context.im->IsKeyPressed("fire_trap")) {
        ChangeStructure(6);
    }
    if (context.im->IsKeyPressed("ice_trap")) {
        ChangeStructure(7);
    }
    if (context.im->IsKeyPressed("explosion_trap")) {
        ChangeStructure(8);
    }
    if (context.im->IsMouseButtonPressed(1)) {
        LeftMouseClick();
    }
    if (context.im->IsMouseButtonPressed(3)) {
        RemoveStructure();
    }

    //update Camera zoom
    context.renderer->cam->AdjustZoom(context.im->GetScrollDelta() * 0.1);
}

void WorldScene::MovePlayer(MovementDir dir, float deltaTime) {
    player->Move(dir, deltaTime);
}

void WorldScene::LeftMouseClick() {
    if (buildMode)
        PlaceStructure();
    else
        Attack();
}

void WorldScene::ChangeMaterial() {
    isStone = !isStone;
}

void WorldScene::ToggleBuildMode() {
    buildMode = !buildMode;

    //play sound
    context.am->PlaySound("BuildMode", "Default", { player->GetPosition().x, 100, player->GetPosition().y }, { 0, 0, 0 }, Vector2(0.85, 1.15));
}

void WorldScene::ChangeStructure(int s) {
    if (s == currentStructure) return;
    if (buildMode)
        RemoveHoverEffect(currentHoveredCellCoords); // clean up old hologram if changed while in build mode
    currentStructure = s;
}