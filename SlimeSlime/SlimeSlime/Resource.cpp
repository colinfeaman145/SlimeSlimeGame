#include "Resource.hpp"
#include "Player.hpp"
#include "Grid.hpp"

void Resource::Initialize(Vector2 pos, Vector2 vel, Sprite* spr, float ft, ResourceType t) {
	Entity::Initialize(pos, vel, spr);
	fallTime = ft;
	SetCanCollide(false);
}

void Resource::Process(float deltaTime, GameContext& context) {
	Entity::Process(deltaTime, context);

	fallTime -= deltaTime;
	if (fallTime > 0) {
		position += Vector2(0, context.grid->GetCellSize() * 0.05);
	}
}

void Resource::HandleCollision(Collidable* other, Vector2 penetration, GameContext& context) {
	if (Player* n = dynamic_cast<Player*>(other)) {//if colliding with player
		//pull towards player
		position.x -= penetration.x;
		position.y -= penetration.y;
	}
}