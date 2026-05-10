#ifndef EXPLOSION_HPP
#define EXPLOSION_HPP

#include "Entity.hpp"

class Explosion : public Entity {
	public:
		Explosion(const Explosion& other);
		Explosion(int size, int dam);
		void Process(float deltaTime) override;
		void Explode();
		void SetPosition(Vector2 pos) override;
		bool isActive();
		float GetDamageScaler(Collidable* c);
		void Damage(float amount) override;
		void IncreaseDamage(float amount);

		void HandleCollision(Collidable* other, Vector2 penetration) override;

	private:
		float damage;
		bool activated;
		float damageDelay;
		float currentTimer;
		bool canDamage;//?
		bool damageDealt;
};

#endif