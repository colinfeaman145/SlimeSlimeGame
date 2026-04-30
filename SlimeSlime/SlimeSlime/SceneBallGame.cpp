#include "SceneBallGame.hpp"

SceneBallGame::SceneBallGame() : m_time(0.0f) {
}

SceneBallGame::~SceneBallGame() {
}

bool SceneBallGame::Initialise(SDL_Renderer* renderer) {
    return true;
}

void SceneBallGame::Process(float deltaTime) {
    m_time += deltaTime;
}

void SceneBallGame::Draw(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
}