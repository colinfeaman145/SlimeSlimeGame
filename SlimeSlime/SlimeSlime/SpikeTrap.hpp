#ifndef SPIKE_TRAP_HPP
#define SPIKE_TRAP_HPP

#include "Trap.hpp"

class SpikeTrap : public Trap {
	public:
		SpikeTrap* Clone() const override { return new SpikeTrap(*this); }
		void Initialize();
		void ActivateTrapAbility(Enemy* e) override;

		void HandleCollision(Collidable* other, Vector2 penetration) override;
};

#endif