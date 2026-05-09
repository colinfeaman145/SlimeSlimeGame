#include "PushTrap.hpp"
#include "Grid.hpp"

void PushTrap::Initialize(int dir) {
	int size = context.grid->GetCellSize();

	AnimatedSprite* s = new AnimatedSprite();
	SDL_Texture* tex = context.txm->LoadTexture(context.renderer, "../../assets/traps/push_trap.png");
	s->Initialize(tex, 256, 256, 0, 0, size, size, 4, 16);
	s->SetFrameDuration(0.05);
	s->SetLeaveOnLastFrame(true);
	s->SetLooping(false);
	s->Animate();
	sprite = s;

	SetDirection(dir);

	maxHealth = 100;
	health = maxHealth;
	damage = 2;
	cooldown = 10;
	activationDelay = 0.2; //2 frames

	Trap::Initialize();
}

void PushTrap::SetDirection(int dir) {
	switch (dir) {
	case(0):
		direction = PushTrapDirection::NORTH;
		sprite->SetRotation(90);
		break;
	case(1):
		direction = PushTrapDirection::EAST;
		sprite->SetRotation(0);
		break;
	case(2):
		direction = PushTrapDirection::SOUTH;
		sprite->SetRotation(-90);
		break;
	case(3):
		direction = PushTrapDirection::WEST;
		sprite->SetRotation(-180);
		break;
	}
}

void PushTrap::ActivateTrapAbility(Enemy* e) {
	if (!activateAbility) return;//only activate ability one frame active

	e->SetFrozen(damage);
	abilityActivated = true;//set so next frames trap doesnt damage

	int pushScale = 100;
	switch (direction) {
		case(PushTrapDirection::NORTH):
			e->SetVelocity(Vector2(0, damage * pushScale));
			break;
		case(PushTrapDirection::EAST):
			e->SetVelocity(Vector2(damage * pushScale, 0));
			break;
		case(PushTrapDirection::SOUTH):
			e->SetVelocity(Vector2(0, -damage * pushScale));
			break;
		case(PushTrapDirection::WEST):
			e->SetVelocity(Vector2(-damage * pushScale, 0));
			break;
	}

}

void PushTrap::HandleCollision(Collidable* other, Vector2 penetration) {
	if (other->GetCollidableType() == CollidableType::ENEMY) {
		if (!activated && currentTimer >= cooldown) {
			Activate();
			context.am->PlaySound("PushTrap", "Default", { position.x, 100, position.y }, { 0, 0, 0 }, Vector2(0.85, 1.15));
		}

		if (activated && ((currentTimer - cooldown) > activationDelay))
			ActivateTrapAbility(static_cast<Enemy*>(other));
	}
}