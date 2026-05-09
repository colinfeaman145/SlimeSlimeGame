#include "ExplosionTrap.hpp"
#include "Grid.hpp"

void ExplosionTrap::Initialize() {
	int size = context.grid->GetCellSize();

	AnimatedSprite* s = new AnimatedSprite();
	SDL_Texture* tex = context.txm->LoadTexture(context.renderer, "../../assets/traps/explosion_trap.png");
	s->Initialize(tex, 179, 176, 0, 0, size, size, 4, 16);
	s->SetFrameDuration(0.2);
	s->SetLeaveOnLastFrame(true);
	s->SetLooping(false);
	s->Animate();
	sprite = s;

	maxHealth = 300;
	health = maxHealth;
	damage = 300;
	cooldown = 25;
	activationDelay = 2.0; //roughly 11 frames

	explosion = new Explosion(size * 3, damage);

	Trap::Initialize();
}
void ExplosionTrap::Draw(Renderer* renderer) {
	Trap::Draw(renderer);
	explosion->Draw(renderer);
}

void ExplosionTrap::Process(float deltaTime) {
	Trap::Process(deltaTime);
	explosion->Process(deltaTime);
}

void ExplosionTrap::IncreaseAttack(float amount) {
	Trap::IncreaseAttack(amount);
	explosion->IncreaseDamage(amount);
}

void ExplosionTrap::IncreaseAttackByPercent(float amount) {
	damage += (damage * amount);
	explosion->IncreaseDamage(damage * amount);
}

void ExplosionTrap::ActivateTrapAbility(Enemy* e) {
	if (abilityActivated) return;//only activate once per cooldown

	explosion->Explode();
	abilityActivated = true; //if another enemy enters area, set so only called once
}

void ExplosionTrap::HandleCollision(Collidable* other, Vector2 penetration) {
	if (other->GetCollidableType() == CollidableType::ENEMY) {
		if (!activated && currentTimer >= cooldown) {
			Activate();
			context.am->PlaySound("ExplosionPrepare", "Default", { position.x, 100, position.y }, { 0, 0, 0 }, Vector2(0.85, 1.15));
		}

		if (activated && ((currentTimer - cooldown) > activationDelay))
			ActivateTrapAbility(static_cast<Enemy*>(other));
	}
}