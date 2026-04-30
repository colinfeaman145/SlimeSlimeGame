#ifndef NATURE_HPP
#define NATURE_HPP

#include "Structure.hpp"
#include "GameContext.hpp"
#include "Entity.hpp"


enum class NatureType {
	TREE,
	ROCK,
	BUSH,
	STUMP,
	FOLIAGE
};

class Nature : public Structure, public Entity {
public:
	void Initialize(Sprite* spr, NatureType t, bool canCollide);
	NatureType GetNatureType() const { return type; }
	void SetPosition(Vector2 pos);
	Vector2 GetPosition() override;
	void Draw(Renderer* renderer) override;
	void Process(float deltaTime, GameContext& context) override;

	void HandleCollision(Collidable* other, Vector2 penetration, GameContext& context) override;
private:
	Vector2 position;
	NatureType type;
};

static uniform_int_distribution<int> treeSpriteGen(1, 4);
static uniform_int_distribution<int> rockSpriteGen(1, 6);
static uniform_int_distribution<int> stumpSpriteGen(1, 2);
static uniform_int_distribution<int> bushSpriteGen(1, 4);
static uniform_int_distribution<int> foliageSpriteGen(1, 7);
static uniform_real_distribution<double> widthGen(0.9, 1.1);
static uniform_real_distribution<double> heightGen(.85, 1.15);
static uniform_real_distribution<double> treeScaler(2, 4);
static uniform_real_distribution<double> rockScaler(.5, 2.5);
static uniform_real_distribution<double> stumpScaler(.5, 1);
static uniform_real_distribution<double> bushScaler(.5, 1);

static Nature* GetRandomTree(GameContext& context, int cellSize) {
	int i = treeSpriteGen(gen);

	int scaler = treeScaler(gen);
	int width = cellSize * widthGen(gen) * scaler;
	int height = cellSize * heightGen(gen) * scaler * 1.25;

	SDL_Texture* tex;
	Sprite* sprite = new Sprite();
	Nature* n = new Nature();
	CollisionShape cs;
	switch (i) {
		case(1):
			width *= 1.25;
			tex = context.txm->LoadTexture(context.renderer, "../../assets/nature/tree/tree1.png");
			sprite->Initialize(tex, 150, 191, 0, 0, width, height, RenderLayer::NATURE);
			cs = CollisionShape::MakeAABB(width * 0.2, height * 0.1, Vector2(width * 0.4, height * 0.9));
			break;
		case(2):
			tex = context.txm->LoadTexture(context.renderer, "../../assets/nature/tree/tree2.png");
			sprite->Initialize(tex, 147, 168, 0, 0, width, height, RenderLayer::NATURE);
			cs = CollisionShape::MakeAABB(width * 0.2, height * 0.1, Vector2(width * 0.5, height * 0.9));
			break;
		case(3):
			tex = context.txm->LoadTexture(context.renderer, "../../assets/nature/tree/tree3.png");
			sprite->Initialize(tex, 111, 188, 0, 0, width, height, RenderLayer::NATURE);
			cs = CollisionShape::MakeAABB(width * 0.2, height * 0.1, Vector2(width * 0.4, height * 0.9));
			break;
		case(4):
			height *= 1.25;
			tex = context.txm->LoadTexture(context.renderer, "../../assets/nature/tree/tree4.png");
			sprite->Initialize(tex, 61, 183, 0, 0, width, height, RenderLayer::NATURE);
			cs = CollisionShape::MakeAABB(width * 0.2, height * 0.1, Vector2(width * 0.4, height * 0.9));
			break;
	}
	n->Initialize(sprite, NatureType::TREE, true);
	n->Collidable::SetCollisionBound(cs);
	
	return n;
}

static Nature* GetRandomRock(GameContext& context, int cellSize) {
	int i = rockSpriteGen(gen);

	double scaler = rockScaler(gen);
	int width = cellSize * widthGen(gen) * scaler;
	int height = cellSize * heightGen(gen) * scaler;

	SDL_Texture* tex;
	Sprite* sprite = new Sprite();
	Nature* n = new Nature();
	switch (i) {
	case(1):
		tex = context.txm->LoadTexture(context.renderer, "../../assets/nature/rock/rock1.png");
		sprite->Initialize(tex, 118, 74, 0, 0, width, height * 0.9, RenderLayer::NATURE);
		break;
	case(2):
		tex = context.txm->LoadTexture(context.renderer, "../../assets/nature/rock/rock2.png");
		sprite->Initialize(tex, 98, 82, 0, 0, width, height * 0.9, RenderLayer::NATURE);
		break;
	case(3):
		tex = context.txm->LoadTexture(context.renderer, "../../assets/nature/rock/rock3.png");
		sprite->Initialize(tex, 106, 58, 0, 0, width, height * 0.9, RenderLayer::NATURE);
		break;
	case(4):
		tex = context.txm->LoadTexture(context.renderer, "../../assets/nature/rock/rock4.png");
		sprite->Initialize(tex, 100, 62, 0, 0, width, height * 0.9, RenderLayer::NATURE);
		break;
	case(5):
		tex = context.txm->LoadTexture(context.renderer, "../../assets/nature/rock/rock5.png");
		sprite->Initialize(tex, 65, 43, 0, 0, width, height * 0.9, RenderLayer::NATURE);
		break;
	case(6):
		tex = context.txm->LoadTexture(context.renderer, "../../assets/nature/rock/rock6.png");
		sprite->Initialize(tex, 69, 43, 0, 0, width, height * 0.9, RenderLayer::NATURE);
		break;
	}
	n->Initialize(sprite, NatureType::ROCK, true);
	CollisionShape cs = CollisionShape::MakeAABB(width, height * 0.5, Vector2(0, height * 0.4));
	n->SetCollisionBound(cs);

	return n;
}

