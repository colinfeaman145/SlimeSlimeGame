#include "Resource.hpp"

static void SpawnDrops(ResourceDrop drop, int dropPickupRadius, GameContext& context) {
	int numDrops = drop.amount;
	SDL_Texture* tex = nullptr;
	Sprite* spr = new Sprite();
	switch (drop.type) {
		case(ResourceType::COIN):
			tex = context.txm->LoadTexture(context.renderer, "../../assets/coin.png");
			spr->Initialize(tex, 2195, 2195, 0, 0, context.gridCellSize * 0.25, context.gridCellSize * 0.25);
			break;
		case(ResourceType::GOLD):
			tex = context.txm->LoadTexture(context.renderer, "../../assets/gold.png");
			spr->Initialize(tex, 302, 183, 0, 0, context.gridCellSize * 0.25, context.gridCellSize * 0.25);
			break;
		case(ResourceType::WOOD):
			tex = context.txm->LoadTexture(context.renderer, "../../assets/log.png");
			spr->Initialize(tex, 410, 261, 0, 0, context.gridCellSize * 0.25, context.gridCellSize * 0.25);
			break;
		case(ResourceType::STONE):
			tex = context.txm->LoadTexture(context.renderer, "../../assets/stone.png");
			spr->Initialize(tex, 2195, 2195, 0, 0, context.gridCellSize * 0.25, context.gridCellSize * 0.25);
			break;
	}
	for (int i = 0; i < numDrops; ++i) {
		Resource* r = new Resource();
		Sprite* s = spr->Clone();
		r->Initialize();
	}
}