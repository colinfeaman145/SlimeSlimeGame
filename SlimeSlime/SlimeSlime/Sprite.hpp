#ifndef SPRITE_HPP
#define SPRITE_HPP

#include <SDL.h>
#include "Renderer.hpp"
#include "GameContext.hpp"

class Sprite {
public:
    Sprite();
    Sprite(const Sprite& other);
    virtual Sprite* Clone() const;
    virtual ~Sprite();

    bool Initialize(SDL_Texture* tex, int srcWidth, int srcHeight, int srcX, int srcY, int drawWidth, int drawHeight, RenderLayer l = RenderLayer::OBJECTS, int sl = 0);

    virtual void Process(float deltaTime, GameContext& context);
    virtual void Draw(Renderer* renderer);

    void SetPosition(int x, int y);
    void SetPosition(Vector2 vec);
    void SetRotation(float angle);
    void SetColor(color c);
    void SetAlpha(int a);
    void SetDrawSize(int w, int h);
    void SetDrawLayer(RenderLayer l, int sl = 0);
    int GetWidth() const { return dstRect.w; };
    Vector2 GetPosition() const { return Vector2(dstRect.x, dstRect.y); }
    Vector2 GetDrawSize() const { return Vector2(dstRect.w, dstRect.h); }
    float GetRotation() const { return rotation; }
    color GetColor() const { return color; }
    float GetAlpha() const { return alpha; }

protected: 
    SDL_Texture* texture;
    color color;
    float alpha;
    RenderLayer layer;
    int subLayer;

    SDL_Rect srcRect;
    SDL_Rect dstRect;

    float rotation;
};

#endif