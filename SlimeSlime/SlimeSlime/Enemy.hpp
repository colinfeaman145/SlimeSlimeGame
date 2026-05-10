#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "Entity.hpp"
#include "GridCoord.hpp"
#include "AnimatedSprite.hpp"
#include "Explosion.hpp"

enum class EnemyType {
	WALL_FOCUS,//GREY
	NORMAL,//GREEN
	PLAYER_FOCUS,//BLUE
	FAST,//PEACH
	ATLAS_FOCUS,//ORANGE
	EXPLOSIVE,//RED
	RANDOM//PINK = random
};

class Enemy : public Entity {
	public:
		Enemy();
		~Enemy();
		void Initialize(Vector2 pos, AnimatedSprite* spr, float retarget, int targetRad, float atlasTarget, float playerTarget);
		void Draw(Renderer* renderer) override;
		void Process(float deltaTime) override;
		void SetSprites(AnimatedSprite* move, AnimatedSprite* attack, AnimatedSprite* die);
		void SetSpritesDrawSize(int size);
		void SetSpriteDirection(bool b);
		void SetType(EnemyType t);
		EnemyType GetType();
		void SetDamage(int d);
		int GetDamage();
		void SetDead() override;
		void Damage(float amount) override;
		bool IsDying();
		void SetAttackCooldown(float atckCool);
		void SetExplosion();
		void TryExplode();
		void SetFrozen(float duration);
		bool IsFrozen();
		float GetAttackCooldown();
		ResourceType GetDropType() const override;
		int GetDropAmount() const override;

		void Attack(Attackable* a);

		void HandleCollision(Collidable* other, Vector2 penetration) override;

		//pathfinding
		void SetTarget(Collidable* c);
		Collidable* GetTarget() const { return target; }
		Collidable* FindNewTarget();
		void Hone();
	private:
		Collidable* target;
		GridCoord lastCell;
		Vector2 previousPosition;
		float stuckTime;
		float frozenTime;
		bool unfrozen;

		EnemyType type;
		int damage;
		float attackCooldown;
		float currentAttackCooldown;

		AnimatedSprite* moving;
		AnimatedSprite* attacking;
		AnimatedSprite* death;
		
		float adjustCourseTimer; 
		float currentRetargetTime;
		float retargetCooldown;
		int framesSinceLastHone;
		int targetRadius;
		float atlasTargetChance;//higher = less likely
		float playerTargetChance;

		//for exploding slime only
		Explosion* explosion;
};

#endif