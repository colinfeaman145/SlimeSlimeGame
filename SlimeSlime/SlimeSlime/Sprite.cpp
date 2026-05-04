#include "Sprite.hpp"

Sprite::Sprite() {
    texture = nullptr;
    srcRect = { 0, 0, 0, 0 };
    dstRect = { 0, 0, 0, 0 };
    rotation = 0.0f;
    color = struct color(255, 255, 255);
    alpha = 255;
}

Sprite::Sprite(const Sprite& other) {
    texture = other.texture;  
    color = other.color;
    alpha = other.alpha;
    srcRect = other.srcRect;
    dstRect = other.dstRect;
    rotation = other.rotation;
    layer = other.layer;
    subLayer = other.subLayer;
}

Sprite* Sprite::Clone() const {
    return new Sprite(*this); 
}

Sprite::~Sprite() {}

//srcRect source size and location
// srcX/Y is top left corner
//destRect where to draw and how big
bool Sprite::Initialize(SDL_Texture* tex, int srcWidth, int srcHeight, int srcX, int srcY, int drawWidth, int drawHeight, RenderLayer l, int sl) {
    texture = tex;

    srcRect.x = srcX;
    srcRect.y = srcY;
    srcRect.w = srcWidth;
    srcRect.h = srcHeight;

    dstRect.w = drawWidth;
    dstRect.h = drawHeight;

    layer = l;
    subLayer = sl;

    return true;
}

void Sprite::Process(float deltaTime, GameContext& context) {}

//draw using renderer
void Sprite::Draw(Renderer* renderer) {
    if (texture) {
        renderer->DrawTexture(texture, &srcRect, &dstRect, color, alpha, rotation, layer, subLayer);
    }
}

void Sprite::SetPosition(int x, int y) {
    dstRect.x = x;
    dstRect.y = y;
}

void Sprite::SetPosition(Vector2 vec) {
    dstRect.x = vec.x;
    dstRect.y = vec.y;
}

void Sprite::SetRotation(float angle) {
    rotation = angle;
}

void Sprite::SetColor(struct color c) {
    color = c;
}

void Sprite::SetAlpha(int a) {
    alpha = a;
}

void Sprite::SetDrawSize(int w, int h) {
    dstRect.w = w;
    dstRect.h = h;
}

void Sprite::SetDrawLayer(RenderLayer l, int sl) {
    layer = l;
    subLayer = sl;
}
