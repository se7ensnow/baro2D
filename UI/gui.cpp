//
// Created by User on 06.12.2023.
//

#include "gui.h"


Button::Button(float x, float y, float width, float height,
               sf::Font *font, const std::string& text, uint32_t characterSize,
               sf::Color textIdleColor, sf::Color textHoverColor, sf::Color textActiveColor,
               sf::Color idleColor, sf::Color hoverColor, sf::Color activeColor,
               sf::Color outlineIdleColor, sf::Color outlineHoverColor, sf::Color outlineActiveColor,
               uint16_t id)
               : font_(font),
               textIdleColor_(textIdleColor),
               textHoverColor_(textHoverColor),
               textActiveColor_(textActiveColor),
               idleColor_(idleColor),
               hoverColor_(hoverColor),
               activeColor_(activeColor),
               outlineIdleColor_(outlineIdleColor),
               outlineHoverColor_(outlineHoverColor),
               outlineActiveColor_(outlineActiveColor),
               id_(id) {
    buttonState_ = BTN_IDLE;

    shape_.setPosition(sf::Vector2f(x, y));
    shape_.setSize(sf::Vector2f(width, height));
    shape_.setFillColor(idleColor_);
    shape_.setOutlineThickness(1.f);
    shape_.setOutlineColor(outlineIdleColor_);

    text_.setFont(*font_);
    text_.setString(text);
    text_.setFillColor(textIdleColor_);
    text_.setCharacterSize(characterSize);
    text_.setPosition(
            shape_.getPosition().x + (shape_.getSize().x / 2.f) - text_.getGlobalBounds().width / 2.f - text_.getGlobalBounds().left,
            shape_.getPosition().y + (shape_.getSize().y / 2.f) - text_.getGlobalBounds().height / 2.f - text_.getGlobalBounds().top);
}

Button::~Button() {

}

bool Button::isPressed() const {
    if (buttonState_ == BTN_ACTIVE)
        return true;
    return false;
}

std::string Button::getText() const {
    return text_.getString();
}

const uint16_t& Button::getId() const {
    return id_;
}

void Button::setText(const std::string &text) {
    text_.setString(text);
}

void Button::setId(uint16_t id) {
    id_ = id;
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
            shape_.setOutlineColor(outlineIdleColor_);
            text_.setFillColor(textIdleColor_);
            break;
        case BTN_HOVER:
            shape_.setFillColor(hoverColor_);
            shape_.setOutlineColor(outlineHoverColor_);
            text_.setFillColor(textHoverColor_);
            break;
        case BTN_ACTIVE:
            shape_.setFillColor(activeColor_);
            shape_.setOutlineColor(outlineActiveColor_);
            text_.setFillColor(textActiveColor_);
            break;
        default:
            std::cerr <<"ERROR::BUTTON::Switch state default case!" <<std::endl;
            break;
    }
}

void Button::render(sf::RenderTarget* target) {
    target->draw(shape_);
    target->draw(text_);
}

DropDownList::DropDownList(float x, float y, float width, float height,
                           sf::Font& font,
                           std::vector<std::string> list,
                           size_t defaultIndex) : font_(font) {
    showList_ = false;

    buttonCount_ = list.size();

    activeElement_ = std::make_unique<Button>(Button(x, y, width, height,
                                                     &font_, list[defaultIndex], 14,
                                                     sf::Color(255, 255, 255, 150),
                                                     sf::Color(255, 255, 255, 200),
                                                     sf::Color(255, 255, 255, 50),
                                                     sf::Color(70, 70, 70, 200),
                                                     sf::Color(150, 150, 150, 200),
                                                     sf::Color(70, 70, 70, 150)));

    for (size_t i = 0; i < buttonCount_; ++i) {
        elements_.push_back(std::make_unique<Button>(Button(x, y + (static_cast<float>(i + 1) * height), width, height,
                                                            &font_, list[i], 14,
                                                            sf::Color(255, 255, 255, 200),
                                                            sf::Color(255, 255, 255, 255),
                                                            sf::Color(255, 255, 255, 50),
                                                            sf::Color(70, 70, 70, 200),
                                                            sf::Color(150, 150, 150, 200),
                                                            sf::Color(70, 70, 70, 150))));
        elements_[i]->setId(i);
    }

    buttonStates_.resize(buttonCount_ + 1, false);
}

DropDownList::~DropDownList() {

}

const uint16_t &DropDownList::getActiveElementId() const {
    return activeElement_->getId();
}


void DropDownList::update(const sf::Vector2f &mousePos) {
    activeElement_->update(mousePos);

    if (activeElement_->isPressed()) {
        if (!buttonStates_[buttonCount_]) {
            buttonStates_[buttonCount_] = true;
            if (showList_) {
                showList_ = false;
            } else {
                showList_ = true;
            }
        }
    } else {
        buttonStates_[buttonCount_] = false;
    }

    if (showList_) {
        for (auto &i: elements_) {
            i->update(mousePos);

            if (i->isPressed()) {
                if (!buttonStates_[i->getId()]) {
                    buttonStates_[i->getId()] = true;
                    showList_ = false;
                    activeElement_->setText(i->getText());
                    activeElement_->setId(i->getId());
                }
            } else {
                buttonStates_[i->getId()] = false;
            }
        }
    }
}

void DropDownList::render(sf::RenderTarget* target) {
    activeElement_->render(target);
    if (showList_) {
        for (auto &i: elements_) {
            i->render(target);
        }
    }

}