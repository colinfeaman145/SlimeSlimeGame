#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "Entity.hpp"
#include "GridCoord.hpp"

class Enemy : public Entity, public Attackable {
	public:
		void Initialize(Vector2 pos, Sprite* spr, float retarget, int targetRad, float atlasTarget, float playerTarget);
		void Draw(Renderer* renderer) override;
		void Process(float deltaTime, GameContext& context) override;
		ResourceType GetDropType() const override;
		int GetDropAmount() const override;
		void HandleCollision(Collidable* other, Vector2 penetration, GameContext& context) override;

		void SetTarget(Collidable* c);
		Collidable* GetTarget() const { return target; }
		Collidable* FindNewTarget(GameContext& context);
		void Hone(GameContext& context);
	private:
		Collidable* target;
		GridCoord lastCell;
		float adjustCourseTimer; 

		int damage;

		float currentRetargetTime;
		float retargetCooldown;
		int targetRadius;
		float atlasTargetChance;//higher = less likely
		float playerTargetChance;
};

#endif