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
#include "Resource.hpp"
#include "Enemy.hpp"

class Nature;
class Foliage;

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
    void Process(float deltaTime, GameContext& context, bool isRendered);
    void DrawWalls(Renderer* renderer);
    void ProcessWalls(float deltaTime, GameContext& context);
    void DrawNature(Renderer* renderer);
    void ProcessNature(float deltaTime, GameContext& context);
    void DrawDrops(Renderer* renderer);
    void ProcessDrops(float deltaTime, GameContext& context);

    vector<Collidable*> GetCollidables() const;

    //enemy
    void AddEnemy(Enemy* enemy);
    void RemoveEnemy(Enemy* enemy);
    void ClearEnemies();
    const vector<Enemy*>& GetEnemies() const { return enemies; }

    //structures
    void AddStructure(Structure* structure);
    void RemoveStructure();
    bool HasStructure() const;
    Structure* GetStructure() const { return strctr; };
    void SetHoldingHologramStruct(bool b);

    //walls
    bool PlaceWall(WallDirection dir, Structure* wall);
    bool RemoveWall(WallDirection dir);
    bool HasWall(WallDirection dir) const;
    Structure* GetWall(WallDirection dir) const;
    void SetHoldingHologramWall(bool b, WallDirection dir);

    //nature
    void SetNaturePosition(Nature* n);
    void PlaceNature(Nature* nature);
    void RemoveNature(Nature* nature);

    //drops
    void AddDrop(Resource* drop);
    void RemoveDrop(Resource* drop);

    //other entities
    void AddOther(Entity* p);
    void RemoveOther(Entity* p);

private:
    GridCoord coords;
    Vector2 position;
    Sprite* sprite;
    vector <Entity*> entities;//player, attackCone
    vector<Enemy*> enemies;
    vector<Nature*> nature;
    vector<Resource*> drops;
    Structure* strctr;
    Structure* walls[2];
    bool holdingHologramStruct;
    bool holdingHologramWall[2];
};

#endif