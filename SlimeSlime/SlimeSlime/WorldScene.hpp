#ifndef WorldScene_HPP
#define WorldScene_HPP

#include "Scene.hpp"
#include "Grid.hpp"
#include "AttackCone.hpp"

class WorldScene : public Scene {
public:
    WorldScene();
    virtual ~WorldScene();

    virtual bool Initialize(GameContext& context);
    virtual void Process(GameContext& context, float deltaTime);
    virtual void Draw(Renderer* renderer);

    void ToggleBuildMode();
    void ChangeStructure(int s, GameContext& context);
    void PlaceStructure(GameContext& context, bool isHologram = false);
    void RemoveStructure(GameContext& context);
    Entity* GetPlayer();
    void MovePlayer(MovementDir dir, float deltaTime);

    void UpdateCurrentHoveredCell(GameContext& context);
    void ApplyHoverEffect(GridCell* cell, GameContext& context);
    void RemoveHoverEffect(GridCoord coord, GameContext& context);

private:
    float time;
    
    Entity* player;
    AttackCone* attackCone;
    GridCoord currentHoveredCellCoords;
    Structure* st;
    Structure* wallH;
    Structure* wallV;
    Text* text;
    Grid* grid;
    float explosionFrameTime;
    int currentStructure;
    bool buildMode;
};

#endif