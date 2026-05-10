#include "Explosion.hpp"
#include "AnimatedSprite.hpp"
#include "Structure.hpp"
#include "Enemy.hpp"
#include "Player.hpp"
#include "Nature.hpp";
#include "Grid.hpp"
#include "ExplosionTrap.hpp"

Explosion::Explosion(const Explosion& other) 
	: Explosion(other.radius * 2, other.damage){}

Explosion::Explosion(int size, int dam) {

	AnimatedSprite* s = new AnimatedSprite();
	SDL_Texture* tex = context.txm->LoadTexture(context.renderer, "../../assets/explosion.png");
	s->Initialize(tex, 125, 125, 0, 0, size, size, 4, 16);
	s->SetFrameDuration(0.15);
	s->SetLooping(false);
	s->SetLeaveOnLastFrame(false);
	SetCanCollide(false);
	sprite = s;
	radius = (float)size / 2;
	position = Vector2();
	velocity = Vector2();

	collisionBound = CollisionShape::MakeCircle(radius * 1.2);

	damage = dam;
	health = FLT_MAX;

	canDamage = false;
	activated = false;
	damageDelay = 0.3;//2 frames
	currentTimer = 0;
}

void Explosion::Process(float deltaTime) {
	if (activated) {
		currentTimer += deltaTime;
		if (currentTimer >= damageDelay && !damageDealt) {
			canDamage = true;
		}
	}

	if (canDamage) {
		context.grid->ResolveCollisions(this);
		canDamage = false;
		damageDealt = true;
	}

	AnimatedSprite* a = static_cast<AnimatedSprite*>(sprite);
	if (!a->IsAnimating()) {
		activated = false;
		currentTimer = 0;
	}
	Entity::Process(deltaTime);
}

void Explosion::Explode() {
	activated = true;
	damageDealt = false;
	context.am->PlaySound("Explosion", "Default", { position.x, 100, position.y }, { 0, 0, 0 }, Vector2(0.85, 1.15));
	static_cast<AnimatedSprite*>(sprite)->Restart();
	static_cast<AnimatedSprite*>(sprite)->Animate();
}

//cant damage an explosion
void Explosion::Damage(float amount) {
	return;
}

//center around pos
void Explosion::SetPosition(Vector2 pos) {
	position = Vector2(pos.x - radius, pos.y - radius);
	context.grid->UpdateOccupancy(static_cast<Entity*>(this), &GridCell::AddOther, &GridCell::RemoveOther);
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
		if (other->GetCollidableType() == CollidableType::STRUCTURE) {
			if (dynamic_cast<ExplosionTrap*>(other))//explosion traps immune to explosion
				return;
			else
				static_cast<Structure*>(other)->Damage(damage * GetDamageScaler(other));
		}
		else if (other->GetCollidableType() == CollidableType::NATURE)
			static_cast<Nature*>(other)->Damage(damage * GetDamageScaler(other));
		else if (other->GetCollidableType() == CollidableType::PLAYER)
			static_cast<Player*>(other)->Damage(damage * GetDamageScaler(other));
		else if (other->GetCollidableType() == CollidableType::ENEMY)
			static_cast<Enemy*>(other)->Damage(damage * GetDamageScaler(other));
	}
}