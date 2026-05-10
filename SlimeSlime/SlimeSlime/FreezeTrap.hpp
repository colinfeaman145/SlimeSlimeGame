#ifndef FREEZE_TRAP_HPP
#define FREEZE_TRAP_HPP

#include "Trap.hpp"

class FreezeTrap : public Trap {
	public:
		FreezeTrap() = default;
		FreezeTrap* Clone() const override { return new FreezeTrap(*this); }
		void Initialize();
		void ActivateTrapAbility(Enemy* e) override;

		void HandleCollision(Collidable* other, Vector2 penetration) override;
};

#endif