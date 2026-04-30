#ifndef SCENEBALLGAME_HPP
#define SCENEBALLGAME_HPP

#include "scene.hpp"
#include <vector>

#define numBalls 50

struct Vector2D {
    double x;
    double y;
    double vx;
    double vy;
};
struct ball {
    bool isGreen;
    int size;
    Vector2D vec;
};

class SceneBallGame : public Scene {
public:
    SceneBallGame();
    virtual ~SceneBallGame();

    virtual bool Initialise(Renderer* renderer);
    virtual void Process(float deltaTime);
    virtual void Draw(Renderer* renderer);

private:
    float m_time;
    int playerSize;
    vector<ball*> balls;

    void UpdateBall(ball* b);
    void UpdateBallPosition(ball* b);
    bool CheckCollision(ball* b);


};

#endif