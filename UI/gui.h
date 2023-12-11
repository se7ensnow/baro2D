#pragma once

#include "../src/headers.h"


class Button {
public:
    enum button_states{BTN_IDLE = 0, BTN_HOVER, BTN_ACTIVE};

public:
    Button(float x, float y, float width, float height,
           sf::Font* font, const std::string& text, uint32_t characterSize,
           sf::Color textIdleColor, sf::Color textHoverColor, sf::Color textActiveColor,
           sf::Color idleColor, sf::Color hoverColor, sf::Color activeColor,
           sf::Color outlineIdleColor = sf::Color::Transparent,
           sf::Color outlineHoverColor = sf::Color::Transparent,
           sf::Color outlineActiveColor = sf::Color::Transparent,
           uint16_t id = 0);
    ~Button();

    bool isPressed() const;
    std::string getText() const;
    const uint16_t& getId() const;

    void setText(const std::string& text);
    void setId(uint16_t id);

    void update(const sf::Vector2f& mousePos);
    void render(sf::RenderTarget* target);

protected:
    uint16_t buttonState_;
    uint16_t id_;

    sf::RectangleShape shape_;
    sf::Font* font_;
    sf::Text text_;

    sf::Color textIdleColor_;
    sf::Color textHoverColor_;
    sf::Color textActiveColor_;

    sf::Color idleColor_;
    sf::Color hoverColor_;
    sf::Color activeColor_;

    sf::Color outlineIdleColor_;
    sf::Color outlineHoverColor_;
    sf::Color outlineActiveColor_;
};

class DropDownList {
public:
    DropDownList(float x, float y, float width, float height,
                 sf::Font& font,
                 std::vector<std::string> list,
                 size_t defaultIndex = 0);
    ~DropDownList();

    const uint16_t& getActiveElementId() const;

    void update(const sf::Vector2f& mousePos);
    void render(sf::RenderTarget* target);

protected:
    sf::Font& font_;
    std::unique_ptr<Button> activeElement_;
    std::vector<std::unique_ptr<Button>> elements_;
    std::vector<bool> buttonStates_;

    size_t buttonCount_;
    bool showList_;
};
