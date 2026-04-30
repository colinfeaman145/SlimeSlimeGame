#include "Structure.hpp"
#include "Grid.hpp"

Structure::Structure(){
    sprite = nullptr;
    position = Vector2(-1, -1);
    canCollide = false;
}

Structure::Structure(const Structure& other){
    position = other.position;
    sprite = other.sprite ? other.sprite->Clone() : nullptr;
    canCollide = other.canCollide;
    collisionBound = other.collisionBound;
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
    sprite->SetDrawLayer(RenderLayer::STRUCTURES);
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
}

void Structure::HandleCollision(Collidable* other, Vector2 penetration, GameContext& context) {
    return;
}