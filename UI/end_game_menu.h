#pragma once

#include "pause_menu.h"

class EndGameMenu : public PauseMenu {
public:
    enum endStates {NONE=0, WON, LOST};

public:
    EndGameMenu(sf::RenderWindow& window, sf::Font& font);
    ~EndGameMenu();

    void setState(endStates state);

    void render(sf::RenderTarget* target) override;

protected:
    void initTexts();

protected:
    sf::Text winText_;
    sf::Text loseText_;

    endStates state_;
};
