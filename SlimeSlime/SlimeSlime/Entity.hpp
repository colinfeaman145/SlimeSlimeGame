#ifndef ENTITY_HPP
#define ENTITY_HPP
#include "Element.hpp"
#include "Collidable.hpp"
#include "Attackable.hpp"
#include "PercentageBar.hpp"

struct GridOccupancy {
	int minCol, maxCol, minRow, maxRow;

	bool operator==(const GridOccupancy& o) const {
		return minCol == o.minCol && maxCol == o.maxCol
			&& minRow == o.minRow && maxRow == o.maxRow;
	}
};

enum class MovementDir {
	NORTH = 1,
	EAST = 2,
	SOUTH = 3,
	WEST = 4
};

class Renderer;
class Sprite;

class Entity : public virtual Element, public virtual Collidable, public virtual Attackable
{
protected:
	Sprite* sprite;
	PercentageBar* healthBar;
	float radius;//for collision
	Vector2 position;
	Vector2 velocity;
	bool alive;
	GridOccupancy occupancy;//what gridCells the entity is in(used for collision detection)
	float movementSpeed;

	//for flash on hit animation
	float flashDuration;

public:
	Entity();
	~Entity();
	bool Initialize(Vector2 pos, Vector2 vel = Vector2(0, 0), Sprite* spr = nullptr);
	void Draw(Renderer* renderer) override;
	void Process(float deltaTime, GameContext& context) override;
	void Rotate(float direction);
	bool IsAlive() const;
	void SetDead();
	void Damage(int amount);
	void Heal(int amount);
	float GetRadius();
	Vector2 GetFacingDirection();
	Vector2 GetPosition() override; 
	Vector2 GetCorner();
	Vector2& GetVelocity();
	Sprite* GetSprite();
	float GetMovementSpeed();
	void SetMovementSpeed(float speed);
	void SetPosition(Vector2 pos);
	void Move(MovementDir m, float deltaTime);
	void SetHealthBar(PercentageBar* bar);
	PercentageBar* GetHealthBar();

	void SetFlash(bool flash);

	virtual ResourceType GetDropType() const { return ResourceType::COIN; }
	virtual int GetDropAmount() const { return ((health / 10) * dropVariance(gen)); }

	GridOccupancy GetOccupancy() const;
	void SetOccupancy(GridOccupancy occ);

	void HandleCollision(Collidable* other, Vector2 penetration, GameContext& context) override;


};

#endif