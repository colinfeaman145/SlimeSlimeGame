#ifndef CAMERA_HPP
#define CAMERA_HPP
#include <SDL.h>
#include "Vector2.hpp"

inline int WIDTH = 0;
inline int HEIGHT = 0;
#define CAM_SPEED 10
#define RENDER_DISTANCE 10 

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
    int GetWidth() const { return WIDTH; }
    int GetHeight() const { return HEIGHT; }
    float GetZoom() const { return zoom; }
    SDL_Rect GetViewportWorldRect() const {
        return {
            (int)x,
            (int)y,
            (int)(WIDTH / zoom),
            (int)(HEIGHT / zoom)
        };
    }

private:
    float x, y;
    float zoom;
};

#endif