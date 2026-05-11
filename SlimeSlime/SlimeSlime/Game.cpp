#include <SDL.h>
#include <SDL_image.h>
#include "game.hpp"
#include "WorldScene.hpp"

/*
CALL PIPELINE
Main makes game
Game makes renderer and scenes
Each frame, game draws current scene
Scene draws out sprites
Sprites call drawTexture to renderer
*/

Game::Game() {
    //define context
    context.renderer = new Renderer();
    context.renderer->Initialize("Slime Slime Game", WIDTH, HEIGHT, false);
    context.txm = new TextureManager();
    context.fm = new FontManager();
    context.grid = new Grid(15000, 10000, 100);
    SDL_Texture* grassTex = context.txm->LoadTexture(context.renderer, "../../assets/grass.png");
    context.grid->Initialize(grassTex);
    context.am = new AudioManager();
    context.am->Initialize({0, 0, 0 });
    context.im = new InputManager();
    context.im->Initialize("../../data/inputs.json");
    context.gameDifficulty = (int)Difficulty::NORMAL;

    currentScene = 0;
    running = true;

}

Game::~Game() {
    for (size_t i = 0; i < scenes.size(); ++i) {
        delete scenes[i];
        scenes[i] = nullptr;
    }
    scenes.clear();

    IMG_Quit();
}

//add scenes to game
bool Game::Initialize() {
    Scene* pScene;
    Scene* pScene2;

    // scene 1
    pScene = new WorldScene();
    pScene->Initialize();
    scenes.push_back(pScene);
    //// scene 2
    //pScene2 = new WorldScene();
    //pScene2->Initialize(context);
    //scenes.push_back(pScene2);

    return true;
}

void Game::Run() {

    Uint32 lastTick = SDL_GetTicks();

    while (running) {
        Uint32 current = SDL_GetTicks();
        float deltaTime = (current - lastTick) / 1000.0f;
        lastTick = current;

        Process(deltaTime);
        Draw();
    }
}

void Game::Process(float deltaTime) {

    context.im->Process();//process inputs
    SDL_Event event;
   
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
        }

        context.im->HandleEvent(event);
        if(context.im->IsKeyDown("change_scene")){
            currentScene++;
            if (currentScene >= (int)scenes.size()) {
                currentScene = 0;
            }
        }
    }

    scenes[currentScene]->Process(deltaTime);
}

void Game::Draw()
{
    context.renderer->BeginFrame();
    scenes[currentScene]->Draw(context.renderer);
    context.renderer->EndFrame();
}

void Game::Cleanup() 
{}