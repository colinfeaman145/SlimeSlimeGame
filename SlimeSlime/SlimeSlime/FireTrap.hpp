#ifndef FIRE_TRAP_HPP
#define FIRE_TRAP_HPP

#include "Trap.hpp"

class FireTrap : public Trap {
	public:
		FireTrap() = default;
		FireTrap* Clone() const override { return new FireTrap(*this); }
		void Initialize();
		void ActivateTrapAbility(Enemy* e) override;

		void HandleCollision(Collidable* other, Vector2 penetration) override;
};

#endif