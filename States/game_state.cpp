#include "game_state.h"

GameState::GameState(sf::RenderWindow* window,
                     std::map<std::string, int>* supportedKeys,
                     std::stack<State*>* states)
    : State(window, supportedKeys, states) {
    pmenu_ = nullptr;
    initKeybinds();
    initFonts();
    initTextures();
    initPauseMenu();
    initSub();
    initMap();
}

GameState::~GameState() {
    delete pmenu_;
    delete sub_;
}

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

void GameState::initFonts() {
    if (!font_.loadFromFile("../../Fonts/Silkscreen-Bold.ttf")) {
        std::cerr <<"ERROR::GAMESTATE::Could not load font!" <<std::endl;
    }
}

void GameState::initTextures() {
    sf::Texture temp;
    if (!textures_["SUBMARINE_IDLE"].loadFromFile("../../Resources/Images/Sprites/Sub/submarine_texture.png")) {
        std::cerr <<"ERROR::GAMESTATE::Could not load submarine texture!" <<std::endl;
    }
}

void GameState::initPauseMenu() {
    pmenu_ = new PauseMenu(*window_, font_);

    pmenu_->addButton("EXIT_STATE", static_cast<float>(window_->getSize().y) - 200.f, "Quit");
}

void GameState::initSub() {
    sub_ = new Sub(0, 0, textures_["SUBMARINE_IDLE"]);
}

void GameState::initMap() {
    map_ = new Map(192, 108, 10.f, 45, 5);
}

void GameState::update(const float& dt) {
    updateMousePositions();
    updateInput(dt);

    if (!paused_) {
        updatePlayerInput(dt);
        sub_->update(dt);
    } else {
        pmenu_->update(mousePosView_);
        updatePauseMenuGui();
    }
}

void GameState::render(sf::RenderTarget* target) {
    if (!target) {
        target = window_;
    }

    sub_->render(target);
    map_->render(target);

    if (paused_) {
        pmenu_->render(target);
    }
}

void GameState::updatePlayerInput(const float &dt) {
     if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(keybinds_.at("MOVE_LEFT")))) {
        sub_->move(-1.f, 0.f, dt);
        std::cout <<"A" <<std::endl;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(keybinds_.at("MOVE_RIGHT")))) {
        sub_->move(1.f, 0.f, dt);
        std::cout <<"D" <<std::endl;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(keybinds_.at("MOVE_UP")))) {
        sub_->move(0.f, -1.f, dt);
        std::cout <<"W" <<std::endl;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(keybinds_.at("MOVE_DOWN")))) {
        sub_->move(0.f, 1.f, dt);
        std::cout <<"S" <<std::endl;
    }
}

void GameState::updateInput(const float &dt) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(keybinds_.at("CLOSE")))) {
        if (!keyStates_[keybinds_.at("CLOSE")]) {
            keyStates_[keybinds_.at("CLOSE")] = true;
            if (!paused_) {
                pauseState();
            } else {
                unpauseState();
            }
        }
    } else {
        keyStates_[keybinds_.at("CLOSE")] = false;
    }
}

void GameState::updatePauseMenuGui() {
    if (pmenu_->isButtonPressed("EXIT_STATE")) {
        endState();
    }
}

