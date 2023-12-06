#pragma once
#include "../src/headers.h"

class Button {
public:
    enum button_states{BTN_IDLE = 0, BTN_HOVER, BTN_ACTIVE};

public:
    Button(float x, float y, float width, float height,
           sf::Font* font, std::string text,
           sf::Color idleColor, sf::Color hoverColor, sf::Color activeColor);
    ~Button();

    bool isPressed() const;

    void update(const sf::Vector2f& mousePos);
    void render(sf::RenderTarget* target);

protected:
    uint16_t buttonState_;

    sf::RectangleShape shape_;
    sf::Font* font_;
    sf::Text text_;

    sf::Color idleColor_;
    sf::Color hoverColor_;
    sf::Color activeColor_;
};
