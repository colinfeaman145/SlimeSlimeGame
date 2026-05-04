#ifndef GRID_HPP
#define GRID_HPP

#include <vector>
#include <unordered_map>
#include <functional>
#include "Vector2.hpp"
#include "Entity.hpp"
#include "GridCell.hpp"
#include "GridCoord.hpp"
#include "GameContext.hpp"
#include "NatureType.hpp"

class Grid : public Element {
public:
    Grid(int worldWidth, int worldHeight, int cellSize);
    ~Grid();
    bool Initialize(SDL_Texture* cellSprite, GameContext& context);
    void Draw(Renderer* renderer) override;
    void Process(float deltaTime, GameContext& context) override;

    int GetCellSize() const { return cellSize; }
    int GetGridWidth() const { return gridWidth; }
    int GetGridHeight() const { return gridHeight; }

    GridCell* GetCell(GridCoord coord);
    GridCell* GetCell(int col, int row);
    bool IsValidCoord(GridCoord coord) const;
    GridCoord WorldToGrid(Vector2 worldPos) const;
    Vector2 GridToWorld(GridCoord coord) const; //returns top-left of cell
    Vector2 SnapToGrid(Vector2 worldPos) const; //snaps a world position to cell origin

    vector<GridCell*> FindEntityCells(Entity* entity);
    vector<GridCell*> GetNeighbourCells(GridCoord coord, int radius);
    vector<Collidable*> GetNearbyCollidables(GridCoord coord, int radius);

    template<typename T>
    void UpdateOccupancy(T* entity, void (GridCell::*addFunc)(T*), void (GridCell::*removeFunc)(T*));
    bool ResolveCollisions(Entity* entity, GameContext& context);

    //Enemies
    void AddEnemy(Enemy* enemy);
    void RemoveEnemy(Enemy* enemy);
    void UpdateEnemyOccupancy(Enemy* enemy);

    //Structures
    bool CanPlaceStructure(GridCoord coord) const;
    bool PlaceStructure(Structure* structure, GridCoord coord);
    bool RemoveStructure(GridCoord coord);

    //Walls
    bool CanPlaceWall(GridCoord coord, EdgeDirection dir);
    bool PlaceWall(GridCoord coord, EdgeDirection dir, Structure* w);
    bool RemoveWall(GridCoord coord, EdgeDirection dir);

    //Nature
    void PlaceNature(uniform_int_distribution<int> spreadChance, NatureType type, GridCell* cell, GameContext& context);
    void PlaceFoliage(uniform_int_distribution<int> spreadChance, GridCell* cell, GameContext& context);
    void RemoveNature(Nature* n);

    //Drops
    void AddDrop(Resource* drop);
    void RemoveDrop(Resource* drop);
    void SpawnDrops(ResourceDrop drop, int dropPickupRadius, GameContext& context);
    void UpdateDropOccupancy(Resource* drop);

private:

    void CondenseToGold(ResourceDrop drop, int dropPickupRadius, GameContext& context);

    int cellSize;
    int gridWidth; //in cells
    int gridHeight;

    GridCell*** cells;//2d array of pointers
};

//OCCUPANCY MANAGEMENT
template<typename T>
//updates Entities value occupancy
void Grid::UpdateOccupancy(T* entity, void (GridCell::*addFunc)(T*), void (GridCell::*removeFunc)(T*)) {

    static_assert(std::is_base_of_v<Entity, T>, "T must be a subclass of Entity");
    if (!entity) return;

    Vector2 pos = entity->GetCollisionBound().WorldPosition(entity->GetPosition());
    float r = entity->GetRadius();

    GridOccupancy next = {
        (int)floor((pos.x - r) / cellSize),
        (int)floor((pos.x + r) / cellSize),
        (int)floor((pos.y - r) / cellSize),
        (int)floor((pos.y + r) / cellSize)
    };

    const GridOccupancy& prev = entity->GetOccupancy();
    if (next == prev) return; //if no change, dont update anything

    //Remove from old cells
    for (int row = prev.minRow; row <= prev.maxRow; ++row) {
        for (int col = prev.minCol; col <= prev.maxCol; ++col) {
            if (GridCell* cell = GetCell(col, row))
                (cell->*removeFunc)(entity);
        }
    }

    //Insert into new cells
    for (int row = next.minRow; row <= next.maxRow; ++row) {
        for (int col = next.minCol; col <= next.maxCol; ++col) {
            if (GridCell* cell = GetCell(col, row))
                (cell->*addFunc)(entity);
        }
    }

    entity->SetOccupancy(next);
}

#endif