#pragma once
#include "../States/main_menu_state.h"

class Game {
public:
    Game();
    ~Game();

    void endApplication();

    void update();
    void render();
    void run();

protected:
    void initVariables();
    void initWindow();
    void initKeys();
    void initStates();

    void updateDeltaTime();
    void updateSFMLEvents();

protected:
    sf::RenderWindow* window_;
    sf::Event sfEvent_;
    std::vector<sf::VideoMode> videoModes_;
    sf::ContextSettings windowSettings_;
    bool fullscreen_;

    sf::Clock dtClock_;
    float deltaTime_;

    std::stack<State*> states_;

    std::map<std::string, int> supportedKeys_;
};
