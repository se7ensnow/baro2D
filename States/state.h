#pragma once

#include "../Entities/entity.h"

class State {
public:
    explicit State(sf::RenderWindow* window, std::map<std::string, int>* supportedKeys, std::stack<State*>* states);
    virtual ~State();

    const bool& getQuit() const;

    void endState();
    void pauseState();
    void unpauseState();

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
    std::array<bool, sf::Keyboard::KeyCount> keyStates_;

    bool quit_;
    bool paused_;

    sf::Vector2i mousePosScreen_;
    sf::Vector2i mousePosWindow_;
    sf::Vector2f mousePosView_;

    std::map<std::string, sf::Texture> textures_;
    std::map<std::string, sf::SoundBuffer> sounds_;
};
