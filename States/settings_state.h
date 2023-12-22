#pragma once

#include "state.h"
#include "../UI/gui.h"

class SettingsState : public State {
public:
    SettingsState(sf::RenderWindow* window,
                  std::map<std::string, int>* supportedKeys,
                  std::stack<std::unique_ptr<State>>& states);
    ~SettingsState();

    void update(const float& dt) override;
    void render(sf::RenderTarget* target) override;

protected:
    void initVariables();
    void initBackground();
    void initFonts();
    void initKeybinds() override;
    void initGui();
    void initText();
    void updateInput(const float& dt) override;
    void updateGui();
    void renderGui(sf::RenderTarget* target = nullptr);

protected:
    sf::Texture bgTexture_;
    sf::RectangleShape background_;
    sf::Font font_;
    sf::Text optionsText_;

    std::map<std::string, std::unique_ptr<Button>> buttons_;
    std::map<std::string, std::unique_ptr<DropDownList>> dropDownLists_;

    std::vector<sf::VideoMode> modes_;
};
