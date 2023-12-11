#pragma once


#include "gui.h"

class PauseMenu {
public:
    PauseMenu(sf::RenderWindow& window, sf::Font& font);
    ~PauseMenu();



    void addButton(const std::string& key, float y, const std::string& text);
    bool isButtonPressed(const std::string& key);
    void update(const sf::Vector2f& mousePos);
    void render(sf::RenderTarget* target);

protected:
    void updateButtons(const sf::Vector2f& mousePos);
    void renderButtons(sf::RenderTarget* target);

protected:
    sf::Font& font_;
    sf::Text menuText_;
    sf::RectangleShape background_;
    sf::RectangleShape container_;

    std::map<std::string, Button*> buttons_;
};
