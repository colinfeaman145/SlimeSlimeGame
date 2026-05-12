#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Entity.hpp"
#include "AttackCone.hpp"

class Resource;

class Player : public Entity {
	public:

		void Initialize(Vector2 pos, int pullRadius = 100, Vector2 vel = Vector2(0, 0), AnimatedSprite* spr = nullptr);
		void SetSprites(AnimatedSprite* f, AnimatedSprite* b);
		void Draw(Renderer* renderer) override;
		void Process(float deltaTime) override;

		void SetAttackCone(AttackCone* ac);
		AttackCone* GetAttackCone();
		void AddCoins(int amount);
		void RemoveCoins(int amount);
		bool HasEnoughCoins(int amount);
		int GetCoins() const { return coins; }
		void AddWood(int amount);
		void RemoveWood(int amount);
		bool HasEnoughWood(int amount);
		int GetWood() const { return wood; }
		void AddStone(int amount);
		void RemoveStone(int amount);
		bool HasEnoughStone(int amount);
		int GetStone() const { return stone; }
		void IncreaseMaxHealth(int amount);
		void DecreaseHealCooldownByPercent(float amount);
		void Damage(float amount) override;
		void Heal(int amount) override;
		bool CanMakeRecipe(unordered_map<ResourceType, int> recipe);
		void RemoveRecipeCost(unordered_map<ResourceType, int> recipe);

		void HandleCollision(Collidable* other, Vector2 penetration) override;
		void HandleResourcePickup(Resource* r);

	private:
		AttackCone* attackCone;
		int coins;
		int wood;
		int stone;
		int itemPickupRadius;
		PercentageBar* cooldownBar;

		float healCooldown;
		float healAmount;
		float onHitHealCooldownAdd;
};

#endif