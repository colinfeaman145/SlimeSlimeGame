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
	int coneRadius = radius;
	Entity::Initialize(pos, Vector2(), spr);//constructor fucks me, fix it
	radius = coneRadius;

	CollisionShape cs = CollisionShape::MakeCone(radius, Vector2(1.0, 0.0), halfAngle);
	SetCollisionBound(cs);
	canCollide = false;
	swoosh = spr;
	swoosh->SetPosition(this->position);
	return true;
}

void AttackCone::Draw(Renderer* renderer) {
	Collidable::Draw(renderer, {100, 100, 100}, 100, RenderLayer::ATTACK_CONE);
}

void AttackCone::Process(float deltaTime, GameContext& context) {
	if (currentAttackTime > 0)
		currentAttackTime -= deltaTime;
	Entity::Process(deltaTime, context);
}

void AttackCone::SetTargetPosition(Vector2 target) {
	Vector2 dir = target - position;
	float angle = atan2(dir.y, dir.x);
	collisionBound.SetConeRotation(angle);
}

bool AttackCone::CanAttack() {
	if (currentAttackTime > 0) return false;
	if (swoosh->IsAnimating()) return false;
	return true;
}

void AttackCone::PlayAttack() {
	if (!CanAttack()) return;
	printf("ATTACKNG\n");
	currentAttackTime = attackCooldown;
	swoosh->Animate();
}

void AttackCone::IncreaseAttackDamage(int damage) {
	attackDamage += damage;
}

void AttackCone::IncreaseRadius(int increaseAmount) {
	radius += increaseAmount;
}

void AttackCone::IncreaseWidth(float increaseAmount) {
	halfAngle += increaseAmount;
}

void AttackCone::ProcessDropQueue(int dropPickupRadius, GameContext& context) {
	for (ResourceDrop drop : dropQueue) {
		context.grid->SpawnDrops(drop, dropPickupRadius, context);
	}
	dropQueue.clear();
}

void AttackCone::HandleCollision(Collidable* other, Vector2 penetration, GameContext& context) {
	if (Enemy* n = dynamic_cast<Enemy*>(other)) {//if its an enemy
		n->Damage(attackDamage);
		//knockback
	}
	if (Nature* n = dynamic_cast<Nature*>(other)) {//if its nature
		if (!n->GetSprite()) return;//return if already deleted

		printf("Damage %d, Health %d / %d\n", attackDamage, n->GetHealth(), n->GetMaxHealth());
		n->Nature::Damage(attackDamage);
		if (n->GetHealth() <= 0) {//if attack broke structure
			//spawn drops
			ResourceDrop drops = ResourceDrop();
			drops.amount = n->GetDropAmount();
			drops.type = n->GetDropType();
			drops.spawnerPosition = n->GetPosition();
			drops.spawnerSize = n->GetSprite()->GetDrawSize();
			dropQueue.push_back(drops);
			n->Break();
		}
	}
}