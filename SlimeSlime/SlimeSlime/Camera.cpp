#include "Camera.hpp"

Camera::Camera(){
    x = y = 0;
    zoom = .5;
}

void Camera::Move(float dx, float dy, float deltaTime) {
    x += dx * CAM_SPEED * deltaTime;
    y += dy * CAM_SPEED * deltaTime;
}

void Camera::Follow(Vector2 pos) {
    x = pos.x - WIDTH / (2.0 * zoom);
    y = pos.y - HEIGHT / (2.0 * zoom);
}

void Camera::SetZoom(float z) {
    zoom = z;
}

void Camera::AdjustZoom(float amount) {
    zoom += amount;
    if (zoom < 0.1) zoom = 0.1; // prevent flipping
    if (zoom > 3.0) zoom = 3;
}

SDL_Rect Camera::GetScreenRect(const SDL_Rect* spriteRect) {
    return SDL_Rect{
        (int)round((spriteRect->x - x) * zoom),
        (int)round((spriteRect->y - y) * zoom),
        (int)round(spriteRect->w * zoom),
        (int)round(spriteRect->h * zoom)
    };
}