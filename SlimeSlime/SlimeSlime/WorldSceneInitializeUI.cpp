#include "WorldScene.hpp"

void WorldScene::InitializeUI() {

    //UI
    //wood
    int resourceSize = WIDTH * 0.04;
    SDL_Texture* woodTex = context.txm->LoadTexture(context.renderer, "../../assets/log.png");
    Sprite* woodIcon = new Sprite();
    woodIcon->Initialize(woodTex, 410, 261, 0, 0, resourceSize, resourceSize);
    woodIcon->SetDrawLayer(RenderLayer::UI);
    woodIcon->SetPosition(Vector2(10, HEIGHT * 0.02));
    UI.push_back(woodIcon);

    woodCount = new Text();
    woodCount->Initialize("0", "../../fonts/PROXON.ttf", resourceSize);
    woodCount->SetPosition(WIDTH * 0.05, HEIGHT * 0.02);
    UI.push_back(woodCount);

    //stone
    SDL_Texture* stoneTex = context.txm->LoadTexture(context.renderer, "../../assets/stone.png");
    Sprite* stoneIcon = new Sprite();
    stoneIcon->Initialize(stoneTex, 404, 334, 0, 0, resourceSize, resourceSize);
    stoneIcon->SetDrawLayer(RenderLayer::UI);
    stoneIcon->SetPosition(Vector2(10, HEIGHT * 0.07));
    UI.push_back(stoneIcon);

    stoneCount = new Text();
    stoneCount->Initialize("0", "../../fonts/PROXON.ttf", resourceSize);
    stoneCount->SetPosition(WIDTH * 0.05, HEIGHT * 0.07);
    UI.push_back(stoneCount);

    //coin
    SDL_Texture* coinTex = context.txm->LoadTexture(context.renderer, "../../assets/coin.png");
    Sprite* coinIcon = new Sprite();
    coinIcon->Initialize(coinTex, 2195, 2195, 0, 0, resourceSize, resourceSize);
    coinIcon->SetDrawLayer(RenderLayer::UI);
    coinIcon->SetPosition(Vector2(10, HEIGHT * 0.12));
    UI.push_back(coinIcon);

    coinCount = new Text();
    coinCount->Initialize("0", "../../fonts/PROXON.ttf", resourceSize);
    coinCount->SetPosition(WIDTH * 0.05, HEIGHT * 0.12);
    UI.push_back(coinCount);

    //gameOver screen
    gameOverScreen = new Container(WIDTH * 0.2, HEIGHT * 0.2, WIDTH * 0.6, HEIGHT  * 0.6, 
        { 120, 120, 120 }, { 60, 60, 60 }, 120, 5);
    Button* retryButton = new Button(0, 0, WIDTH * 0.45, HEIGHT * 0.075,
        { 90, 90, 90 }, { 220, 120, 0 }, { 70, 70, 70 }, { 200, 100, 0 }, 3,
        [this]() { context.changeScene(2); }, 1.05);
    Button* goMainMenu = new Button(0, 0, WIDTH * 0.45, HEIGHT * 0.075,
        { 90, 90, 90 }, { 220, 120, 0 }, { 70, 70, 70 }, { 200, 100, 0 }, 3,
        [this]() { context.changeScene(0); }, 1.05);
    Text* retryText = new Text();
    retryText->Initialize("Retry", "../../fonts/PROXON.ttf", WIDTH * 0.04);
    retryButton->SetImage(retryText);
    Text* mainMenuText = new Text();
    mainMenuText->Initialize("Main Menu", "../../fonts/PROXON.ttf", WIDTH * 0.04);
    goMainMenu->SetImage(mainMenuText);
    Text* gameOverText = new Text();
    gameOverText->Initialize("Game Over", "../../fonts/PROXON.ttf", WIDTH * 0.075);
    timerText = new Text();
    timerText->Initialize("You lasted 0 minutes and 0 seconds", "../../fonts/PROXON.ttf", WIDTH * 0.02);
    gameOverScreen->AddText(gameOverText, (gameOverScreen->GetWidth() - gameOverText->GetWidth()) / 2, gameOverScreen->GetHeight() * 0.15);
    gameOverScreen->AddButton(retryButton, (gameOverScreen->GetWidth() - retryButton->GetWidth()) / 2, gameOverScreen->GetHeight() * 0.5);
    gameOverScreen->AddButton(goMainMenu, (gameOverScreen->GetWidth() - goMainMenu->GetWidth()) / 2, gameOverScreen->GetHeight() * 0.7);
    UI.push_back(gameOverScreen);
    gameOverScreen->Toggle();

    //player death screen
    playerDeathScreen = new Container(WIDTH * 0.2, HEIGHT * 0.2, WIDTH * 0.6, HEIGHT * 0.6,
        { 120, 120, 120 }, { 60, 60, 60 }, 120, 5);
    Text* deathText = new Text();
    deathText->Initialize("You died", "../../fonts/PROXON.ttf", WIDTH * 0.075);
    playerDeathScreen->AddText(deathText, (playerDeathScreen->GetWidth() - deathText->GetWidth()) / 2, playerDeathScreen->GetHeight() * 0.2);
    Text* goldLostText = new Text();
    goldLostText->Initialize("Half your gold has been lost", "../../fonts/PROXON.ttf", WIDTH * 0.035);
    playerDeathScreen->AddText(goldLostText, (playerDeathScreen->GetWidth() - goldLostText->GetWidth()) / 2, playerDeathScreen->GetHeight() * 0.45);
    Text* respawnTimerLabel = new Text();
    respawnTimerLabel->Initialize("You will respawn in", "../../fonts/PROXON.ttf", WIDTH * 0.035);
    playerDeathScreen->AddText(respawnTimerLabel, (playerDeathScreen->GetWidth() - respawnTimerLabel->GetWidth()) / 2, playerDeathScreen->GetHeight() * 0.6);
    respawnTimerText = new Text();
    respawnTimerText->Initialize("20 seconds", "../../fonts/PROXON.ttf", WIDTH * 0.035);
    playerDeathScreen->AddText(respawnTimerText, (playerDeathScreen->GetWidth() - respawnTimerText->GetWidth()) / 2, playerDeathScreen->GetHeight() * 0.75);

    UI.push_back(playerDeathScreen);
    playerDeathScreen->Toggle();

    //pause screen
    pauseScreen = new Container(WIDTH * 0.2, HEIGHT * 0.2, WIDTH * 0.6, HEIGHT * 0.6,
        { 120, 120, 120 }, { 60, 60, 60 }, 120, 5);
    Button* continueButton = new Button(0, 0, WIDTH * 0.45, HEIGHT * 0.075,
        { 90, 90, 90 }, { 220, 120, 0 }, { 70, 70, 70 }, { 200, 100, 0 }, 3,
        [this]() { this->PauseGame(); }, 1.05);
    Button* goMainMenuP = new Button(0, 0, WIDTH * 0.45, HEIGHT * 0.075,
        { 90, 90, 90 }, { 220, 120, 0 }, { 70, 70, 70 }, { 200, 100, 0 }, 3,
        [this]() { context.changeScene(0); }, 1.05);
    Text* pauseText = new Text();
    pauseText->Initialize("Game Paused", "../../fonts/PROXON.ttf", WIDTH * 0.075);
    pauseScreen->AddText(pauseText, (playerDeathScreen->GetWidth() - pauseText->GetWidth()) / 2, playerDeathScreen->GetHeight() * 0.2);
    Text* continueText = new Text();
    continueText->Initialize("Continue", "../../fonts/PROXON.ttf", WIDTH * 0.04);
    continueButton->SetImage(continueText);
    Text* mainMenuTextP = new Text();
    mainMenuTextP->Initialize("Main Menu", "../../fonts/PROXON.ttf", WIDTH * 0.04);
    goMainMenuP->SetImage(mainMenuTextP);
    pauseScreen->AddButton(continueButton, (pauseScreen->GetWidth() - retryButton->GetWidth()) / 2, pauseScreen->GetHeight() * 0.5);
    pauseScreen->AddButton(goMainMenuP, (pauseScreen->GetWidth() - goMainMenuP->GetWidth()) / 2, pauseScreen->GetHeight() * 0.7);

    UI.push_back(pauseScreen);
    pauseScreen->Toggle();

    InitializeUpgradeContainer();
    InitializeStructureHUD();
}