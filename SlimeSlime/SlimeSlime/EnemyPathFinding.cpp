#include "Enemy.hpp"
#include "Grid.hpp"
#include "Player.hpp"

void Enemy::SetTarget(Collidable* c) {
    if (target) {
        auto& vec = target->targetedBy;
        auto it = std::find(vec.begin(), vec.end(), this);
        if (it != vec.end()) vec.erase(it);
    }
    target = c;
    if (target)
        target->targetedBy.push_back(this);
}

Collidable* Enemy::FindNewTarget() {

    uniform_real_distribution<float> targetAtlasGen(0, atlasTargetChance);
    uniform_real_distribution<float> targetPlayerGen(0, playerTargetChance);
    if (targetAtlasGen(gen) <= 1)
        return context.grid->GetAtlas();

    //attack bullshit
    vector<Structure*> structures = context.grid->GetNearbyStructures(lastCell, targetRadius);
    Player* player = context.grid->GetPlayer();
    if (player && targetPlayerGen(gen) <= 1)
        return player;

    //if nothing to fuck up, go to where shit is
    if (structures.size() == 0) return context.grid->GetAtlas();

    //attack random structure nearby
    uniform_int_distribution<int> randomStructureGen(0, structures.size() - 1);
    return structures.at(randomStructureGen(gen));
}

void Enemy::Hone() {

    if (currentRetargetTime < 0) {
        SetTarget(FindNewTarget());
        currentRetargetTime = retargetCooldown;
    }
    if (target == nullptr) return;

    GridCoord myCell = context.grid->WorldToGrid(GetPosition());
    GridCoord targetCoord = context.grid->WorldToGrid(target->GetPosition());

    if (myCell.col == targetCoord.col && myCell.row == targetCoord.row) {
        Vector2 toTarget = target->GetCenter() - GetCenter();
        if (toTarget.Length() > max(sprite->GetWidth(), sprite->GetHeight()))
            velocity = toTarget.Normalized() * movementSpeed;
        return;
    }

    //don't apply its vector until we're near the center 
    int cellSize = context.grid->GetCellSize();
    Vector2 cellWorld = context.grid->GridToWorld(myCell);
    Vector2 cellCenter = { cellWorld.x + cellSize * 0.5f,
                            cellWorld.y + cellSize * 0.5f };

    float dx = GetCenter().x - cellCenter.x;
    float dy = GetCenter().y - cellCenter.y;
    float threshold = cellSize * 0.35f;

    //smaller = closer to center before allowing vector change
    if ((dx * dx + dy * dy) < (threshold * threshold) || adjustCourseTimer < 0) {
        Vector2 flowDir = context.grid->GetFlowVector(myCell, targetCoord);
        if (flowDir.x != 0.f || flowDir.y != 0.f)
            velocity = flowDir * movementSpeed;
        adjustCourseTimer = 5.0f;
    }
    lastCell = myCell;
    return;

}