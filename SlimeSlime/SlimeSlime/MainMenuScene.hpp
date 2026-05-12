#ifndef MAIN_MENU_HPP
#define MAIN_MENU_HPP

#include "Scene.hpp"
#include "Grid.hpp"
#include "Container.hpp"
#include "Button.hpp"

using namespace std;
class MainMenu : public Scene {
    protected:
        vector<Element*> elements;
        vector<Sprite*> UI;

        Grid* grid;
        Text* difficultyText;
    public:
        MainMenu() {}
        ~MainMenu() {}

        bool Initialize() override;
        void Process(float deltaTime) override;
        void Draw(Renderer* renderer) override;

        void ReadInputs(float deltaTime) override;
        void ChangeDifficulty();

};

#endif