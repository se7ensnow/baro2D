#pragma once
#include "game_state.h"
#include "settings_state.h"
#include "../UI/gui.h"

class MainMenuState : public State {
public:
    explicit MainMenuState(sf::RenderWindow* window,
                           std::map<std::string, int>* supportedKeys,
                           std::stack<State*>* states);
    ~MainMenuState() override;

    void update(const float& dt) override;
    void render(sf::RenderTarget* target) override;

protected:
    void initVariables();
    void initBackground();
    void initFonts();
    void initKeybinds() override;
    void initGui();
    void updateInput(const float& dt) override;
    void updateGui();
    void renderGui(sf::RenderTarget* target = nullptr);

protected:
    sf::Texture bgTexture_;
    sf::RectangleShape background_;
    sf::Font font_;

    std::map<std::string, std::unique_ptr<Button>> buttons_;
};
