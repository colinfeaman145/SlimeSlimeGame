#include "Trap.hpp"
#include "Grid.hpp"

Trap::Trap(const Trap& other) : Structure(other) {
	activationDelay = other.activationDelay;
	cooldown = other.cooldown;
	currentTimer = other.currentTimer;
	damage = other.damage;
	activated = other.activated;
	abilityActivated = other.abilityActivated;
	activateAbility = other.activateAbility;
	particles = other.particles;
	cooldownBar = new PercentageBar(*other.cooldownBar);
}

void Trap::Initialize() {
	int size = context.grid->GetCellSize();

	healthBar = new PercentageBar(health, maxHealth, size * 0.9, size * 0.1, { 255, 50, 50 }, { 0, 0, 0 });
	healthBar->SetPosition(GetCenter().x, GetCenter().y);
	healthBar->SetOffset(-(size * 0.45), (size * 0.3));

	currentTimer = cooldown;
	cooldownBar = new PercentageBar(cooldown, cooldown, size * 0.8, size * 0.1, { 50, 50, 255 }, { 0, 0, 0 });
	cooldownBar->SetPosition(GetCenter().x, GetCenter().y);
	cooldownBar->SetOffset(size * 0.1, (size * 0.45));

	collisionBound = CollisionShape::MakeAABB(size * 0.8, size * 0.8, Vector2(size * 0.1, size * 0.1));
	SetCanCollide(false);
	collideType = CollidableType::STRUCTURE;
}

void Trap::Draw(Renderer* renderer) {
	if (currentTimer > 0 && currentTimer < cooldown)
		cooldownBar->Draw(renderer);
	Structure::Draw(renderer);
}

void Trap::Process(float deltaTime) {

	if (currentTimer < cooldown) currentTimer += deltaTime;//pause timer when ready
	cooldownBar->SetValues(currentTimer, cooldown);

	if (activated) {
		currentTimer += deltaTime;
		//do we reset?
		if (sprite) {
			if (!static_cast<AnimatedSprite*>(sprite)->IsAnimating()) {
				currentTimer = 0;
				activated = false;
			}
		}
	}

	if (abilityActivated)
		activateAbility = false;

	Structure::Process(deltaTime);
}

void Trap::Activate() {
	activated = true;
	activateAbility = true;
	abilityActivated = false;
	if (sprite) {
		AnimatedSprite* a = static_cast<AnimatedSprite*>(sprite);
		a->Restart();
		a->Animate();
	}
}

void Trap::IncreaseAttack(float amount) {
	damage += amount;
}

void Trap::IncreaseAttackByPercent(float amount) {
	damage += (damage * amount);
}

void Trap::SetPosition(Vector2 pos) {
	Structure::SetPosition(pos);

	cooldownBar->SetPosition(pos.x, pos.y);
}