#include "Container.hpp"

Container::Container(int x, int y, int w, int h, Color fillColor, Color borderColor, int alpha, int borderThickness)
    : fillColor(fillColor), borderColor(borderColor)
    , borderThickness(borderThickness)
    , alpha(alpha)
    , visible(true) {
    dstRect = { x, y, w, h };
    SetDrawLayer(RenderLayer::UI, -1);
}

void Container::AddSprite(Sprite* sprite, int localX, int localY) {
    sprites.push_back({ sprite, localX, localY });
    sprite->SetPosition(dstRect.x + localX, dstRect.y + localY);
    sprite->SetDrawLayer(RenderLayer::UITEXT, 2);
}

void Container::AddButton(Button* button, int localX, int localY) {
    buttons.push_back({ button, localX, localY });
    button->SetPosition(dstRect.x + localX, dstRect.y + localY);
    button->SetDrawLayer(RenderLayer::UITEXT, 1);
}

void Container::AddText(Text* text, int localX, int localY) {
    texts.push_back({ text, localX, localY });
    text->SetPosition(dstRect.x + localX, dstRect.y + localY);
    text->SetDrawLayer(RenderLayer::UITEXT, 3);
}

void Container::Process(float deltaTime) {
    if (!visible) return;

    for (auto& item : buttons)
        item.button->Process(deltaTime);

    for (auto& item : sprites)
        item.sprite->Process(deltaTime);

    for (auto& item : texts)
        item.text->Process(deltaTime);
}

void Container::Draw(Renderer* renderer) {
    if (!visible) return;

    // draw border as outer filled rect
    renderer->AddFilledRect(dstRect.x, dstRect.y, dstRect.w, dstRect.h, borderColor, alpha, RenderLayer::UI);

    // draw fill inset by border thickness
    renderer->AddFilledRect(
        dstRect.x + borderThickness, dstRect.y + borderThickness,
        dstRect.w - borderThickness * 2, dstRect.h - borderThickness * 2,
        fillColor, alpha, RenderLayer::UI);

    for (auto& item : sprites)
        item.sprite->Draw(renderer);

    for (auto& item : buttons)
        item.button->Draw(renderer);

    for (auto& item : texts)
        item.text->Draw(renderer);
}

void Container::SetPosition(int newX, int newY) {
    dstRect.x = newX;
    dstRect.y = newY;

    for (auto& item : sprites)
        item.sprite->SetPosition(dstRect.x + item.localX, dstRect.y + item.localY);

    for (auto& item : buttons)
        item.button->SetPosition(dstRect.x + item.localX, dstRect.y + item.localY);

    for (auto& item : texts)
        item.text->SetPosition(dstRect.x + item.localX, dstRect.y + item.localY);
}

void Container::SetVisible(bool v) {
    visible = v;
}

bool Container::IsVisible() const {
    return visible;
}

void Container::Toggle() {
    visible = !visible;
}

Button* Container::GetFirstButton() {
    if (buttons.empty()) return nullptr;
    return buttons[0].button;
}

void Container::SetFirstSprite(Sprite* s) {
    if (sprites.empty()) return;
    Vector2 drawSize = sprites[0].sprite->GetDrawSize();
    sprites[0].sprite = s; // actually modifies the vector
    s->SetDrawSize(drawSize.x, drawSize.y);
    s->SetPosition(dstRect.x + sprites[0].localX, dstRect.y + sprites[0].localY);
    s->SetDrawLayer(RenderLayer::UITEXT, 2);
}

void Container::UpdateCardRecipe(int a, int b, int c) {
    if (texts.size() < 3) return;

    texts[0].text->SetText(to_string(a));
    texts[0].text->SetPosition(dstRect.x + texts[0].localX, dstRect.y + texts[0].localY);
    texts[1].text->SetText(to_string(b));
    texts[1].text->SetPosition(dstRect.x + texts[1].localX, dstRect.y + texts[1].localY);
    texts[2].text->SetText(to_string(c));
    texts[2].text->SetPosition(dstRect.x + texts[2].localX, dstRect.y + texts[2].localY);
}