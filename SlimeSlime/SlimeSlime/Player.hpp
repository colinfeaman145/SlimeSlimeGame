#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Entity.hpp"
#include "AttackCone.hpp"

class Resource;

class Player : public Entity {
	public:

		void Initialize(Vector2 pos, int pullRadius = 100, Vector2 vel = Vector2(0, 0), Sprite* spr = nullptr);
		void Draw(Renderer* renderer) override;
		void Process(float deltaTime, GameContext& context) override;

		void SetAttackCone(AttackCone* ac);
		AttackCone* GetAttackCone();
		void AddCoins(int amount);
		void RemoveCoins(int amount);
		bool HasEnoughCoins(int amount);
		void AddWood(int amount);
		void RemoveWood(int amount);
		bool HasEnoughWood(int amount);
		void AddStone(int amount);
		void RemoveStone(int amount);
		bool HasEnoughStone(int amount);

		void HandleCollision(Collidable* other, Vector2 penetration, GameContext& context) override;
		void HandleResourcePickup(Resource* r, GameContext& context);

	private:
		AttackCone* attackCone;
		int coins;
		int wood;
		int stone;
		int itemPullRadius ;
		int itemPickupRadius;
};

#endif