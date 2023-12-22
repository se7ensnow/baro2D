#pragma once

#include "state.h"
#include "../UI/sonar.h"
#include "../Controls/control.h"


class GameState : public State {
public:
    explicit GameState(sf::RenderWindow* window,
                       std::map<std::string, int>* supportedKeys,
                       std::stack<std::unique_ptr<State>>& states);
    ~GameState() override;

    void update(const float& dt) override;
    void render(sf::RenderTarget* target) override;

protected:
    void initKeybinds() override;
    void initFonts();
    void initTextures();
    void initSounds();
    void initMenus();
    void initSub();
    void initControl();
    void initMap();
    void initSonar();
    void updatePlayerInput(const float& dt);
    void updateInput(const float& dt) override;
    void updateMenusGui();

protected:
    sf::Font font_;
    sf::Font secondFont_;
    std::unique_ptr<PauseMenu> pmenu_;
    std::unique_ptr<EndGameMenu> emenu_;
    EndGameMenu::endStates gameState_;

    std::unique_ptr<Sub> sub_;
    std::unique_ptr<Map> map_;
    std::unique_ptr<Sonar> sonar_;
    std::unique_ptr<Control> control_;
};
