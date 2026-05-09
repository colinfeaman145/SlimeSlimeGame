#ifndef GAME_HPP
#define GAME_HPP

#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include "Scene.hpp"
#include "Camera.hpp"

using namespace std;
class Scene;

class Game {
public:
    Game();
    ~Game();

    bool Initialize();
    void Run();
    void Cleanup();

private:
    void Process(float deltaTime);
    void Draw();

private:
    vector<Scene*> scenes;
    int currentScene;
    bool running;
};

#endif