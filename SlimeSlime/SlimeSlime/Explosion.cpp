#include "Explosion.hpp"
#include "AnimatedSprite.hpp"
#include "Structure.hpp"
#include "Enemy.hpp"
#include "Player.hpp"
#include "Nature.hpp";

Explosion::Explosion(int size, int dam) {

	AnimatedSprite* s = new AnimatedSprite();
	SDL_Texture* tex = context.txm->LoadTexture(context.renderer, "../../assets/explosion.png");
	s->Initialize(tex, 125, 125, 0, 0, size, size, 4, 16);
	s->SetFrameDuration(0.15);
	sprite = s;
	radius = (float)size / 2;

	collisionBound = CollisionShape::MakeCircle(size * 1.4);

	damage = dam;
}

void Explosion::Process(float deltaTime) {
	if (activated) {
		currentTimer += deltaTime;
		if (currentTimer >= damageDelay) {
			canDamage = true;
		}
	}

	if (!static_cast<AnimatedSprite*>(sprite)->IsAnimating()) {
		activated = false;
		canDamage = false;
		currentTimer = 0;
	}
}

void Explosion::Explode() {
	activated = true;
	context.am->PlaySound("Explosion", "Default", { position.x, 100, position.y }, { 0, 0, 0 }, Vector2(0.85, 1.15));
	static_cast<AnimatedSprite*>(sprite)->Restart();
	static_cast<AnimatedSprite*>(sprite)->Animate();
}

//cant damage an explosion
void Explosion::Damage(float amount) {
	return;
}

bool Explosion::isActive() {
	return activated;
}

float Explosion::GetDamageScaler(Collidable* c) {
	float dist = Distance(position, c->GetPosition());
	return (dist / radius) * 1.25;
}

void Explosion::IncreaseDamage(float amount) {
	damage += amount;
}

void Explosion::HandleCollision(Collidable* other, Vector2 penetration) {
	if (canDamage) {
		if (other->GetCollidableType() == CollidableType::STRUCTURE)
			static_cast<Structure*>(other)->Damage(damage * GetDamageScaler(other));
		else if (other->GetCollidableType() == CollidableType::NATURE)
			static_cast<Nature*>(other)->Damage(damage * GetDamageScaler(other));
		else if (other->GetCollidableType() == CollidableType::PLAYER)
			static_cast<Player*>(other)->Damage(damage * GetDamageScaler(other));
		else if (other->GetCollidableType() == CollidableType::ENEMY)
			static_cast<Enemy*>(other)->Damage(damage * GetDamageScaler(other));
	}
}