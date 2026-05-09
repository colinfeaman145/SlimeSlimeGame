#ifndef GAMECONTEXT_HPP
#define GAMECONTEXT_HPP

#include <random>
#include "Renderer.hpp"
#include "TextureManager.hpp"
#include "FontManager.hpp"
#include "AudioManager.hpp"
#include "InputManager.hpp"

class Grid;

//class Renderer;
//class TextureManager;
//class FontManager;
//class AudioManager;


struct GameContext {
    Renderer* renderer;
    TextureManager* txm;
    FontManager* fm;
    AudioManager* am;
    InputManager* im;
    Grid* grid;
    float gameProgress;
};

inline GameContext context;
inline mt19937 gen(random_device{}());

#endif