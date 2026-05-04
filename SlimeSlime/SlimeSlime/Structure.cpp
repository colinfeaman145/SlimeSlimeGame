#include "Structure.hpp"
#include "Grid.hpp"

Structure::Structure(){
    sprite = nullptr;
    position = Vector2(-1, -1);
    canCollide = false;
    maxDurability = 100;
    durability = maxDurability;
}

Structure::Structure(const Structure& other){
    position = other.position;
    sprite = other.sprite ? other.sprite->Clone() : nullptr;
    canCollide = other.canCollide;
    collisionBound = other.collisionBound;
    maxDurability = other.maxDurability;
    durability = other.durability;
}

Structure* Structure::Clone() const {
    return new Structure(*this);
}

Structure::~Structure() {
    delete sprite;
    sprite = nullptr;
}

bool Structure::Initialize(Sprite* spr, bool canCollide) {
    sprite = spr->Clone();
    sprite->SetDrawLayer(RenderLayer::STRUCTURES, position.y + sprite->GetHeight());
    Vector2 size = sprite->GetDrawSize();
    collisionBound = CollisionShape::MakeAABB(size.x, size.y);
    SetCanCollide(canCollide);
    return sprite != nullptr;
}

void Structure::Draw(Renderer* renderer) {
    if (sprite) {
        sprite->Draw(renderer);
    }
    if (DEBUGMODE) Collidable::Draw(renderer, {255, 0, 0});
}

void Structure::ChangeSize(int w, int h) {
    sprite->SetDrawSize(w, h);
    collisionBound.box.UpdateSize(w, h);
}

void Structure::Process(float deltaTime, GameContext& context) {
    if (sprite) sprite->Process(deltaTime, context);
}

void Structure::SetPosition(Vector2 pos) {
    position = pos;
    sprite->SetPosition(position);
    sprite->SetDrawLayer(RenderLayer::STRUCTURES, position.y);
}

bool Structure::IsBroken() const {
    return broken;
}

void Structure::SetBroken(bool b) {
    broken = b;
}

void Structure::Damage(int amount) {
    health -= amount;
    if (health <= 0)
        SetBroken(true);
}

void Structure::Heal(int amount) {
    health += amount;
    if (health >= maxHealth)
        health = maxHealth;
}

void Structure::SetDurability(int d) {
    maxDurability = d;
}

int Structure::GetDurability() {
    return durability;
}

ResourceType Structure::GetDropType() const{
    return ResourceType::NONE;
}

int Structure::GetDropAmount() const {
    return 0;
}

void Structure::HandleCollision(Collidable* other, Vector2 penetration, GameContext& context) {
    return;
}