#ifndef ELEMENT_HPP
#define ELEMENT_HPP
#include "Vector2.hpp"
#include "Renderer.hpp"
#include "Collidable.hpp"

#define DEBUGMODE false
//VIRTUAL CLASS
class Element {
	public:
		virtual void Draw(Renderer* renderer) = 0;
		virtual void Process(float deltaTime, GameContext& context) = 0;
};

//calls draw for:
	//Entity > Sprite
	//ParticleEmitter > Particle > Sprite
	//Text > Sprite 

#endif