#include "Enemy.hpp"
#include "Grid.hpp"
#include "Player.hpp"

void Enemy::Initialize(Vector2 pos, Sprite* spr, float retarget, int targetRad, float atlasTarget, float playerTarget) {
    retargetCooldown = retarget;
    currentRetargetTime = retarget;
    targetRadius = targetRad;
    adjustCourseTimer = 1.5f;
    atlasTargetChance = max(atlasTarget, 0.0f);//must be atleast 0
    playerTargetChance = max(playerTarget, 0.0f);//must be atleast 0
    Entity::Initialize(pos, Vector2(), spr);
    movementSpeed = 35;
    //target and velocity defined in process on first pass
}

void Enemy::Draw(Renderer* renderer) {
    Entity::Draw(renderer);
}

void Enemy::Process(float deltaTime, GameContext& context) {
    currentRetargetTime -= deltaTime;
    adjustCourseTimer -= deltaTime;
    if (currentRetargetTime < 0 || !target) {
        target = FindNewTarget(context);
        currentRetargetTime = retargetCooldown;
    }

    Hone(context); //move towards target
   
    Entity::Process(deltaTime, context);
    context.grid->UpdateEnemyOccupancy(this);
}

ResourceType Enemy::GetDropType() const {
    return ResourceType::COIN;
}

int Enemy::GetDropAmount() const {
    return 10;
}

void Enemy::SetTarget(Collidable* c) {
    target = c;
}

Collidable* Enemy::FindNewTarget(GameContext& context) {

    uniform_real_distribution<float> targetAtlasGen(0, atlasTargetChance);
    uniform_real_distribution<float> targetPlayerGen(0, playerTargetChance);
    if (targetAtlasGen(gen) <= 1) 
        return context.grid->GetAtlas();

    //attack bullshit
    vector<Collidable*> collidables = context.grid->GetNearbyCollidables(lastCell, targetRadius);
    vector<Collidable*> structures;
    for (Collidable* c : collidables) {//filter
        if (dynamic_cast<Player*>(c) && targetPlayerGen(gen) <= 1) {
            return c;
        }
        else if (dynamic_cast<Structure*>(c)) {
            structures.push_back(c);
        }
    }
    
    //if nothing to fuck up, go to where shit is
    if (structures.size() == 0) return context.grid->GetAtlas();

    //attack random structure nearby
    uniform_int_distribution<int> randomStructureGen(0, structures.size() - 1);
    return structures.at(randomStructureGen(gen));
}

void Enemy::Hone(GameContext& context) {
    if (!target) return;

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
    float distToCenter = sqrtf(dx * dx + dy * dy);

    //smaller = closer to center before allowing vector change
    if (distToCenter < cellSize * 0.35f || adjustCourseTimer < 0) {
        printf("ADJUST\n");
        Vector2 flowDir = context.grid->GetFlowVector(myCell, targetCoord);
        if (flowDir.x != 0.f || flowDir.y != 0.f)
            velocity = flowDir * movementSpeed;
        adjustCourseTimer = 5.0f;
    }
    lastCell = myCell;
    return;

    
}


//landing hit on player refreshes cooldown time so that it keeps tracking player. 
//If goes a while without landing hit, automatically will switch target
void Enemy::HandleCollision(Collidable* other, Vector2 penetration, GameContext& context) {
    if (other->CanCollide()) {
        position = GetCorner() + penetration;
    }

    if (dynamic_cast<Structure*>(other)) {
        //printf("Velocity: %f, %f\n", velocity.x, velocity.y);
        //printf("Position: %f, %f\n", position.x, position.y);
        //printf("Last Cell: %d, %d\n", lastCell.col, lastCell.row);
    }
}