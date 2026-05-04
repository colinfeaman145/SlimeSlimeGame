#ifndef RESOURCE_HPP
#define RESOURCE_HPP

#include "Entity.hpp"
#include "ResourceType.hpp"

class Player;


class Resource : public Entity {
	public:
		void Initialize(Vector2 pos, Vector2 vel, Sprite* spr, float ft, ResourceType t);
		void Process(float deltaTime, GameContext& context) override;
		void HandleCollision(Collidable* other, Vector2 penetration, GameContext& context);

		ResourceType GetResourceType() const { return type; }

	private:
		ResourceType type;
		float fallTime;
};

#endif