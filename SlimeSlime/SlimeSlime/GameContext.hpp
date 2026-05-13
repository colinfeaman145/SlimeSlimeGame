#ifndef GAMECONTEXT_HPP
#define GAMECONTEXT_HPP

#include <random>
#include <functional>
#include "Renderer.hpp"
#include "TextureManager.hpp"
#include "FontManager.hpp"
#include "AudioManager.hpp"
#include "InputManager.hpp"

class Grid;

#define DEBUGMODE false
#define GOD_MODE false

enum class Difficulty {
    EASY = 200,
    NORMAL = 150,
    HARD = 100
};

struct GameContext {
    Renderer* renderer;
    TextureManager* txm;
    FontManager* fm;
    AudioManager* am;
    InputManager* im;
    Grid* grid;
    float gameProgress;
    int gameDifficulty;
    function<void(int)> changeScene;
};

inline GameContext context;
inline mt19937 gen(random_device{}());

#endif