#include "FireTrap.hpp"
#include "Grid.hpp"

void FireTrap::Initialize() {
	int size = context.grid->GetCellSize();

	//recipe
	recipe.clear();
	recipe.insert({ ResourceType::WOOD, 10 });
	recipe.insert({ ResourceType::STONE, 10 });
	recipe.insert({ ResourceType::COIN, 12 });

	AnimatedSprite* s = new AnimatedSprite();
	SDL_Texture* tex = context.txm->LoadTexture(context.renderer, "../../assets/traps/fire_trap.png");
	s->Initialize(tex, 240, 240, 0, 0, size, size, 4, 16);
	s->SetFrameDuration(0.07);
	s->SetLeaveOnLastFrame(true);
	s->SetLooping(false);
	s->SetFrame(15);
	sprite = s;

	maxHealth = 130;
	health = maxHealth;
	damage = 0.7; //per frame
	cooldown = 10;
	activationDelay = 0.21; //3 frames

	Trap::Initialize();
}

void FireTrap::ActivateTrapAbility(Enemy* e) {
	//activates every frame while active

	e->Damage(damage);
}

void FireTrap::HandleCollision(Collidable* other, Vector2 penetration) {
	if (other->GetCollidableType() == CollidableType::ENEMY) {
		if (!activated && currentTimer >= cooldown) {
			Activate();
			context.am->PlaySound("FireTrap", "Default", { position.x, 100, position.y }, { 0, 0, 0 }, Vector2(0.85, 1.15));
		}

		if (activated && ((currentTimer - cooldown) > activationDelay))
			ActivateTrapAbility(static_cast<Enemy*>(other));
	}
}