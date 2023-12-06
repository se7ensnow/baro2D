//
// Created by User on 06.12.2023.
//

#include "button.h"


Button::Button(float x, float y, float width, float height,
               sf::Font *font, std::string text,
               sf::Color idleColor, sf::Color hoverColor, sf::Color activeColor)
               : font_(font), idleColor_(idleColor), hoverColor_(hoverColor), activeColor_(activeColor) {
    buttonState_ = BTN_IDLE;

    shape_.setPosition(sf::Vector2f(x, y));
    shape_.setSize(sf::Vector2f(width, height));

    text_.setFont(*font_);
    text_.setString(text);
    text_.setFillColor(sf::Color::White);
    text_.setCharacterSize(12);
    text_.setPosition(
            shape_.getPosition().x + (shape_.getSize().x / 2.f) - text_.getGlobalBounds().width / 2.f,
            shape_.getPosition().y + (shape_.getSize().y / 2.f) - text_.getGlobalBounds().height / 2.f);

    shape_.setFillColor(idleColor_);
}

Button::~Button() {

}

void Button::update(const sf::Vector2f& mousePos) {

    buttonState_ = BTN_IDLE;

    if (shape_.getGlobalBounds().contains(mousePos)) {
        buttonState_ = BTN_HOVER;

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            buttonState_ = BTN_ACTIVE;
        }
    }

    switch (buttonState_) {
        case BTN_IDLE:
            shape_.setFillColor(idleColor_);
            break;
        case BTN_HOVER:
            shape_.setFillColor(hoverColor_);
            break;
        case BTN_ACTIVE:
            shape_.setFillColor(activeColor_);
            break;
        default:
            std::cerr <<"ERROR::BUTTON::Switch state default case!" <<std::endl;
            break;
    }
}

void Button::render(sf::RenderTarget *target) {
    target->draw(shape_);
    target->draw(text_);
}

bool Button::isPressed() const {
    if (buttonState_ == BTN_ACTIVE)
        return true;
    return false;
}




