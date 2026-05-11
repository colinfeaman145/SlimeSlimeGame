#ifndef TRAP_HPP
#define TRAP_HPP

#include "Structure.hpp"
#include "ParticleEmitter.hpp"
#include "Enemy.hpp"

class Trap : public Structure {

	public:
		Trap() = default;
		Trap(const Trap& other);
		virtual void Initialize();
		virtual void Draw(Renderer* renderer) override;
		virtual void Process(float deltaTime) override;

		void Activate();
		virtual void ActivateTrapAbility(Enemy* e) = 0;
		virtual void IncreaseAttack(float amount);
		virtual void IncreaseAttackByPercent(float amount);
		void DecreaseTrapCooldownByPercent(float amount);

		void SetPosition(Vector2 pos) override;
		virtual void HandleCollision(Collidable* other, Vector2 penetration) = 0;

	protected:
		float activationDelay;//delay after activation before damaging
		float cooldown;
		float currentTimer;
		float damage;
		bool activated;
		bool abilityActivated;
		bool activateAbility;//?
		ParticleEmitter* particles;
		PercentageBar* cooldownBar;
};

#endif