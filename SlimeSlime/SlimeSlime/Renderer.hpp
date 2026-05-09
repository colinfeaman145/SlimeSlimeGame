#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <SDL.h>
#include <string>
#include "Camera.hpp"
#include <vector>
#include "ShapeType.hpp"

struct color {
    float r;
    float g;
    float b;
};

enum class RenderLayer {
    GROUND = 0,
    STRUCTURES = 1,
    OBJECTS = 2,
    ENEMIES = 3,
    ATTACK_CONE = 4,
    PLAYER = 5,
    NATURE = 6,
    PARTICLE = 7,
    UI = 8,
    DEBUG = 9
};

struct RenderCommand {
    SDL_Texture* texture;
    SDL_Rect srcRect;
    SDL_Rect dstRect;
    float rotation;
    RenderLayer layer;
    int subOrder;
    Uint8 r, g, b, a;
    SDL_RendererFlip flip;
    bool blendMode;
};

struct BoundingDrawCommand {
    ShapeType type;
    float x, y, w, h;
    float radius;
    Vector2 direction;
    float halfAngle;
    color c;
    int alpha;
    RenderLayer layer;
};

using namespace std;
class Renderer {
public:
    Renderer();
    ~Renderer();
    Camera* cam;

    bool Initialize(const string& windowTitle, int width, int height, bool fullscreen = false);
    void BeginFrame();
    void EndFrame();

    // draw a texture
    void DrawTexture(SDL_Texture* texture, SDL_Rect* srcRect, SDL_Rect* dstRect, color c, float a, int rotation = 0, 
        RenderLayer layer = RenderLayer::OBJECTS, int subOrder = 0, bool bm = false, SDL_RendererFlip f = SDL_FLIP_NONE);
    void AddDrawRect(float x, float y, float w, float h, color c, int a, RenderLayer layer);
    void AddDrawCircle(float cx, float cy, float radius, color c, int a, RenderLayer layer);
    void AddDrawCone(float cx, float cy, float radius, Vector2 direction, float halfAngle, color c, int a, RenderLayer layer);
    void AddFilledRect(float x, float y, float w, float h, color c, int a, RenderLayer layer);

    SDL_Renderer* GetSDLRenderer() const { return renderer; }

private:
    SDL_Window* window;
    SDL_Renderer* renderer;

    vector<RenderCommand> renderQueue;
    vector<BoundingDrawCommand> boundingQueue;

    inline static bool CompareRenderCommands(const RenderCommand& a, const RenderCommand& b) {
        if (a.layer != b.layer)
            return static_cast<int>(a.layer) < static_cast<int>(b.layer);
        return a.subOrder < b.subOrder;
    }

    inline static bool CompareBoundingCommands(const BoundingDrawCommand& a, const BoundingDrawCommand& b) {
        return static_cast<int>(a.layer) < static_cast<int>(b.layer);
    }

    void DrawSingleRenderCommand(const RenderCommand& cmd);
    void DrawSingleBoundingCommand(const BoundingDrawCommand& cmd);
       
    void DrawBoundingRect(int x, int y, int w, int h);
    void DrawBoundingCircle(int cx, int cy, int radius);
    void DrawBoundingCone(int cx, int cy, int radius, Vector2 direction, float halfAngle);
    void DrawFilledRect(int x, int y, int w, int h, color c, int a);
};

#endif