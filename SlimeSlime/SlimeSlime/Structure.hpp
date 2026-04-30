#ifndef STRUCTURE_HPP
#define STRUCTURE_HPP

#include "Vector2.hpp"
#include "Sprite.hpp"
#include "GridCoord.hpp"
#include "Renderer.hpp"
#include "Element.hpp"

class Grid;

class Structure : public Element, public Collidable {
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

    void HandleCollision(Collidable* other, Vector2 penetration, GameContext& context) override;

    void SetPosition(Vector2 pos);


protected:
    Sprite* sprite;
    Vector2 position;
};

#endif