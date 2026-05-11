#include "WorldScene.hpp"

void WorldScene::InitializeStructureHUD() {

	structureHUD = new Container(WIDTH * 0.01, HEIGHT * 0.84, WIDTH * 0.98, HEIGHT * 0.15,
		{ 120, 120, 120 }, { 60, 60, 60 }, 120, 5);
	structureHUD->Toggle();
	UI.push_back(structureHUD);

	int HUDColumnSize = structureHUD->GetWidth() * .125;
	
	for (int i = 0; i < 7; i++) {//8 = number of structures
		Container* c = MakeStructureCard(i + 1);
		structureCards[i] = c;
		structureHUD->AddSprite(c, structureHUD->GetWidth() * 0.0125 + (HUDColumnSize * i), structureHUD->GetWidth() * 0.0125);
	}

	//change material button
	Button* changeMaterialButton = new Button(0, 0, WIDTH * 0.25, HEIGHT * 0.075,
		{ 90, 90, 90 }, { 220, 120, 0 }, { 70, 70, 70 }, { 200, 100, 0 }, 3,
		[this]() { this->ChangeMaterial(); }, 1.05);
	Text* changeMatLabel = new Text();
	changeMatLabel->Initialize("Change Material", "../../fonts/PROXON.ttf", 20);
	changeMaterialButton->SetImage(changeMatLabel);
	structureHUD->AddButton(changeMaterialButton, 0, -(HEIGHT * 0.085));
}

Container* WorldScene::MakeStructureCard(int i) {

	Structure* structure = GetCurrentStructure(i);

	int HUDSize = structureHUD->GetWidth() * .1;
	Container* c = new Container(0, 0, HUDSize * 0.9, HUDSize * 0.9, 
		{ 0, 0, 0 }, { 0, 0, 0 }, 0, 5);
	int cWidth = c->GetWidth();
	int cHeight = c->GetHeight();
	Button* b = new Button(0, 0, HUDSize * 0.9, HUDSize * 0.9, 
		{ 90, 90, 90 }, { 220, 120, 0 }, { 70, 70, 70 }, { 200, 100, 0 }, 3,
		[this, i]() { this->ChangeStructure(i); }, 1.05);
	c->AddButton(b, 0, 0);

	Sprite* s = new Sprite(*structure->GetSprite());
	s->SetDrawSize(cWidth * 0.8, cHeight * 0.8);
	c->AddSprite(s, cWidth * 0.1, cHeight * 0.125);

	//wood icon
	int resourceSize = cWidth * 0.2;
	SDL_Texture* woodTex = context.txm->LoadTexture(context.renderer, "../../assets/log.png");
	Sprite* woodIcon = new Sprite();
	woodIcon->Initialize(woodTex, 410, 261, 0, 0, resourceSize, resourceSize);
	woodIcon->SetDrawLayer(RenderLayer::UI, 2);
	woodIcon->SetPosition(Vector2(10, 10));
	c->AddSprite(woodIcon, cWidth * 0.5, cHeight * 0.1);

	Text* woodCost = new Text();
	woodCost->Initialize(to_string(structure->GetRecipe().at(ResourceType::WOOD)), "../../fonts/PROXON.ttf", resourceSize);
	c->AddText(woodCost, cWidth * 0.75, cHeight * 0.1);

	//stone icon
	SDL_Texture* stoneTex = context.txm->LoadTexture(context.renderer, "../../assets/stone.png");
	Sprite* stoneIcon = new Sprite();
	stoneIcon->Initialize(stoneTex, 404, 334, 0, 0, resourceSize, resourceSize);
	stoneIcon->SetDrawLayer(RenderLayer::UI, 2);
	stoneIcon->SetPosition(Vector2(10, 50));
	c->AddSprite(stoneIcon, cWidth * 0.5, cHeight * 0.35);

	Text* stoneCost = new Text();
	stoneCost->Initialize(to_string(structure->GetRecipe().at(ResourceType::STONE)), "../../fonts/PROXON.ttf", resourceSize);
	c->AddText(stoneCost, cWidth * 0.75, cHeight * 0.35);

	//coin icon
	SDL_Texture* coinTex = context.txm->LoadTexture(context.renderer, "../../assets/coin.png");
	Sprite* coinIcon = new Sprite();
	coinIcon->Initialize(coinTex, 2195, 2195, 0, 0, resourceSize, resourceSize);
	coinIcon->SetDrawLayer(RenderLayer::UI, 2);
	coinIcon->SetPosition(Vector2(10, 90));
	c->AddSprite(coinIcon, cWidth * 0.5, cHeight * 0.6);

	Text* coinCost = new Text();
	coinCost->Initialize(to_string(structure->GetRecipe().at(ResourceType::COIN)), "../../fonts/PROXON.ttf", resourceSize);
	c->AddText(coinCost, cWidth * 0.75, cHeight * 0.6);


	return c;
}