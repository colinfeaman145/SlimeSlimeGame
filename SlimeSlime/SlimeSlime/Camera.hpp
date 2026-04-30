#ifndef CAMERA_HPP
#define CAMERA_HPP
#include <SDL.h>
#include "Vector2.hpp"

#define WIDTH 800
#define HEIGHT 600
#define CAM_SPEED 50

class Camera {
public:
    Camera();
    void Move(float dx, float dy, float deltaTime);
    void Follow(Vector2 pos);
    void SetZoom(float z);
    void AdjustZoom(float amount);

    SDL_Rect GetScreenRect(const SDL_Rect* spriteRect);

    void SetSpeed(float spd);
    float GetX() const { return x; }
    float GetY() const { return y; }
    int GetWidth() const { return HEIGHT; }
    int GetHeight() const { return WIDTH; }
    float GetZoom() const { return zoom; }

private:
    float x, y;
    float zoom;
};

#endif