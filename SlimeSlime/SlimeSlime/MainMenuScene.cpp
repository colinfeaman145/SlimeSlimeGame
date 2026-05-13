#include "MainMenuScene.hpp"
#include "EnemySpawner.hpp"

bool MainMenu::Initialize() {

	context.gameDifficulty = (int)Difficulty::NORMAL;

	SDL_Texture* logoTex = context.txm->LoadTexture(context.renderer, "../../assets/logo.png");
	Sprite* logo = new Sprite();
	logo->Initialize(logoTex, 263, 87, 0, 0, WIDTH * 0.6, HEIGHT * 0.25, RenderLayer::UI);
	logo->SetPosition(WIDTH * 0.01, HEIGHT * 0.1);
	UI.push_back(logo);

	Button* start = new Button(WIDTH * 0.065, HEIGHT * 0.38, WIDTH * 0.2, HEIGHT * 0.1,
		{ 90, 90, 90 }, { 220, 120, 0 }, { 70, 70, 70 }, { 200, 100, 0 }, 3,
		[]() { context.changeScene(2);}, 1.1, 0);
	Text* startText = new Text();
	startText->Initialize("Play", "../../fonts/pixelFont.ttf", HEIGHT * 0.08);
	start->SetImage(startText);
	UI.push_back(start);

	Button* difficulty = new Button(WIDTH * 0.01, HEIGHT * 0.52, WIDTH * 0.6, HEIGHT * 0.1,
		{ 90, 90, 90 }, { 220, 120, 0 }, { 70, 70, 70 }, { 200, 100, 0 }, 3,
		[this]() { this->ChangeDifficulty();}, 1.07, 0);
	difficultyText = new Text();
	difficultyText->Initialize("Difficulty: NORMAL", "../../fonts/pixelFont.ttf", HEIGHT * 0.08);
	difficulty->SetImage(difficultyText);
	UI.push_back(difficulty);

	Button* howToPlay = new Button(WIDTH * 0.045, HEIGHT * 0.66, WIDTH * 0.4, HEIGHT * 0.1,
		{ 90, 90, 90 }, { 220, 120, 0 }, { 70, 70, 70 }, { 200, 100, 0 }, 3,
		[]() { context.changeScene(1);}, 1.07, 0);
	Text* howToPlayText = new Text();
	howToPlayText->Initialize("How To Play", "../../fonts/pixelFont.ttf", HEIGHT * 0.08);
	howToPlay->SetImage(howToPlayText);
	UI.push_back(howToPlay);
	
	Button* quit = new Button(WIDTH * 0.04, HEIGHT * 0.8, WIDTH * 0.24, HEIGHT * 0.1,
		{ 90, 90, 90 }, { 220, 120, 0 }, { 70, 70, 70 }, { 200, 100, 0 }, 3,
		[]() { context.changeScene(-1);}, 1.1, 0);
	Text* quitText = new Text();
	quitText->Initialize("Quit", "../../fonts/pixelFont.ttf", HEIGHT * 0.08);
	quit->SetImage(quitText);
	UI.push_back(quit);


	//grid init
	grid = new Grid(WIDTH, HEIGHT, 150);
	SDL_Texture* grassTex = context.txm->LoadTexture(context.renderer, "../../assets/grass.png");
	grid->Initialize(grassTex);
	elements.push_back(grid);

	//audio init bruv
	context.am = new AudioManager();
	context.am->Initialize({ 0, 0, 0 }, 1);
	context.am->LoadMusicTrack("../../assets/sounds/ambient.mp3", "Ambient");
	context.am->LoadMusicTrack("../../assets/sounds/background_music1.mp3", "Song1");
	context.am->LoadMusicTrack("../../assets/sounds/background_music2.mp3", "Song2");
	context.am->LoadMusicTrack("../../assets/sounds/background_music2.mp3", "Song3");

	context.renderer->cam->Follow(Vector2(WIDTH / 2, HEIGHT / 2));

	//spawn ambient enemies
	EnemySpawner* spawner = new EnemySpawner();
	spawner->Initialize("../../data/enemyStats.txt", "../../data/spawnPools.json");
	elements.push_back(spawner);

	return true;
}

void MainMenu::Process(float deltaTime) {

	context.am->Process(Vector2(500, 500), deltaTime);
	context.renderer->cam->SetZoom(1);
	context.renderer->cam->Follow(Vector2(WIDTH / 2, HEIGHT / 2));

	for (Element* e : elements) {
		e->Process(deltaTime);
	}
	for (Sprite* s : UI) {
		s->Process(deltaTime);
	}
}

void MainMenu::Draw(Renderer* renderer) {
	for (Element* e : elements) {
		e->Draw(renderer);
	}
	for (Sprite* s : UI) {
		s->Draw(renderer);
	}
}

void MainMenu::ReadInputs(float deltaTime) {
	return;
}

void MainMenu::ChangeDifficulty() {
	if (context.gameDifficulty == (int)Difficulty::EASY) {
		context.gameDifficulty = (int)Difficulty::NORMAL;
		difficultyText->SetText("Difficulty: NORMAL");
		return;
	}
	else if (context.gameDifficulty == (int)Difficulty::NORMAL) {
		context.gameDifficulty = (int)Difficulty::HARD;
		difficultyText->SetText("Difficulty: HARD");
		return;
	}
	else if (context.gameDifficulty == (int)Difficulty::HARD) {
		context.gameDifficulty = (int)Difficulty::EASY;
		difficultyText->SetText("Difficulty: EASY");
		return;
	}
}