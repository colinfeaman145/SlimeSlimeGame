#ifndef ENEMY_SPAWNER_HPP
#define ENEMY_SPAWNER_HPP

#include "Element.hpp"
#include "Enemy.hpp"

enum class EnemyType {
	WALL_FOCUS,//GREY
	NORMAL,//GREEN
	PLAYER_FOCUS,//BLUE
	FAST,//PEACH
	ATLAS_FOCUS,//ORANGE
	EXPLOSIVE,//RED
	//PINK = random
};

class EnemySpawner : public Element
{
	public:
		void Initialize();
		void Draw(Renderer* renderer) override;
		void Process(float deltaTime, GameContext& context) override;

		void SpawnEnemies(GameContext& context);
		Enemy* GetRandomEnemy(int type);

	private:
		vector<Enemy*> enemies;
};

#endif