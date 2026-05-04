#ifndef ATTACKABLE_HPP
#define ATTACKABLE_HPP

#include "ResourceType.hpp"

struct ResourceDrop {
	ResourceType type;
	Vector2 spawnerPosition;//position of entity that is dropping the resource
	Vector2 spawnerSize;
	int amount;
};

static uniform_real_distribution<double> dropVariance(0.75, 1.5);

//VIRTUAL CLASS
class Attackable {
	protected:
		int maxHealth;
		int health;
	public:
		inline int GetHealth();
		inline int GetMaxHealth();
		inline void SetHealth(int h);
		virtual ResourceType GetDropType() const = 0;
		virtual int GetDropAmount() const = 0;
};

inline int Attackable::GetHealth() {
	return health;
}

inline int Attackable::GetMaxHealth() {
	return maxHealth;
}

inline void Attackable::SetHealth(int h) {
	maxHealth = h;
	health = h;
}

#endif