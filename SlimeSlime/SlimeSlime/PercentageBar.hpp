#ifndef PERCENTAGE_BAR_HPP
#define PERCENTAGE_BAR_HPP

#include "Renderer.hpp"

class PercentageBar {
public:
    PercentageBar(float current, float max, int w, int h, color fillColor, color outlineColor);

    void SetValues(float current, float max);
    void SetPosition(int x, int y);
    void SetOffset(int offX, int offY);
    void SetFillColor(color c);
    void SetOutlineColor(color c);
    void Draw(Renderer* renderer);

private:
    float current;
    float max;
    int w;
    int h;
    int x;
    int y;
    int offsetX;
    int offsetY;
    color fillColor;
    color outlineColor;
};

#endif