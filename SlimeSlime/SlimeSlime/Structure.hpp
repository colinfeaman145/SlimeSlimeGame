#ifndef STRUCTURE_HPP
#define STRUCTURE_HPP

#include "Vector2.hpp"
#include "Sprite.hpp"
#include "GridCoord.hpp"
#include "Renderer.hpp"
#include "Element.hpp"
#include "Attackable.hpp"
#include "PercentageBar.hpp"
#include "Resource.hpp"

class Grid;

class Structure : public virtual Element, public Collidable, public Attackable {
public:
    Structure();
    Structure(const Structure& other);
    virtual Structure* Clone() const;
    virtual ~Structure();

    bool Initialize(Sprite* spr, bool canCollide);
    virtual void Draw(Renderer* renderer) override;
    virtual void Process(float deltaTime) override;

    void ChangeSize(int w, int h);
    Vector2 GetPosition() const override{ return position; }
    Sprite* GetSprite() const { return sprite; }

    bool IsBroken() const;
    virtual void SetBroken(bool b);
    virtual void SetPosition(Vector2 pos);
    void Damage(float amount) override;
    void Heal(int amount);
    void SetDurability(int d);
    int GetDurability();
    auto GetRecipe() const { return recipe; }
    void SetRecipe(unordered_map<ResourceType, int> r);
    virtual float GetTraversalCost() const { return 100; }

    ResourceType GetDropType() const override;
    int GetDropAmount() const override;

    void HandleCollision(Collidable* other, Vector2 penetration) override;

protected:
    Sprite* sprite;
    Vector2 position;
    bool broken;
    unordered_map<ResourceType, int> recipe;
    PercentageBar* healthBar;
};  

#endif