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
    context.renderer->Initialize("GameName", WIDTH, HEIGHT, false);
    context.txm = new TextureManager();
    context.fm = new FontManager();
    context.am = new AudioManager();
    context.am->Initialize({0, 0, 0 });
    context.im = new InputManager();
    context.im->Initialize("../../data/inputs.json");

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
    pScene->Initialize(context);
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

        Process(deltaTime, context);
        Draw();
    }
}

void Game::Process(float deltaTime, GameContext& context) {

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

    if (currentScene == 0) {//World Scene
        WorldScene* scene = ((WorldScene*)scenes[currentScene]);
        // FREE CAM
            //if (context.im->IsKeyDown("move_left")) {
            //    context.renderer->cam->Move(-10.0, 0.0, deltaTime);
            //}
            //if (context.im->IsKeyDown("move_right")) {
            //    context.renderer->cam->Move(10.0, 0.0, deltaTime);
            //}
            //if (context.im->IsKeyDown("move_up")) {
            //    context.renderer->cam->Move(0.0, -10.0, deltaTime);
            //}
            //if (context.im->IsKeyDown("move_down")) {
            //    context.renderer->cam->Move(0.0, 10.0, deltaTime);
            //}
        if (context.im->IsKeyDown("move_left")) {
            scene->MovePlayer(MovementDir::WEST, deltaTime);
        }
        if (context.im->IsKeyDown("move_right")) {
            scene->MovePlayer(MovementDir::EAST, deltaTime);
        }
        if (context.im->IsKeyDown("move_up")) {
            scene->MovePlayer(MovementDir::NORTH, deltaTime);
        }
        if (context.im->IsKeyDown("move_down")) {
            scene->MovePlayer(MovementDir::SOUTH, deltaTime);
        }
        if (context.im->IsKeyPressed("build_mode")) {
            scene->ToggleBuildMode();
        }
        if (context.im->IsKeyPressed("trap")) {
            scene->ChangeStructure(3, context);
        }
        if (context.im->IsKeyPressed("horizontal_wall")) {
            scene->ChangeStructure(2, context);
        }
        if (context.im->IsKeyPressed("vertical_wall")) {
            scene->ChangeStructure(1, context);
        }
        if (context.im->IsMouseButtonPressed(1)) {
            scene->PlaceStructure(context);
        }
        if (context.im->IsMouseButtonPressed(3)) {
            scene->RemoveStructure(context);
        }
        //update Camera zoom
        context.renderer->cam->AdjustZoom(context.im->GetScrollDelta() * 0.1);

        Vector2 pos = context.im->GetMouseWorldPosition(context.renderer->cam);
        scene->UpdateCurrentHoveredCell(context);

        
    }


    scenes[currentScene]->Process(context, deltaTime);
    context.am->Update();//play sounds
}

void Game::Draw()
{
    context.renderer->BeginFrame();
    scenes[currentScene]->Draw(context.renderer);
    context.renderer->EndFrame();
}

void Game::Cleanup() 
{}