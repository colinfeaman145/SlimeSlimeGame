#include "Resource.hpp"
#include "Player.hpp"
#include "Grid.hpp"

void Resource::Initialize(Vector2 pos, Vector2 vel, Sprite* spr, float ft, ResourceType t) {
	Entity::Initialize(pos, vel, spr);
	fallTime = ft;
	type = t;
	SetCanCollide(false);
	collideType = CollidableType::RESOURCE;
}

void Resource::Process(float deltaTime) {
	Entity::Process(deltaTime);

	fallTime -= deltaTime;
	if (fallTime > 0) {
		position += Vector2(0, context.grid->GetCellSize() * 0.05);
	}
}

void Resource::HandleCollision(Collidable* other, Vector2 penetration) {
	if (other->GetCollidableType() == CollidableType::PLAYER) {//if colliding with player
		//pull towards player
		position.x -= penetration.x;
		position.y -= penetration.y;
	}
}