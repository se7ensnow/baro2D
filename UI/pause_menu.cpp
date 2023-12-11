#include "pause_menu.h"

PauseMenu::PauseMenu(sf::RenderWindow& window, sf::Font& font) : font_(font) {
    background_.setSize(sf::Vector2f(static_cast<float>(window.getSize().x),
                                     static_cast<float>(window.getSize().y)));
    background_.setFillColor(sf::Color(20, 20, 20, 100));

    container_.setSize(sf::Vector2f(static_cast<float>(window.getSize().x) / 4.f,
                                    static_cast<float>(window.getSize().y)));
    container_.setFillColor(sf::Color(20, 20, 20, 200));
    container_.setPosition(static_cast<float>(window.getSize().x) / 2.f - container_.getSize().x / 2.f, 0.f);

    menuText_.setFont(font_);
    menuText_.setFillColor(sf::Color(255, 255, 255, 200));
    menuText_.setCharacterSize(80);
    menuText_.setString("PAUSED");
    menuText_.setPosition(container_.getPosition().x + (container_.getSize().x / 2.f) - (menuText_.getGlobalBounds().width / 2.f) - menuText_.getGlobalBounds().left,
                          container_.getPosition().y + 60.f);
}

PauseMenu::~PauseMenu() {
    auto it = buttons_.begin();
    for (it = buttons_.begin(); it != buttons_.end(); ++it) {
        delete it->second;
    }
}

void PauseMenu::addButton(const std::string& key, float y, const std::string& text) {
    float width = 250.f;
    float height = 50.f;
    float x = container_.getPosition().x + (container_.getSize().x / 2.f) - (width / 2.f);
    buttons_[key] = new Button(x, y, width, height,
                               &font_, text, 50,
                               sf::Color(70, 70, 70, 200),
                               sf::Color(250, 250, 250, 250),
                               sf::Color(20, 20, 20, 50),
                               sf::Color(70, 70, 70, 0),
                               sf::Color(150, 150, 150, 0),
                               sf::Color(20, 20, 20, 0));
}

bool PauseMenu::isButtonPressed(const std::string& key) {
    return buttons_[key]->isPressed();
}

void PauseMenu::update(const sf::Vector2f& mousePos) {
    updateButtons(mousePos);
}

void PauseMenu::render(sf::RenderTarget* target) {
    target->draw(background_);
    target->draw(container_);
    target->draw(menuText_);
    renderButtons(target);
}

void PauseMenu::updateButtons(const sf::Vector2f& mousePos) {
    for (auto& it : buttons_) {
        it.second->update(mousePos);
    }
}

void PauseMenu::renderButtons(sf::RenderTarget* target) {
    for (auto& it : buttons_) {
        it.second->render(target);
    }
}
