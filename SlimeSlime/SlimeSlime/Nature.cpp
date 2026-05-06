#include "Nature.hpp"

void Nature::Initialize(Sprite* spr, NatureType t, bool canCollide) {
	Entity::Initialize(Vector2(), Vector2(), spr);//real position set in gridCell PlaceNature
	SetCanCollide(canCollide);

	sprite->SetDrawLayer(RenderLayer::NATURE, Entity::position.y);
	type = t;
	//health set in nature.hpp
}

void Nature::Draw(Renderer* renderer) {
	Entity::Draw(renderer);
}

void Nature::Process(float deltaTime, GameContext& context) {
	Entity::Process(deltaTime, context);
}

void Nature::Damage(int amount) {
	health -= amount;
	SetFlash(true);
	healthBar->SetValues(health, maxHealth);
	//Break if called in AttackCone CollisionHandler
}

void Nature::Break() {
	SetDead();
}

ResourceType Nature::GetDropType() const {

	switch (type) {
		case(NatureType::TREE):
			return ResourceType::WOOD;
		case(NatureType::BUSH):
			return ResourceType::WOOD;
		case(NatureType::STUMP):
			return ResourceType::WOOD;
		case(NatureType::ROCK):
			return ResourceType::STONE;
		case(NatureType::FOLIAGE):
			return ResourceType::NONE;
	}
}

int Nature::GetDropAmount() const {
	return (((double)maxHealth / 10) * dropVariance(gen));
}

void Nature::SetPosition(Vector2 pos) {
	Entity::SetPosition(pos);
	Entity::sprite->SetDrawLayer(RenderLayer::NATURE, position.y + Entity::sprite->GetHeight());
}

Vector2 Nature::GetPosition() const {
	return position;
}

void Nature::HandleCollision(Collidable* other, Vector2 penetration, GameContext& context) {
	if (Nature* n = dynamic_cast<Nature*>(other)) {//if its another nature
		SetPosition(position - penetration);
	}
}
