#include "HowToPlayScene.hpp"
#include "EnemySpawner.hpp"

bool HowToPlay::Initialize() {

	Button* back = new Button(WIDTH * 0.02, HEIGHT * 0.02, WIDTH * 0.2, HEIGHT * 0.08,
		{ 90, 90, 90 }, { 220, 120, 0 }, { 70, 70, 70 }, { 200, 100, 0 }, 3,
		[]() { context.changeScene(0);}, 1.1, 255);
	Text* backText = new Text();
	backText->Initialize("<- Back", "../../fonts/pixelFont.ttf", WIDTH * 0.05);
	back->SetImage(backText);
	UI.push_back(back);

	int headerSize = WIDTH * 0.075;
	int textSize = WIDTH * 0.035;

	//goal Label
	Text* goalLabel = new Text();
	goalLabel->Initialize("Goal", "../../fonts/pixelFont.ttf", headerSize);
	goalLabel->SetFontStyle(TTF_STYLE_UNDERLINE);
	goalLabel->SetPosition(WIDTH * 0.05, HEIGHT * 0.12);
	UI.push_back(goalLabel);

	//goal 1
	Text* goal1 = new Text();
	goal1->Initialize("*Defend the Atlas!", "../../fonts/pixelFont.ttf", textSize);
	goal1->SetPosition(WIDTH * 0.065, HEIGHT * 0.22);
	UI.push_back(goal1);
	//goal 2
	Text* goal2 = new Text();
	goal2->Initialize("*Collect resources and build defenses", "../../fonts/pixelFont.ttf", textSize);
	goal2->SetPosition(WIDTH * 0.065, HEIGHT * 0.27);
	UI.push_back(goal2);
	//goal 3
	Text* goal3 = new Text();
	goal3->Initialize("*Upgrade and prepare for the horde", "../../fonts/pixelFont.ttf", textSize);
	goal3->SetPosition(WIDTH * 0.065, HEIGHT * 0.32);
	UI.push_back(goal3);

	//controls Label
	Text* controlLabel = new Text();
	controlLabel->Initialize("Controls", "../../fonts/pixelFont.ttf", headerSize);
	controlLabel->SetFontStyle(TTF_STYLE_UNDERLINE);
	controlLabel->SetPosition(WIDTH * 0.05, HEIGHT * 0.38);
	UI.push_back(controlLabel);

	//control 1
	Text* control1 = new Text();
	control1->Initialize("*WASD or Arrow keys: Move", "../../fonts/pixelFont.ttf", textSize);
	control1->SetPosition(WIDTH * 0.065, HEIGHT * 0.48);
	UI.push_back(control1);
	//control 2
	Text* control2 = new Text();
	control2->Initialize("*Click: Place build/ Attack/ Select build", "../../fonts/pixelFont.ttf", textSize);
	control2->SetPosition(WIDTH * 0.065, HEIGHT * 0.53);
	UI.push_back(control2);
	//control 2.5
	Text* control2h = new Text();
	control2h->Initialize("*Left Click: Remove building", "../../fonts/pixelFont.ttf", textSize);
	control2h->SetPosition(WIDTH * 0.065, HEIGHT * 0.58);
	UI.push_back(control2h);
	//control 3
	Text* control3 = new Text();
	control3->Initialize("*Scroll: Zoom", "../../fonts/pixelFont.ttf", textSize);
	control3->SetPosition(WIDTH * 0.065, HEIGHT * 0.63);
	UI.push_back(control3);
	//control 4
	Text* control4 = new Text();
	control4->Initialize("*'F' or 'B': Build mode", "../../fonts/pixelFont.ttf", textSize);
	control4->SetPosition(WIDTH * 0.065, HEIGHT * 0.68);
	UI.push_back(control4);
	//control 5
	Text* control5 = new Text();
	control5->Initialize("*'R' or 'U': Upgrade menu", "../../fonts/pixelFont.ttf", textSize);
	control5->SetPosition(WIDTH * 0.065, HEIGHT * 0.73);
	UI.push_back(control5);
	//control 6
	Text* control6 = new Text();
	control6->Initialize("*'C': Change wall material", "../../fonts/pixelFont.ttf", textSize);
	control6->SetPosition(WIDTH * 0.065, HEIGHT * 0.78);
	UI.push_back(control6);
	//control 7
	Text* control7 = new Text();
	control7->Initialize("*Numbers: Change trap", "../../fonts/pixelFont.ttf", textSize);
	control7->SetPosition(WIDTH * 0.065, HEIGHT * 0.83);
	UI.push_back(control7);
	//control 8
	Text* control8 = new Text();
	control8->Initialize("*Click 1 or 2 to rotate", "../../fonts/pixelFont.ttf", textSize);
	control8->SetPosition(WIDTH * 0.085, HEIGHT * 0.88);
	UI.push_back(control8);

	//slime types
	EnemySpawner* spawner = new EnemySpawner();
	spawner->InitSprites();
	int slimeDrawSize = WIDTH * 0.2;

	//slime type label
	Text* slimesLabel = new Text();
	slimesLabel->Initialize("Slimes:", "../../fonts/pixelFont.ttf", headerSize);
	slimesLabel->SetFontStyle(TTF_STYLE_UNDERLINE);
	slimesLabel->SetPosition(WIDTH * 0.61, HEIGHT * 0.12);
	UI.push_back(slimesLabel);

	//slime 1
	AnimatedSprite* slime1 = spawner->GetSprite(EnemyType::WALL_FOCUS);
	slime1->Animate();
	slime1->SetDrawLayer(RenderLayer::UI);
	slime1->SetDrawSize(slimeDrawSize, slimeDrawSize);
	slime1->SetPosition(WIDTH * 0.56, HEIGHT * 0.11);
	UI.push_back(slime1);
	Text* slimeText1 = new Text();
	slimeText1->Initialize("*Likes to attack walls", "../../fonts/pixelFont.ttf", textSize);
	slimeText1->SetPosition(WIDTH * 0.68, HEIGHT * 0.24);
	UI.push_back(slimeText1);
	//slime 2
	AnimatedSprite* slime2 = spawner->GetSprite(EnemyType::NORMAL);
	slime2->Animate();
	slime2->SetDrawLayer(RenderLayer::UI);
	slime2->SetDrawSize(slimeDrawSize, slimeDrawSize);
	slime2->SetPosition(WIDTH * 0.56, HEIGHT * 0.18);
	UI.push_back(slime2);
	Text* slimeText2 = new Text();
	slimeText2->Initialize("*Regular", "../../fonts/pixelFont.ttf", textSize);
	slimeText2->SetPosition(WIDTH * 0.68, HEIGHT * 0.31);
	UI.push_back(slimeText2);
	//slime 3
	AnimatedSprite* slime3 = spawner->GetSprite(EnemyType::PLAYER_FOCUS);
	slime3->Animate();
	slime3->SetDrawLayer(RenderLayer::UI);
	slime3->SetDrawSize(slimeDrawSize, slimeDrawSize);
	slime3->SetPosition(WIDTH * 0.56, HEIGHT * 0.25);
	UI.push_back(slime3);
	Text* slimeText3 = new Text();
	slimeText3->Initialize("*Likes to attack you", "../../fonts/pixelFont.ttf", textSize);
	slimeText3->SetPosition(WIDTH * 0.68, HEIGHT * 0.38);
	UI.push_back(slimeText3);
	//slime 4
	AnimatedSprite* slime4 = spawner->GetSprite(EnemyType::FAST);
	slime4->Animate();
	slime4->SetDrawLayer(RenderLayer::UI);
	slime4->SetDrawSize(slimeDrawSize, slimeDrawSize);
	slime4->SetPosition(WIDTH * 0.56, HEIGHT * 0.32);
	UI.push_back(slime4);
	Text* slimeText4 = new Text();
	slimeText4->Initialize("*Speedy", "../../fonts/pixelFont.ttf", textSize);
	slimeText4->SetPosition(WIDTH * 0.68, HEIGHT * 0.45);
	UI.push_back(slimeText4);
	//slime 5
	AnimatedSprite* slime5 = spawner->GetSprite(EnemyType::ATLAS_FOCUS);
	slime5->Animate();
	slime5->SetDrawLayer(RenderLayer::UI);
	slime5->SetDrawSize(slimeDrawSize, slimeDrawSize);
	slime5->SetPosition(WIDTH * 0.56, HEIGHT * 0.39);
	UI.push_back(slime5);
	Text* slimeText5 = new Text();
	slimeText5->Initialize("*Strong and", "../../fonts/pixelFont.ttf", textSize);
	slimeText5->SetPosition(WIDTH * 0.68, HEIGHT * 0.52);
	UI.push_back(slimeText5);
	Text* slimeText5h = new Text();
	slimeText5h->Initialize("   likes to attack Atlas", "../../fonts/pixelFont.ttf", textSize);
	slimeText5h->SetPosition(WIDTH * 0.68, HEIGHT * 0.57);
	UI.push_back(slimeText5h);
	//slime 6
	AnimatedSprite* slime6 = spawner->GetSprite(EnemyType::EXPLOSIVE);
	slime6->Animate();
	slime6->SetDrawLayer(RenderLayer::UI);
	slime6->SetDrawSize(slimeDrawSize, slimeDrawSize);
	slime6->SetPosition(WIDTH * 0.56, HEIGHT * 0.51);
	UI.push_back(slime6);
	Text* slimeText6 = new Text();
	slimeText6->Initialize("*Explodes sometimes", "../../fonts/pixelFont.ttf", textSize);
	slimeText6->SetPosition(WIDTH * 0.68, HEIGHT * 0.64);
	UI.push_back(slimeText6);
	//slime 7
	AnimatedSprite* slime7 = spawner->GetSprite(EnemyType::RANDOM);
	slime7->Animate();
	slime7->SetDrawLayer(RenderLayer::UI);
	slime7->SetDrawSize(slimeDrawSize, slimeDrawSize);
	slime7->SetPosition(WIDTH * 0.56, HEIGHT * 0.58);
	UI.push_back(slime7);
	Text* slimeText7 = new Text();
	slimeText7->Initialize("???", "../../fonts/pixelFont.ttf", textSize);
	slimeText7->SetPosition(WIDTH * 0.68, HEIGHT * 0.71);
	UI.push_back(slimeText7);

	//other
	Text* other1 = new Text();
	other1->Initialize("*Hint:Close/frozen targets", "../../fonts/pixelFont.ttf", textSize);
	other1->SetPosition(WIDTH * 0.63, HEIGHT * 0.81);
	other1->SetColor({ 20, 20, 55 });
	UI.push_back(other1);
	Text* other2 = new Text();
	other2->Initialize("are more vulnerable", "../../fonts/pixelFont.ttf", textSize);
	other2->SetPosition(WIDTH * 0.67, HEIGHT * 0.85);
	other2->SetColor({ 20, 20, 55 });
	UI.push_back(other2);

	Text* other3 = new Text();
	other3->Initialize("*Hint:Enemies drop more gold", "../../fonts/pixelFont.ttf", textSize);
	other3->SetPosition(WIDTH * 0.25, HEIGHT * 0.05);
	other3->SetColor({ 20, 20, 55 });
	UI.push_back(other3);
	Text* other4 = new Text();
	other4->Initialize("when killed by you", "../../fonts/pixelFont.ttf", textSize);
	other4->SetPosition(WIDTH * 0.27, HEIGHT * 0.1);
	other4->SetColor({ 20, 20, 55 });
	UI.push_back(other4);

	return true;
}

void HowToPlay::Process(float deltaTime) {
	for (Element* e : elements) {
		e->Process(deltaTime);
	}
	for (Sprite* s : UI) {
		s->Process(deltaTime);
	}
	context.am->Process(Vector2(500, 500), deltaTime);
}

void HowToPlay::Draw(Renderer* renderer) {
	for (Element* e : elements) {
		e->Draw(renderer);
	}
	for (Sprite* s : UI) {
		s->Draw(renderer);
	}
}

void HowToPlay::ReadInputs(float deltaTime) {
	return;
}