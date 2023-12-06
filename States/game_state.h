#pragma once

#include "state.h"

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
    void updateInput(const float& dt) override;

protected:
    Entity player_;
};
