#ifndef EXPLOSION_TRAP_HPP
#define EXPLOSION_TRAP_HPP

#include "Explosion.hpp"
#include "Trap.hpp"

class ExplosionTrap : public Trap {
	public:
		~ExplosionTrap();
		ExplosionTrap* Clone() const override { return new ExplosionTrap(*this); }
		void Initialize();
		void Draw(Renderer* renderer) override;
		void Process(float deltaTime) override;

		void SetPosition(Vector2 pos) override;

		void ActivateTrapAbility(Enemy* e) override;
		void IncreaseAttack(float amount) override;
		void IncreaseAttackByPercent(float amount) override;

		void HandleCollision(Collidable* other, Vector2 penetration) override;
	private:
		Explosion* explosion;
};

#endif