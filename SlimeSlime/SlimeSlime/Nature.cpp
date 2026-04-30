#include "Nature.hpp"

void Nature::Initialize(Sprite* spr, NatureType t, bool canCollide) {
	Structure::Initialize(spr, canCollide);
	Structure::sprite->SetDrawLayer(RenderLayer::NATURE, Structure::position.y);
	type = t;
}

void Nature::Draw(Renderer* renderer) {
	Structure::Draw(renderer);
}

void Nature::Process(float deltaTime, GameContext& context) {
	Structure::Process(deltaTime, context);
}

void Nature::HandleCollision(Collidable* other, Vector2 penetration, GameContext& context) {
	if (Nature* n = dynamic_cast<Nature*>(other)) {//if its another nature
		SetPosition(Structure::GetPosition() - penetration);
	}
}

void Nature::SetPosition(Vector2 pos) {
	position = pos;
	Structure::SetPosition(pos);
	Entity::SetPosition(pos);
	Structure::sprite->SetDrawLayer(RenderLayer::NATURE, position.y + Structure::sprite->GetHeight());
}

Vector2 Nature::GetPosition() {
	return Structure::GetPosition();
}