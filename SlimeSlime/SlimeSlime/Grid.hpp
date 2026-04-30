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

class Grid : public Element {
public:
    Grid(int worldWidth, int worldHeight, int cellSize);
    ~Grid();
    bool Initialize(SDL_Texture* cellSprite);
    void Draw(Renderer* renderer) override;
    void Process(float deltaTime, GameContext& context) override;

    GridCell* GetCell(GridCoord coord);
    GridCell* GetCell(int col, int row);
    bool IsValidCoord(GridCoord coord) const;

    GridCoord WorldToGrid(Vector2 worldPos) const;
    Vector2 GridToWorld(GridCoord coord) const; //returns top-left of cell
    Vector2 SnapToGrid(Vector2 worldPos) const; //snaps a world position to cell origin

    bool CanPlaceStructure(GridCoord coord) const;
    bool PlaceStructure(Structure* structure, GridCoord coord);
    bool RemoveStructure(GridCoord coord);

    void UpdateEntity(Entity* entity);
    void RemoveEntity(Entity* entity);

    vector<GridCell*> GetNeighbourCells(GridCoord coord, int radius);
    vector<Entity*> GetNearbyEntities(GridCoord coord, int radius);
    vector<Collidable*> GetNearbyCollidables(GridCoord coord, int radius);
    void ResolveCollisions(Entity* entity, GameContext& context);

    int GetCellSize()   const { return cellSize; }
    int GetGridWidth()  const { return gridWidth; }
    int GetGridHeight() const { return gridHeight; }

    bool PlaceWall(GridCoord coord, EdgeDirection dir, Structure* w);
    bool RemoveWall(GridCoord coord, EdgeDirection dir);

private:
    int cellSize;
    int gridWidth; //in cells
    int gridHeight;

    GridCell*** cells;//2d array of pointers
};

#endif