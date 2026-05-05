#ifndef STRUCTURE_HPP
#define STRUCTURE_HPP

#include "Vector2.hpp"
#include "Sprite.hpp"
#include "GridCoord.hpp"
#include "Renderer.hpp"
#include "Element.hpp"
#include "Attackable.hpp"

class Grid;

class Structure : public virtual Element, public virtual Collidable, public virtual Attackable {
public:
    Structure();
    Structure(const Structure& other);
    virtual Structure* Clone() const;
    virtual ~Structure();

    bool Initialize(Sprite* spr, bool canCollide);
    void Draw(Renderer* renderer) override;
    void Process(float deltaTime, GameContext& context) override;

    void ChangeSize(int w, int h);
    Vector2 GetPosition() override{ return position; }
    Sprite* GetSprite() const { return sprite; }

    bool IsBroken() const;
    virtual void SetBroken(bool b);
    void SetPosition(Vector2 pos);
    virtual void Damage(int amount);
    void Heal(int amount);
    void SetDurability(int d);
    int GetDurability();
    int GetBuildCost() const { return buildCost; }

    ResourceType GetDropType() const override;
    int GetDropAmount() const override;

    void HandleCollision(Collidable* other, Vector2 penetration, GameContext& context) override;

protected:
    Sprite* sprite;
    Vector2 position;
    int maxDurability;
    int durability;
    bool broken;
    int buildCost;
};

#endif