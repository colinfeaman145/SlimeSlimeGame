#include "Entity.hpp"
#include "Sprite.hpp"
#include "Renderer.hpp"
#include "Grid.hpp"

Entity::Entity() {
    sprite = nullptr;
    radius = 0;
    position = Vector2(0.0f, 0.0f);
    velocity = Vector2(0.0f, 0.0f);
    alive = true;
    occupancy = { -1, -1, -1, -1 };
    canCollide = true;
    SetHealth(100);
}

Entity::~Entity() {
    delete sprite;
    sprite = nullptr;
}

bool Entity::Initialize(Vector2 pos, Vector2 vel, Sprite* spr) {
    position = pos;
    velocity = vel;
    sprite = spr;
    if(sprite){
        radius = spr->GetWidth() / 2;
        sprite->SetDrawLayer(RenderLayer::ENEMIES);
        Vector2 size = sprite->GetDrawSize();
        collisionBound = CollisionShape::MakeCircle(radius);
        SetCanCollide(true);
    }
    else {
        collisionBound = CollisionShape::MakeCircle(1);//ensure collider data is initialized
    }
    return true;
}

void Entity::Process(float deltaTime, GameContext& context) {
    position = position + (velocity * deltaTime);

    if (sprite) {
        sprite->SetPosition((int)position.x, (int)position.y);
        sprite->Process(deltaTime, context);
    }
}

void Entity::Draw(Renderer* renderer){
    if (sprite) {
        sprite->Draw(renderer);
    }
    if (DEBUGMODE) {
        if(canCollide)
            Collidable::Draw(renderer, {255, 0, 0});
        else
            Collidable::Draw(renderer, {0, 255, 0});
    }
}

void Entity::Rotate(float direction) {
    if (sprite) {
        sprite->SetRotation(direction);
    }
}

bool Entity::IsAlive()const {
    return alive;
}

void Entity::SetDead() {
    alive = false;
}

void Entity::Damage(int amount) {
    health -= amount;
    if (health <= 0) SetDead();
}

void Entity::Heal(int amount) {
    health += amount;
    if (health >= maxHealth)
        health = maxHealth;
}

float Entity::GetRadius() {
    return radius;
}

Vector2 Entity::GetFacingDirection() {
    return velocity.Normalized();
}

Vector2 Entity::GetPosition() {
    Vector2 center = position;
    center.x += radius;
    center.y += radius;
    return center;
}

Vector2 Entity::GetCorner() {
    return position;
}

Vector2& Entity::GetVelocity() {
    return velocity;
}

Sprite* Entity::GetSprite() {
    return sprite;
}

GridOccupancy Entity::GetOccupancy() const { 
    return occupancy;
}

float Entity::GetMovementSpeed() {
    return movementSpeed;
}

void Entity::SetOccupancy(GridOccupancy occ) {
    occupancy = occ;
}

void Entity::SetMovementSpeed(float speed) {
    movementSpeed = speed;
}

void Entity::SetPosition(Vector2 pos) {
    position = pos;
    if(sprite) sprite->SetPosition(pos);
}


void Entity::Move(MovementDir m, float deltaTime) {
    switch (m){
        case MovementDir::NORTH:
            position.y -= movementSpeed * deltaTime;
            break;
        case MovementDir::EAST:
            position.x += movementSpeed * deltaTime;
            break;
        case MovementDir::SOUTH:
            position.y += movementSpeed * deltaTime;
            break;
        case MovementDir::WEST:
            position.x -= movementSpeed * deltaTime;
            break;
        default:
            break;
    }
}

void Entity::HandleCollision(Collidable* other, Vector2 penetration, GameContext& context) {
    if (other->CanCollide()) {
        position = GetCorner() + penetration;
    }
}
