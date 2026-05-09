#include "SpikeTrap.hpp"
#include "Grid.hpp"

void SpikeTrap::Initialize() {
	int size = context.grid->GetCellSize();

	AnimatedSprite* s = new AnimatedSprite();
	SDL_Texture* tex = context.txm->LoadTexture(context.renderer, "../../assets/traps/spike_trap.png");
	s->Initialize(tex, 125, 125, 0, 0, size, size, 4, 16);
	s->SetFrameDuration(0.1);
	s->SetLeaveOnLastFrame(true);
	s->SetLooping(false);
	s->SetFrame(15);
	sprite = s;

	maxHealth = 90;
	health = maxHealth;
	damage = 75;
	cooldown = 7;
	activationDelay = 0.4; //4 frames

	Trap::Initialize();
}

void SpikeTrap::ActivateTrapAbility(Enemy* e) {
	if (!activateAbility) return;//only activate ability one frame active

	e->Damage(damage);

	abilityActivated = true;//set so next frames trap doesnt damage
}

void SpikeTrap::HandleCollision(Collidable* other, Vector2 penetration) {
	if (other->GetCollidableType() == CollidableType::ENEMY) {
		if (!activated && currentTimer >= cooldown) {
			Activate();
			context.am->PlaySound("SpikeTrap", "Default", { position.x, 100, position.y }, { 0, 0, 0 }, Vector2(0.85, 1.15));
		}

		if (activated && ((currentTimer - cooldown) > activationDelay)) 
			ActivateTrapAbility(static_cast<Enemy*>(other));
	}
}