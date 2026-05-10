#ifndef WorldScene_HPP
#define WorldScene_HPP

#include "Scene.hpp"
#include "Grid.hpp"
#include "AttackCone.hpp"
#include "Player.hpp"
#include "EnemySpawner.hpp"
#include "PushTrap.hpp"
#include "SpikeTrap.hpp"
#include "FireTrap.hpp"
#include "FreezeTrap.hpp"
#include "ExplosionTrap.hpp"

class WorldScene : public Scene {
public:
    WorldScene();
    virtual ~WorldScene();

    virtual bool Initialize();
    void Process(float deltaTime) override;
    void Draw(Renderer* renderer) override;
    void ReadInputs(float deltaTime) override;

    void LoadSounds();

    void ToggleBuildMode();
    void LeftMouseClick();
    void ChangeStructure(int s);
    void ChangeMaterial();
    void PlaceStructure(bool isHologram = false);
    void RemoveStructure(bool couldntAfford = false);
    Structure* GetCurrentStructure();
    int GetCurrentStructureNumber() const { return currentStructure; }
    Entity* GetPlayer();
    void MovePlayer(MovementDir dir, float deltaTime);
    void Attack();

    void UpdateCurrentHoveredCell(bool canAfford);
    void UpdateHoverColor(GridCell* cell, bool canAfford);
    void ApplyHoverEffect(GridCell* cell, bool canAfford);
    void RemoveHoverEffect(GridCoord coord);

private:
    float time;
    float spawnCooldown;
    float currentSpawnTime;
    
    Player* player;
    EnemySpawner* spawner;
    GridCoord currentHoveredCellCoords;
    int lastHoveredStructure;
    Structure* st;
    Structure* wallHwood;
    Structure* wallVwood;
    Structure* wallHstone;
    Structure* wallVstone;
    PushTrap* pushTrap;
    SpikeTrap* spikeTrap;
    FireTrap* fireTrap;
    FreezeTrap* freezeTrap;
    ExplosionTrap* explosionTrap;
    int currentStructure;
    bool isStone;
    int pushTrapDirection;
    bool buildMode;
    Text* woodCount;
    Text* stoneCount;
    Text* coinCount;
};

#endif