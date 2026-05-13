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
#include "Container.hpp"
#include "Button.hpp"

#define GRID_WIDTH 15000
#define GRID_HEIGHT 15000
#define CELL_SIZE 150

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
    Structure* GetCurrentStructure(int currentStruct);
    int GetCurrentStructureNumber() const { return currentStructure; }
    Entity* GetPlayer();
    void MovePlayer(MovementDir dir, float deltaTime);
    void Attack();

    void UpdateCurrentHoveredCell(bool canAfford);
    void UpdateHoverColor(GridCell* cell, bool canAfford);
    void ApplyHoverEffect(GridCell* cell, bool canAfford);
    void RemoveHoverEffect(GridCoord coord);

    void InitializeUpgradeContainer();
    void UpgradeDamage();
    void UpgradeHealth();
    void UpgradeAttackSpeed();
    void UpgradeAttackRange();
    void UpgradeStructureHealth();
    void UpgradeTrapDamage();
    void UpgradeTrapCooldown();
    bool CanUpgrade();
    void IncreaseUpgradeCost();
    void ToggleUpgradeBox();
    void UpdateUpgradeLabelColor();

    void InitializeStructureHUD();
    Container* MakeStructureCard(int i);
    void InitializeUI();
    void PauseGame();

private:
    bool gameRunning;
    float time;
    float spawnCooldown;
    float currentSpawnTime;
    
    Player* player;
    EnemySpawner* spawner;
    GridCoord currentHoveredCellCoords;
    int lastHoveredStructure;
    Structure* wallHwood;
    Structure* wallVwood;
    Structure* wallHstone;
    Structure* wallVstone;
    PushTrap* pushTrap;
    SpikeTrap* spikeTrap;
    FireTrap* fireTrap;
    FreezeTrap* freezeTrap;
    ExplosionTrap* explosionTrap;
    Structure* atlas;

    int currentStructure;
    bool isStone;
    int pushTrapDirection;
    bool buildMode;
    Text* woodCount;
    Text* stoneCount;
    Text* coinCount;

    Container* upBox;
    Vector2 textDrawSize;

    Container* structureHUD;
    Container* structureCards[7];

    int upgradeLevel;
    Text* upgradeCostLabel;
    int upgradeCost;
    Text* upDamageLabel;
    int damageLevel;
    Text* upHealthLabel;
    int healthLevel;
    Text* upRangeLabel;
    int rangeLevel;
    Text* upCooldownLabel;
    int cooldownLevel;
    Text* upDurabilityLabel;
    int durabilityLevel;
    Text* upTrapDamageLabel;
    int trapDamageLevel;
    Text* upTrapCooldownLabel;
    int trapCooldownLevel;

    Container* gameOverScreen;
    Container* playerDeathScreen;
    Container* pauseScreen;
    bool gamePaused;
    float respawnTimer;
    Text* respawnTimerText;
    Text* timerText;//for end of game

};

#endif