#include "Player.hpp"
#include "Resource.hpp"
#include "Grid.hpp"

void Player::Initialize(Vector2 pos, int pullRadius, Vector2 vel, Sprite* spr) {
	Entity::Initialize(pos, vel, spr);
	itemPullRadius = pullRadius;
	itemPickupRadius = GetRadius() * 1.5;
	SetCanCollide(true);

	coins = 0;
	wood = 100;
	stone = 0;


	Vector2 size = sprite->GetDrawSize();
	healthBar = new PercentageBar(health, maxHealth, size.x * 1.1, size.y * 0.2, { 255, 50, 50 }, { 150, 50, 50 });
	healthBar->SetPosition(position.x, position.y);
	healthBar->SetOffset(-(size.x * 0.05), (size.y * 0.7));

	cooldownBar = nullptr;//need attackCone to define
}

void Player::Draw(Renderer* renderer) {
	Entity::Draw(renderer);
	attackCone->Draw(renderer);
	if (cooldownBar && attackCone->GetCurrentAttackCooldownTime() > 0) 
		cooldownBar->Draw(renderer);
}

void Player::Process(float deltaTime, GameContext& context) {
	Entity::Process(deltaTime, context);
	
	//process attackCone & cooldown bar
	if (attackCone) {
		float curr = attackCone->GetCurrentAttackCooldownTime();
		float cooldownTime = attackCone->GetCooldownTime();
		cooldownBar->SetValues(cooldownTime - curr, cooldownTime);
		cooldownBar->SetPosition(position.x, position.y);

		attackCone->ProcessDropQueue(itemPullRadius, context);
		attackCone->SetPosition(GetPosition());//follow player
		attackCone->SetTargetPosition(context.im->GetMouseWorldPosition(context.renderer->cam));//cone points to mouse
		attackCone->Process(deltaTime, context);
	}
}


void Player::SetAttackCone(AttackCone* ac) {
	attackCone = ac;

	//define cooldown bar
	Vector2 size = sprite->GetDrawSize();
	float curr = attackCone->GetCurrentAttackCooldownTime();
	float cooldownTime = attackCone->GetCooldownTime();
	cooldownBar = new PercentageBar(cooldownTime - curr, cooldownTime, size.x * 1.1, size.y * 0.2, { 50, 50, 255 }, { 50, 50, 150 });
	cooldownBar->SetPosition(position.x, position.y);
	cooldownBar->SetOffset(-(size.x * 0.05), (size.y * 0.95));
}

AttackCone* Player::GetAttackCone() {
	return attackCone;
}

void Player::AddCoins(int amount) {
	coins += amount;
}

void Player::RemoveCoins(int amount) {
	coins -= amount;
}

bool Player::HasEnoughCoins(int amount) {
	return coins >= amount;
}

void Player::AddWood(int amount) {
	wood += amount;
}

bool Player::HasEnoughWood(int amount) {
	return wood >= amount;
}

void Player::RemoveWood(int amount) {
	wood -= amount;
}

void Player::AddStone(int amount) {
	stone += amount;
}

void Player::RemoveStone(int amount) {
	stone -= amount;
}

bool Player::HasEnoughStone(int amount) {
	return stone >= amount;
}

void Player::HandleCollision(Collidable* other, Vector2 penetration, GameContext& context) {
	if (Resource* r = dynamic_cast<Resource*>(other)) {//if its a resource
		//calculate resource distance
		Vector2 rPos = r->GetPosition();
		float dist = Distance(rPos, GetPosition());
		//if close enough delete and add to supply
		if (dist <= itemPickupRadius) {
			HandleResourcePickup(r, context);
		}
	}
	if (other->CanCollide()) {
		position = GetCorner() + penetration;
	}
}

void Player::HandleResourcePickup(Resource* r, GameContext& context) {
	switch (r->GetResourceType()) {
		case(ResourceType::COIN):
			coins++;
			break;
		case(ResourceType::GOLD):
			coins += 10;
			break;
		case(ResourceType::WOOD):
			wood++;
			break;
		case(ResourceType::STONE):
			stone++;
			break;
	}
	r->SetDead();
}