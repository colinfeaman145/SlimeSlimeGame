#ifndef ATTACK_CONE_HPP
#define ATTACK_CONE_HPP

#include "Entity.hpp"
#include "AnimatedSprite.hpp"

class AttackCone : public Entity {
	public:
		AttackCone(int damage = 10, int r = 100, float ha = (PI/6));
		bool Initialize(Vector2 pos, Vector2 vel = Vector2(0, 0), AnimatedSprite* spr = nullptr);
		void Draw(Renderer* renderer) override;
		void SetTargetPosition(Vector2 target);
		void IncreaseAttackDamage(int damage);
		void IncreaseRadius(int increaseAmount);
		void IncreaseWidth(float increaseAmount);
		int GetAttackDamage() const { return attackDamage; }
		int GetRadius() const { return radius; }
		float GetHalfAngle() const { return halfAngle; }
	private:
		int attackDamage;
		int radius;
		float halfAngle;
		AnimatedSprite* swoosh;
};

#endif