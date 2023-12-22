#include "end_game_menu.h"

EndGameMenu::EndGameMenu(sf::RenderWindow& window, sf::Font& font) : PauseMenu(window, font) {
    state_ = NONE;

    initTexts();
}

void EndGameMenu::initTexts() {
    winText_.setFont(font_);
    winText_.setFillColor(sf::Color::Green);
    winText_.setCharacterSize(130);
    winText_.setString("YOU WON");
    winText_.setPosition(container_.getPosition().x + (container_.getSize().x / 2.f) - (winText_.getGlobalBounds().width / 2.f) - winText_.getGlobalBounds().left,
                         container_.getPosition().y + (container_.getSize().y / 2.f) - (winText_.getGlobalBounds().height / 2.f) - winText_.getGlobalBounds().top);

    loseText_.setFont(font_);
    loseText_.setFillColor(sf::Color::Red);
    loseText_.setCharacterSize(130);
    loseText_.setString("YOU LOST");
    loseText_.setPosition(container_.getPosition().x + (container_.getSize().x / 2.f) - (loseText_.getGlobalBounds().width / 2.f) - loseText_.getGlobalBounds().left,
                         container_.getPosition().y + (container_.getSize().y / 2.f) - (loseText_.getGlobalBounds().height / 2.f) - loseText_.getGlobalBounds().top);
}

void EndGameMenu::render(sf::RenderTarget* target) {
    target->draw(background_);
    target->draw(container_);
    renderButtons(target);
    if (state_ == WON) {
        target->draw(winText_);
    } else if (state_ == LOST) {
        target->draw(loseText_);
    }
}

void EndGameMenu::setState(EndGameMenu::endStates state) {
    state_ = state;
}

EndGameMenu::~EndGameMenu() = default;
