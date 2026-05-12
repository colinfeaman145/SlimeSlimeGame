#ifndef ENEMY_SPAWNER_HPP
#define ENEMY_SPAWNER_HPP

#include "Element.hpp"
#include "Enemy.hpp"
#include "AnimatedSprite.hpp"
#include <fstream>
#include <json.hpp>

using json = nlohmann::json;

struct SpawnPool {//stores spawn weights for enemy types between min and max gameProgress
	float min;
	float max;
	unordered_map<EnemyType, float> weights;
};

class EnemySpawner : public Element
{
	public:
		EnemySpawner() = default;
		void Initialize(const string& enemyStats, const string& spawnPools);
		void InitEnemyStats(const string& enemyStats);
		void InitSpawnPools(const string& spawnPools);
		void InitSprites();
		void Draw(Renderer* renderer) override;
		void Process(float deltaTime) override;

		void SpawnEnemies();
		Enemy* GetRandomEnemy(SpawnPool pool, float gameProg, int cellSize);
		EnemyType GetRandomEnemyType(SpawnPool pool);
		EnemyType StringToEnemyType(const string& str);

		AnimatedSprite* GetSprite(EnemyType type);//used for how to play menu

	private:
		vector<Enemy*> enemies;
		vector<SpawnPool> spawnPools;

		unordered_map<EnemyType, uniform_int_distribution<int>> damage;
		unordered_map<EnemyType, uniform_int_distribution<int>> health;
		unordered_map<EnemyType, uniform_real_distribution<float>> size;
		unordered_map<EnemyType, uniform_real_distribution<float>> moveSpeed;
		unordered_map<EnemyType, uniform_real_distribution<float>> attackSpeed;

		//sprites
		unordered_map<EnemyType, AnimatedSprite*> moving;
		unordered_map<EnemyType, AnimatedSprite*> attacking;
		unordered_map<EnemyType, AnimatedSprite*> death;
};

#endif