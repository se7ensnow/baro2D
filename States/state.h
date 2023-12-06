#pragma once

#include "../Entities/entity.h"

class State {
public:
    explicit State(sf::RenderWindow* window, std::map<std::string, int>* supportedKeys, std::stack<State*>* states);
    virtual ~State();

    const bool& getQuit() const;
    void endState();

    virtual void update(const float& dt) = 0;
    virtual void render(sf::RenderTarget* target) = 0;

protected:

    virtual void initKeybinds() = 0;
    virtual void updateInput(const float& dt) = 0;
    virtual void updateMousePositions();

protected:
    std::stack<State*>* states_;
    sf::RenderWindow* window_;
    std::map<std::string, int>* supportedKeys_;
    std::map<std::string, int> keybinds_;
    bool quit_;

    sf::Vector2i mousePosScreen_;
    sf::Vector2i mousePosWindow_;
    sf::Vector2f mousePosView_;

    std::vector<sf::Texture> textures_;
};
