#pragma once

#include "state.h"
#include "../UI/pause_menu.h"
#include "../Map/map.h"


class GameState : public State {
public:
    explicit GameState(sf::RenderWindow* window,
                       std::map<std::string, int>* supportedKeys,
                       std::stack<State*>* states);
    ~GameState() override;

    void update(const float& dt) override;
    void render(sf::RenderTarget* target) override;

protected:
    void initKeybinds() override;
    void initFonts();
    void initTextures();
    void initPauseMenu();
    void initSub();
    void initMap();
    void updatePlayerInput(const float& dt);
    void updateInput(const float& dt) override;
    void updatePauseMenuGui();

protected:
    sf::Font font_;
    PauseMenu* pmenu_;

    Sub* sub_;
    Map* map_;
};
