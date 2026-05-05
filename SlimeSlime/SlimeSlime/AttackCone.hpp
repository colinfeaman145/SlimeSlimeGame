#ifndef ATTACK_CONE_HPP
#define ATTACK_CONE_HPP

#include "Entity.hpp"
#include "AnimatedSprite.hpp"
#include "ResourceType.hpp"

class Enemy;
class Nature;

class AttackCone : public Entity {
	public:
		AttackCone(int damage = 10, int r = 100, float ha = (PI/6), float ac = 1);
		bool Initialize(Vector2 pos, AnimatedSprite* spr = nullptr);
		void Draw(Renderer* renderer) override;
		void Process(float deltaTime, GameContext& context) override;
		void SetTargetPosition(Vector2 target);

		bool CanAttack();
		void PlayAttack();

		void IncreaseAttackDamage(int damage);
		void IncreaseRadius(int increaseAmount);
		void IncreaseWidth(float increaseAmount);
		int GetAttackDamage() const { return attackDamage; }
		int GetRadius() const { return radius; }
		float GetHalfAngle() const { return halfAngle; }
		float GetCooldownTime() const { return attackCooldown; }
		float GetCurrentAttackCooldownTime() const { return currentAttackTime; }

		void ProcessDropQueue(int dropPickupRadius, GameContext& context);

		void HandleCollision(Collidable* other, Vector2 penetration, GameContext& context) override;
	private:
		int attackDamage;
		int radius;
		float halfAngle;
		float attackCooldown;
		float currentAttackTime;
		AnimatedSprite* swoosh;


		vector<ResourceDrop> dropQueue;
};

#endif