#ifndef GAMECONTEXT_HPP
#define GAMECONTEXT_HPP

#include "Renderer.hpp"
#include "TextureManager.hpp"
#include "FontManager.hpp"
#include "AudioManager.hpp"
#include "InputManager.hpp"

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
};

#endif