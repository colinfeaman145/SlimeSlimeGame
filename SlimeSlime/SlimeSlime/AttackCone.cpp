#include "AttackCone.hpp"

AttackCone::AttackCone(int damage, int r, float ha) {
	attackDamage = damage;
	radius = r;
	halfAngle = ha;
}

bool AttackCone::Initialize(Vector2 pos, Vector2 vel, AnimatedSprite* spr) {
	Entity::Initialize(pos, vel, spr);
	CollisionShape cs = CollisionShape::MakeCone(radius, Vector2(1.0, 0.0), halfAngle);
	SetCollisionBound(cs);
	swoosh = spr;
	return true;
}

void AttackCone::Draw(Renderer* renderer) {
	Collidable::Draw(renderer, {100, 100, 100}, 100, RenderLayer::ATTACK_CONE);
}

void AttackCone::SetTargetPosition(Vector2 target) {
	Vector2 dir = target - position;
	float angle = atan2(dir.y, dir.x);
	collisionBound.SetConeRotation(angle);
}

void AttackCone::IncreaseAttackDamage(int damage) {
	attackDamage += damage;
}

void AttackCone::IncreaseRadius(int increaseAmount) {

}

void AttackCone::IncreaseWidth(float increaseAmount) {

}