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

        collisionBound = CollisionShape::MakeCircle(radius);
        SetCanCollide(true);

        Vector2 size = sprite->GetDrawSize();
        healthBar = new PercentageBar(health, maxHealth, size.x * 0.9, size.y * 0.1, {255, 50, 50}, {0, 0, 0});
        healthBar->SetPosition(position.x, position.y);
        healthBar->SetOffset((size.x * 0.05), (size.y * 0.8));
    }
    else {
        collisionBound = CollisionShape::MakeCircle(1);//ensure collider data is initialized
        healthBar = nullptr;
    }
    return true;
}

void Entity::Process(float deltaTime) {
    position = position + (velocity * deltaTime);

    if (sprite) {
        sprite->SetPosition((int)position.x, (int)position.y);
        sprite->Process(deltaTime);

    }
    if(healthBar) healthBar->SetPosition(position.x, position.y);

    if (flashDuration > 0) flashDuration -= deltaTime;
    else SetFlash(false);
}

void Entity::Draw(Renderer* renderer){
    if (sprite) {
        sprite->Draw(renderer);
    }
    if (healthBar && health > 0 && health < maxHealth) {
        healthBar->Draw(renderer);
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

bool Entity::IsAlive() const {
    return alive;
}

void Entity::SetDead() {
    alive = false;
}

void Entity::Damage(float amount) {
    health -= amount;
    if (health <= 0) SetDead();
    SetFlash(true);
    if(healthBar) healthBar->SetValues(health, maxHealth);
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

Vector2 Entity::GetPosition() const {
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
    if(healthBar) healthBar->SetPosition(position.x, position.y);
}

void Entity::SetVelocity(Vector2 vel) {
    velocity = vel;
}

void Entity::SetHealthBar(PercentageBar* bar) {
    healthBar = bar;
}

PercentageBar* Entity::GetHealthBar() {
    return healthBar;
}

void Entity::SetFlash(bool flash) {
    if (!sprite) return;
    sprite->SetIsFlashing(flash);
    if (flash) flashDuration = 0.5f;
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
    moving = true;
}

void Entity::HandleCollision(Collidable* other, Vector2 penetration) {
    if (other->GetCollidableType() == CollidableType::ENEMY) {
        return;//entitys dont collide with each other
    }
    if (other->CanCollide()) {
        position = GetCorner() + penetration;
    }
}
