#ifndef BALL_HPP
#define BALL_HPP

#include "scene.hpp"
#include <vector>

#define numBalls 50

struct Vector2D {
    double x;
    double y;
    double vx;
    double vy;
};

class Ball{
public:
    Ball();
    ~Ball();
    bool isGreen;
    SDL_Rect dest;
    Vector2D vec;

private:
    void UpdateBall();
    bool UpdateBallPosition();
    bool CheckCollision();
    void CheckBounce();
};

#endif