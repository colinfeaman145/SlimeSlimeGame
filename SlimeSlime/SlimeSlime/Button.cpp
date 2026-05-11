#include "Button.hpp"

Button::Button(int x, int y, int w, int h, Color fillColor, Color hoverFillColor, Color borderColor, 
    Color borderHoverColor, int borderThickness, function<void()> onClick, float hoverScale)
    : x(x), y(y), w(w), h(h)
    , fillColor(fillColor), hoverFillColor(hoverFillColor)
    , borderColor(borderColor), borderHoverColor(borderHoverColor)
    , borderThickness(borderThickness)
    , onClick(onClick)
    , isHovered(false)
    , hoverScale(hoverScale) {
    SetDrawLayer(RenderLayer::UI);
}

void Button::Process(float deltaTime) {
    if (image) image->Process(deltaTime);
    isHovered = context.im->IsMouseOver(x, y, w, h);

    if (isHovered) context.im->SetIsMouseOverUI(true);

    if (isHovered && context.im->IsMouseButtonReleased(SDL_BUTTON_LEFT)) {
        onClick();
    }
}

void Button::Draw(Renderer* renderer) {
    Color currentFill = isHovered ? hoverFillColor : fillColor;
    Color currentBorder = isHovered ? borderHoverColor : borderColor;

    int drawX = x;
    int drawY = y;
    int drawW = w;
    int drawH = h;

    if (isHovered) {
        // expand from center
        int expandW = (int)(w * hoverScale) - w;
        int expandH = (int)(h * hoverScale) - h;
        drawX -= expandW / 2;
        drawY -= expandH / 2;
        drawW += expandW;
        drawH += expandH;
    }

    //center image in button
    if (image) {
        int imgW = isHovered ? (int)(baseImageSize.x * hoverScale) : baseImageSize.x;
        int imgH = isHovered ? (int)(baseImageSize.y * hoverScale) : baseImageSize.y;
        image->SetDrawSize(imgW, imgH);

        //center inside button
        int imgX = drawX + ((drawW - imgW) / 2);
        int imgY = drawY + ((drawH - imgH) / 2);
        image->SetPosition(imgX, imgY);
        image->Draw(renderer);

    }

    //draw border as outer filled rect
    renderer->AddFilledRect(drawX, drawY, drawW, drawH, currentBorder, 255, RenderLayer::UI);

    //draw filled inset by border thickness
    renderer->AddFilledRect(
        drawX + borderThickness,
        drawY + borderThickness,
        drawW - borderThickness * 2,
        drawH - borderThickness * 2,
        currentFill, 255, RenderLayer::UI);
}

void Button::SetPosition(int x, int y) { 
    this->x = x; 
    this->y = y;
}

void Button::SetSize(int w, int h) {
    this->w = w; 
    this->h = h; 
}

void Button::SetImage(Sprite* img) {
    image = img;
    image->SetDrawLayer(RenderLayer::UITEXT);
    baseImageSize = image->GetDrawSize();
}

void Button::SetBorderColor(Color c) {
    borderColor = c;
}