#ifndef FOLIAGE_HPP
#define FOLIAGE_HPP

#include "Nature.hpp"
#include "AttackCone.hpp"
#include "Structure.hpp"
#include "Grid.hpp"

class Foliage : public Nature {

	void HandleCollision(Collidable* other, Vector2 penetration, GameContext& conext) override;

};

static Foliage* GetRandomFoliage(GameContext& context) {
	int i = foliageSpriteGen(gen);

	int cellSize = context.grid->GetCellSize();
	int width = cellSize * widthGen(gen) * 0.5;
	int height = cellSize * heightGen(gen) * 0.5;

	SDL_Texture* tex;
	Sprite* sprite = new Sprite();
	Foliage* n = new Foliage();
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
	n->SetHealth(CalculateDurability(width, height, cellSize, NatureType::FOLIAGE));

	return n;
}

#endif