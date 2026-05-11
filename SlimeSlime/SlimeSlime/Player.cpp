#include "Player.hpp"
#include "Resource.hpp"
#include "Grid.hpp"

void Player::Initialize(Vector2 pos, int pullRadius, Vector2 vel, AnimatedSprite* spr) {
	Entity::Initialize(pos, vel, spr);
	itemPickupRadius = GetRadius() * 1.5;
	context.grid->ChangeItemPickupRadius(pullRadius);
	SetCanCollide(true);
	collideType = CollidableType::PLAYER;

	coins = 0;
	wood = 100;
	stone = 0;
	if (GOD_MODE) {
		coins = 10000000;
		wood = 10000;
		stone = 10000;
	}

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

void Player::Process(float deltaTime) {
	Entity::Process(deltaTime);
	context.am->Process(position, deltaTime);//listen for audio

	//update sprite
	if (sprite) {
		AnimatedSprite* s = static_cast<AnimatedSprite*>(sprite);
		if (moving) {
			if(!s->IsAnimating()) s->Animate();
			moving = false;
		}
		else {
			s->Pause();
		}
	}
	
	//process attackCone & cooldown bar
	if (attackCone) {
		float curr = attackCone->GetCurrentAttackCooldownTime();
		float cooldownTime = attackCone->GetCooldownTime();
		cooldownBar->SetValues(cooldownTime - curr, cooldownTime);
		cooldownBar->SetPosition(position.x, position.y);

		attackCone->SetPosition(GetPosition());//follow player
		attackCone->SetTargetPosition(context.im->GetMouseWorldPosition(context.renderer->cam));//cone points to mouse
		attackCone->Process(deltaTime);

		//set character to rotate with attack cone
		sprite->SetRotation(attackCone->GetSprite()->GetRotation());
	}
}


void Player::SetAttackCone(AttackCone* ac) {
	attackCone = ac;

	//define cooldown bar
	Vector2 size = sprite->GetDrawSize();
	float curr = attackCone->GetCurrentAttackCooldownTime();
	float cooldownTime = attackCone->GetCooldownTime();
	cooldownBar = new PercentageBar(cooldownTime - curr, cooldownTime, size.x * 1.1, size.y * 0.2, { 50, 50, 255 }, { 0, 0, 0 });
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

void Player::IncreaseMaxHealth(int amount) {
	float healthPercent = health / maxHealth;
	maxHealth += amount;
	health = maxHealth * healthPercent;
}

bool Player::CanMakeRecipe(unordered_map<ResourceType, int> recipe) {
	bool canMake = true;
	for (auto r : recipe) {
		switch (r.first) {
			case(ResourceType::WOOD):
				canMake &= HasEnoughWood(r.second);
				break;
			case(ResourceType::STONE):
				canMake &= HasEnoughStone(r.second);
				break;
			case(ResourceType::COIN):
				canMake &= HasEnoughCoins(r.second);
				break;
		}
	}
	return canMake;
}

void Player::RemoveRecipeCost(unordered_map<ResourceType, int> recipe) {
	if (!CanMakeRecipe(recipe)) return;
	for (auto r : recipe) {
		switch (r.first) {
			case(ResourceType::WOOD):
				RemoveWood(r.second);
				break;
			case(ResourceType::STONE):
				RemoveStone(r.second);
				break;
			case(ResourceType::COIN):
				RemoveCoins(r.second);
				break;
		}
	}
}

void Player::HandleCollision(Collidable* other, Vector2 penetration) {
	if (other->GetCollidableType() == CollidableType::RESOURCE) {//if its a resource
		Resource* r = static_cast<Resource*>(other);
		//calculate resource distance
		Vector2 rPos = r->GetPosition();
		float dist = Distance(rPos, GetPosition());
		//if close enough delete and add to supply
		if (dist <= itemPickupRadius) {
			HandleResourcePickup(r);
		}
	}
	if (other->CanCollide()) {
		position = GetCorner() + penetration;
	}
}

void Player::HandleResourcePickup(Resource* r) {
	context.am->PlaySound("Pickup", "Default", { position.x, 100, position.y }, { 0, 0, 0 }, Vector2(0.85, 1.15));
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