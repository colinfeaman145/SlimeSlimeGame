#include "FreezeTrap.hpp"
#include "Grid.hpp"

void FreezeTrap::Initialize() {
	int size = context.grid->GetCellSize();

	AnimatedSprite* s = new AnimatedSprite();
	SDL_Texture* tex = context.txm->LoadTexture(context.renderer, "../../assets/traps/ice_trap.png");
	s->Initialize(tex, 123, 123, 0, 0, size, size, 4, 16);
	s->SetFrameDuration(0.07);
	s->SetLeaveOnLastFrame(true);
	s->SetLooping(false);
	s->SetFrame(15);
	sprite = s;

	maxHealth = 120;
	health = maxHealth;
	damage = .65; //per frame
	cooldown = 14;
	activationDelay = 0.28; //4 frames

	Trap::Initialize();
}

void FreezeTrap::ActivateTrapAbility(Enemy* e) {
	//activates every frame while active

	e->Damage(damage);
	e->SetFrozen(damage * 4);
}

void FreezeTrap::HandleCollision(Collidable* other, Vector2 penetration) {
	if (other->GetCollidableType() == CollidableType::ENEMY) {
		if (!activated && currentTimer >= cooldown) {
			Activate();
			context.am->PlaySound("FreezeTrap", "Default", { position.x, 100, position.y }, { 0, 0, 0 }, Vector2(0.85, 1.15));
		}

		if (activated && ((currentTimer - cooldown) > activationDelay))
			ActivateTrapAbility(static_cast<Enemy*>(other));
	}
}