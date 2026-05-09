#include "AttackCone.hpp"
#include "Nature.hpp"
#include "Enemy.hpp"

AttackCone::AttackCone(int damage, int r, float ha, float ac) {
	attackDamage = damage;
	radius = r;
	halfAngle = ha;
	attackCooldown = ac;
	currentAttackTime = 0;
}

bool AttackCone::Initialize(Vector2 pos, AnimatedSprite* spr) {
	sprite = spr;
	position = pos;

	CollisionShape cs = CollisionShape::MakeCone(radius, Vector2(1.0, 0.0), halfAngle);
	SetCollisionBound(cs);
	canCollide = false;
	collideType = CollidableType::ATTACK_CONE;

	swoosh = spr;
	swoosh->SetPosition(pos);
	swoosh->SetDrawLayer(RenderLayer::PARTICLE);
	int arcWidth = (int)(2 * radius * sin(halfAngle));
	swoosh->SetDrawSize(arcWidth, arcWidth);
	calledAttack = false;

	return true;
}

void AttackCone::Draw(Renderer* renderer) {
	swoosh->Draw(renderer);
	Collidable::Draw(renderer, {100, 100, 100}, 100, RenderLayer::ATTACK_CONE);
}

void AttackCone::Process(float deltaTime) {
	if (currentAttackTime > 0)
		currentAttackTime -= deltaTime;
	swoosh->Process(deltaTime);
}

void AttackCone::SetTargetPosition(Vector2 target) {
    Vector2 dir = target - position;
    float angle = atan2(dir.y, dir.x);
    collisionBound.SetConeRotation(angle);

	//place swoosh
	float offsetDist = radius - swoosh->GetDrawSize().y * 0.25;//.5 = 50% distance of drawSize away from user(lower = farther)
	Vector2 offset = Vector2(cos(angle) * offsetDist, sin(angle) * offsetDist);

    Vector2 halfSize = swoosh->GetDrawSize() * 0.5;
    swoosh->SetPosition(position + offset - halfSize);
    swoosh->SetRotation(angle * (180.0 / PI) + 75);
}

bool AttackCone::CanAttack() {
	if (currentAttackTime > 0) return false;
	if (swoosh->IsAnimating()) return false;
	return true;
}

void AttackCone::PlayAttack() {
	if (!CanAttack()) return;
	currentAttackTime = attackCooldown;
	calledAttack = true;
	context.am->PlaySound("SwordSwing", "Default", { position.x, 100, position.y }, { 0, 0, 0 }, Vector2(0.85, 1.15));
	swoosh->Restart();
	swoosh->Animate();
}

void AttackCone::ClearAttack() {
	calledAttack = false;
}

void AttackCone::IncreaseAttackDamage(int damage) {
	attackDamage += damage;
}

void AttackCone::IncreaseRadius(int increaseAmount) {
	radius += increaseAmount;
}

void AttackCone::IncreaseWidth(float increaseAmount) {
	halfAngle += increaseAmount;
	int arcWidth = (int)(2 * radius * sin(halfAngle));
	swoosh->SetDrawSize(arcWidth, swoosh->GetHeight());
}

float AttackCone::CalculateAttackDamage(float dist) {
	float damageScaler = (dist / radius) * 1.5; //do up to 1.5 more damage depending on distance
	float damage = attackDamage * damageScaler;
	return min(damage, attackDamage * 0.5f); //do no less than half of attack
}

void AttackCone::HandleCollision(Collidable* other, Vector2 penetration) {

	if(calledAttack){//only deal damage when cone calls resolveCollision, not when Enemy does
		if (other->GetCollidableType() == CollidableType::ENEMY) {//if its an enemy
			Enemy* e = static_cast<Enemy*>(other);
			if (DEBUGMODE) printf("Damage %d, Health %d / %d\n", attackDamage, e->GetHealth(), e->GetMaxHealth());

			if (!e->IsAlive()) return;//dont allow hit when dead
			float dist = Distance(e->GetPosition(), position);
			e->Damage(CalculateAttackDamage(dist));
		}

		if (other->GetCollidableType() == CollidableType::NATURE) {//if its nature
			Nature* n = static_cast<Nature*>(other);
			//if (DEBUGMODE) printf("Damage %d, Health %d / %d\n", attackDamage, n->GetHealth(), n->GetMaxHealth());
			if (!n->GetSprite()) return;//return if already deleted

			float dist = Distance(n->GetPosition(), position);
			n->Nature::Damage(CalculateAttackDamage(dist));
			if(!n->IsAlive()) n->SpawnDrops();//only spawn drops when player breaks
		}
	}
}