#ifndef GRID_CELL_HPP
#define GRID_CELL_HPP
#include <vector>
#include "Vector2.hpp"
#include "Sprite.hpp"
#include "GridCoord.hpp"
#include "Structure.hpp"
#include "Renderer.hpp"
#include "Entity.hpp"
#include "Collidable.hpp"
#include "Collision.hpp"
#include "Nature.hpp"

using namespace std;
class GridCell {
public:
    GridCell(Sprite* spr);
    ~GridCell() = default;

    void SetSprite(Sprite* spr);
    void SetPosition(Vector2 worldPos); //called once when grid is 
    void SetCoords(GridCoord gridPos);
    Sprite* GetSprite() const { return sprite; }
    GridCoord GetCoords() const { return coords; }
    Vector2 GetPosition() const { return position; }
    void Draw(Renderer* renderer);
    void Process(float deltaTime, GameContext& context);
    void DrawWalls(Renderer* renderer);
    void ProcessWalls(float deltaTime, GameContext& context);
    void DrawNature(Renderer* renderer);

    //entity management
    void AddEntity(Entity* entity);
    void RemoveEntity(Entity* entity);
    void ClearEntities();
    const vector<Entity*>& GetEntities() const { return entities; }
    vector<Collidable*> GetCollidables() const;

    //structures
    void AddStructure(Structure* structure);
    void RemoveStructure();
    bool HasStructure() const;
    Structure* GetStructure() const { return strctr; };
    void SetHoldingHologramStruct(bool b);

    //walls
    bool PlaceWall(EdgeDirection dir, Structure* wall);
    bool RemoveWall(EdgeDirection dir);
    bool HasWall(EdgeDirection dir) const;
    Structure* GetWall(EdgeDirection dir) const;
    void SetHoldingHologramWall(bool b, EdgeDirection dir);

    //nature
    void PlaceNature(Nature* nature);
    void RemoveNature(Nature* nature);

private:
    GridCoord coords;
    Vector2 position;
    Sprite* sprite;
    vector<Entity*> entities;
    Structure* strctr;
    Structure* walls[2];
    bool holdingHologramStruct;
    bool holdingHologramWall[2];
    vector<Nature*> nature;
};

#endif