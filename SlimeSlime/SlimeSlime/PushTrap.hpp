#ifndef PUSH_TRAP_HPP
#define PUSH_TRAP_HPP

#include "Trap.hpp"

enum class PushTrapDirection {
	NORTH,
	EAST,
	SOUTH,
	WEST
};

class PushTrap : public Trap {
	public:
		PushTrap* Clone() const override { return new PushTrap(*this); }
		void Initialize(int dir);
		void ActivateTrapAbility(Enemy* e) override;

		void HandleCollision(Collidable* other, Vector2 penetration) override;
	private:
		void SetDirection(int dir);
		PushTrapDirection direction;
};

#endif