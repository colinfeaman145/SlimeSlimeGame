#include "PercentageBar.hpp"

PercentageBar::PercentageBar(float current, float max, int w, int h, color fillColor, color outlineColor)
    : current(current), max(max), w(w), h(h), x(0), y(0)
    , fillColor(fillColor), outlineColor(outlineColor) {
}

void PercentageBar::SetValues(float c, float m) {
    current = c;
    max = m;
}

void PercentageBar::SetPosition(int x, int y) {
    this->x = x;
    this->y = y;
}

void PercentageBar::SetOffset(int offX, int offY) {
    offsetX = offX;
    offsetY = offY;
}

void PercentageBar::SetFillColor(color c) { fillColor = c; }
void PercentageBar::SetOutlineColor(color c) { outlineColor = c; }

void PercentageBar::Draw(Renderer* renderer) {
    float pct = (max > 0) ? current / max : 0.0f;
    if (pct < 0.0f) pct = 0.0f;
    if (pct > 1.0f) pct = 1.0f;

    int fillWidth = (int)(w * pct);

    // fill
    if (fillWidth > 0)
        renderer->AddFilledRect(x + offsetX, y + offsetY, fillWidth, h, fillColor, 255, RenderLayer::UI);

    // outline
    renderer->AddDrawRect(x + offsetX, y + offsetY, w, h, outlineColor, 255, RenderLayer::UI);
}