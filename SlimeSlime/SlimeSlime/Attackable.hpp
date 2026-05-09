#ifndef ATTACKABLE_HPP
#define ATTACKABLE_HPP

#include "ResourceType.hpp"
#include "Vector2.hpp"
#include <random>

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
		float maxHealth;
		float health;
	public:
		inline int GetHealth();
		inline int GetMaxHealth();
		inline void SetHealth(float h);
		virtual ResourceType GetDropType() const = 0;
		virtual int GetDropAmount() const = 0;
		virtual void Damage(float amount) = 0;
};

inline int Attackable::GetHealth() {
	return health;
}

inline int Attackable::GetMaxHealth() {
	return maxHealth;
}

inline void Attackable::SetHealth(float h) {
	maxHealth = h;
	health = h;
}

#endif