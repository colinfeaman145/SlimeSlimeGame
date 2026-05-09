#include "Structure.hpp"
#include "Grid.hpp"

Structure::Structure(){
    sprite = nullptr;
    position = Vector2(-1, -1);
    canCollide = false;
    maxHealth = 100;
    health = maxHealth;
    recipe.insert({ ResourceType::WOOD, 3 });
}

Structure::Structure(const Structure& other){
    position = other.position;
    sprite = other.sprite ? other.sprite->Clone() : nullptr;
    canCollide = other.canCollide;
    collisionBound = other.collisionBound;
    maxHealth = other.maxHealth;
    health = other.health;
    healthBar = other.healthBar;
    recipe = other.recipe;
    collideType = other.collideType;
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
    collideType = CollidableType::STRUCTURE;

    healthBar = new PercentageBar(health, maxHealth, size.x * 0.9, size.y * 0.1, { 255, 50, 50 }, { 0, 0, 0 });
    healthBar->SetPosition(GetCenter().x, GetCenter().y);
    int offset = size.x > size.y ? size.x : size.y;
    healthBar->SetOffset(-(offset * 0.45), 0);

    return sprite != nullptr;
}

void Structure::Draw(Renderer* renderer) {
    if (sprite) {
        sprite->Draw(renderer);
    }
    if (healthBar && health > 0 && health < maxHealth) {
        healthBar->Draw(renderer);
    }
    if (DEBUGMODE) Collidable::Draw(renderer, {255, 0, 0});
}

void Structure::ChangeSize(int w, int h) {
    sprite->SetDrawSize(w, h);
    collisionBound.box.UpdateSize(w, h);

    int width = w < h ? h * 0.8 : w * 0.8;
    int height = w < h ? w * 0.25 : h * 0.25;
    healthBar = new PercentageBar(health, maxHealth, width, height, { 255, 50, 255 }, { 0, 0, 0 });
    healthBar->SetPosition(GetCenter().x, GetCenter().y);
    Vector2 size = sprite->GetDrawSize();
    int offset = size.x > size.y ? size.x : size.y;
    healthBar->SetOffset(-(offset * 0.45), 0);
}

void Structure::Process(float deltaTime) {
    if (sprite) sprite->Process(deltaTime);

    if (broken) {
        context.am->PlaySound("BreakStructure", "Default", { position.x, 100, position.y }, { 0, 0, 0 }, Vector2(0.85, 1.15));
        context.grid->RemoveStructure(this);
        //call deletes reference
    }
}

void Structure::SetPosition(Vector2 pos) {
    position = pos;
    sprite->SetPosition(position);
    healthBar->SetPosition(GetCenter().x, GetCenter().y);
    sprite->SetDrawLayer(RenderLayer::STRUCTURES, position.y);
}

bool Structure::IsBroken() const {
    return broken;
}

void Structure::SetBroken(bool b) {
    broken = b;
}

void Structure::Damage(float amount) {
    health -= amount;
    if (health <= 0)
        SetBroken(true);
    if (healthBar) healthBar->SetValues(health, maxHealth);
}

void Structure::Heal(int amount) {
    health += amount;
    if (health >= maxHealth)
        health = maxHealth;
}

void Structure::SetDurability(int d) {
    health = (health / maxHealth) * d;//maintain current percent of health
    maxHealth = d;
    healthBar->SetValues(health, maxHealth);
    if (d > 100) {
        healthBar->SetFillColor({ 200, 200, 0 });
    }
}

void Structure::SetRecipe(unordered_map<ResourceType, int> r) {
    recipe = r;
}

int Structure::GetDurability() {
    return health;
}

ResourceType Structure::GetDropType() const{
    return ResourceType::NONE;
}

int Structure::GetDropAmount() const {
    return 0;
}

void Structure::HandleCollision(Collidable* other, Vector2 penetration) {
    return;
}