static Nature* GetRandomStump(GameContext& context, int cellSize) {
	int i = stumpSpriteGen(gen);

	double scaler = stumpScaler(gen);
	int width = cellSize * widthGen(gen) * scaler;
	int height = cellSize * heightGen(gen) * scaler;

	SDL_Texture* tex;
	Sprite* sprite = new Sprite();
	Nature* n = new Nature();
	CollisionShape cs;
	switch (i) {
		case(1):
			tex = context.txm->LoadTexture(context.renderer, "../../assets/nature/stump/stump1.png");
			sprite->Initialize(tex, 66, 84, 0, 0, width, height, RenderLayer::NATURE);
			cs = CollisionShape::MakeAABB(width, height * 0.3, Vector2(0, height * 0.7));
			break;
		case(2):
			tex = context.txm->LoadTexture(context.renderer, "../../assets/nature/stump/stump2.png");
			sprite->Initialize(tex, 121, 51, 0, 0, width * 1.5, height, RenderLayer::NATURE);
			cs = CollisionShape::MakeAABB(width * 1.5, height * 0.7, Vector2(0, height * 0.3));
			break;
	}
	n->Initialize(sprite, NatureType::STUMP, true);

	n->SetCollisionBound(cs);

	return n;
}

static Nature* GetRandomBush(GameContext& context, int cellSize) {
	int i = bushSpriteGen(gen);

	double scaler = bushScaler(gen);
	int width = cellSize * widthGen(gen) * scaler;
	int height = cellSize * heightGen(gen) * scaler;

	SDL_Texture* tex;
	Sprite* sprite = new Sprite();
	Nature* n = new Nature();
	CollisionShape cs;
	switch (i) {
	case(1):
		tex = context.txm->LoadTexture(context.renderer, "../../assets/nature/bush/bush1.png");
		sprite->Initialize(tex, 104, 72, 0, 0, width, height, RenderLayer::NATURE);
		cs = CollisionShape::MakeAABB(width, height * 0.5, Vector2(0, height * 0.5));
		break;
	case(2):
		tex = context.txm->LoadTexture(context.renderer, "../../assets/nature/bush/bush2.png");
		sprite->Initialize(tex, 112, 73, 0, 0, width, height, RenderLayer::NATURE);
		cs = CollisionShape::MakeAABB(width, height * 0.5, Vector2(0, height * 0.5));
		break;
	case(3):
		tex = context.txm->LoadTexture(context.renderer, "../../assets/nature/bush/bush3.png");
		sprite->Initialize(tex, 135, 49, 0, 0, width, height, RenderLayer::NATURE);
		cs = CollisionShape::MakeAABB(width, height * 0.8, Vector2(0, height * 0.2));
		break;
	case(4):
		tex = context.txm->LoadTexture(context.renderer, "../../assets/nature/bush/bush4.png");
		sprite->Initialize(tex, 88, 77, 0, 0, width, height, RenderLayer::NATURE);
		cs = CollisionShape::MakeAABB(width, height * 0.5, Vector2(0, height * 0.5));
		break;
	}
	n->Initialize(sprite, NatureType::BUSH, true);
	
	n->SetCollisionBound(cs);

	return n;
}
static Nature* GetRandomFoliage(GameContext& context, int cellSize) {
	int i = foliageSpriteGen(gen);

	int width = cellSize * widthGen(gen) * 0.5;
	int height = cellSize * heightGen(gen) * 0.5;

	SDL_Texture* tex;
	Sprite* sprite = new Sprite();
	Nature* n = new Nature();
	switch (i) {
	case(1):
		tex = context.txm->LoadTexture(context.renderer, "../../assets/nature/foliage/foliage1.png");
		sprite->Initialize(tex, 48, 55, 0, 0, width, height, RenderLayer::NATURE);
		break;
	case(2):
		tex = context.txm->LoadTexture(context.renderer, "../../assets/nature/foliage/foliage2.png");
		sprite->Initialize(tex, 63, 69, 0, 0, width, height, RenderLayer::NATURE);
		break;
	case(3):
		tex = context.txm->LoadTexture(context.renderer, "../../assets/nature/foliage/foliage3.png");
		sprite->Initialize(tex, 67, 59, 0, 0, width, height, RenderLayer::NATURE);
		break;
	case(4):
		tex = context.txm->LoadTexture(context.renderer, "../../assets/nature/foliage/foliage4.png");
		sprite->Initialize(tex, 53, 62, 0, 0, width, height, RenderLayer::NATURE);
		break;
	case(5):
		tex = context.txm->LoadTexture(context.renderer, "../../assets/nature/foliage/foliage5.png");
		sprite->Initialize(tex, 46, 80, 0, 0, width, height, RenderLayer::NATURE);
		break;
	case(6):
		tex = context.txm->LoadTexture(context.renderer, "../../assets/nature/foliage/foliage6.png");
		sprite->Initialize(tex, 55, 62, 0, 0, width, height, RenderLayer::NATURE);
		break;
	case(7):
		tex = context.txm->LoadTexture(context.renderer, "../../assets/nature/foliage/foliage7.png");
		sprite->Initialize(tex, 51, 56, 0, 0, width, height, RenderLayer::NATURE);
		break;
	}
	n->Initialize(sprite, NatureType::FOLIAGE, false);
	CollisionShape cs = CollisionShape::MakeAABB(width, height);
	n->SetCollisionBound(cs);

	return n;
}

#endif