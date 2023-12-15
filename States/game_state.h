#pragma once

#include "state.h"
#include "../UI/pause_menu.h"
#include "../UI/sonar.h"
#include "../Controls/control.h"


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
    void initSounds();
    void initPauseMenu();
    void initSub();
    void initControl();
    void initMap();
    void initSonar();
    void updatePlayerInput(const float& dt);
    void updateInput(const float& dt) override;
    void updatePauseMenuGui();

protected:
    sf::Font font_;
    sf::Font secondFont_;
    std::unique_ptr<PauseMenu> pmenu_;

    std::unique_ptr<Sub> sub_;
    std::unique_ptr<Map> map_;
    std::unique_ptr<Sonar> sonar_;
    std::unique_ptr<Control> control_;
};
