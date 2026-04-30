#ifndef SCENE_HPP
#define SCENE_HPP

#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include "Entity.hpp"
#include "Text.hpp"
#include "Sprite.hpp"
#include "AnimatedSprite.hpp"
#include "ParticleEmitter.hpp"
#include "GameContext.hpp"

using namespace std;
class Scene {
    protected:
        vector<Element*> elements;
    public:
        Scene() {}
        virtual ~Scene() {}

        virtual bool Initialize(GameContext& context) = 0; //makes and saves sprites and entities
        virtual void Process(GameContext& context, float deltaTime) = 0;
        virtual void Draw(Renderer* renderer) = 0; 
};

#endif