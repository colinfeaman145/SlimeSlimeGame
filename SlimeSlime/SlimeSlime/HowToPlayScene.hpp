#ifndef HOW_TO_PLAY_HPP
#define HOW_TO_PLAY_HPP

#include "Scene.hpp"
#include "Container.hpp"
#include "Button.hpp"

using namespace std;
class HowToPlay : public Scene {
protected:
    vector<Element*> elements;
    vector<Sprite*> UI;
public:
    HowToPlay() {}
    ~HowToPlay() {}

    bool Initialize() override;
    void Process(float deltaTime) override;
    void Draw(Renderer* renderer) override;

    void ReadInputs(float deltaTime) override;
};

#endif