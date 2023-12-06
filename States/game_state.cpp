#include "game_state.h"

GameState::GameState(sf::RenderWindow* window,
                     std::map<std::string, int>* supportedKeys,
                     std::stack<State*>* states)
    : State(window, supportedKeys, states) {
    initKeybinds();
}

GameState::~GameState() = default;

void GameState::initKeybinds() {
    std::ifstream ifs("../../Config/gamestate_keybinds.ini");

    if (ifs.is_open()) {
        std::string key;
        std::string key2;

        while (ifs >> key >> key2) {
            keybinds_[key] = supportedKeys_->at(key2);
        }

        ifs.close();
    } else {
        std::cerr << "ERROR::GAME_STATE::Could not load keybinds!" << std::endl;
    }
}

void GameState::update(const float& dt) {
    updateMousePositions();
    updateInput(dt);
}

void GameState::render(sf::RenderTarget* target) {
    if (!target) {
        target = window_;
    }
}

void GameState::updateInput(const float &dt) {
     if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(keybinds_.at("MOVE_LEFT")))) {
        player_.move(dt, -1.f, 0.f);
        std::cout <<"A" <<std::endl;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(keybinds_.at("MOVE_RIGHT")))) {
        player_.move(dt, 1.f, 0.f);
        std::cout <<"D" <<std::endl;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(keybinds_.at("MOVE_UP")))) {
        player_.move(dt, 0.f, 1.f);
        std::cout <<"W" <<std::endl;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(keybinds_.at("MOVE_DOWN")))) {
        player_.move(dt, 0.f, -1.f);
        std::cout <<"S" <<std::endl;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(keybinds_.at("CLOSE")))) {
        endState();
    }

}
