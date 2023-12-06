#include "main_menu_state.h"

MainMenuState::MainMenuState(sf::RenderWindow* window,
                             std::map<std::string, int>* supportedKeys,
                             std::stack<State*>* states)
        : State(window, supportedKeys, states) {
    initVariables();
    initBackground();
    initFonts();
    initKeybinds();
    initButtons();
}

MainMenuState::~MainMenuState() {
    auto it = buttons_.begin();
    for (it = buttons_.begin(); it != buttons_.end(); ++it) {
        delete it->second;
    }
}

void MainMenuState::initVariables() {

}

void MainMenuState::initBackground() {
    background_.setSize(sf::Vector2f(window_->getSize()));
    if (!bgTexture_.loadFromFile("../../Resources/Images/Backgrounds/main_menu_bg.png")) {
        std::cerr <<"ERROR::MAINMENUSTATE::Could not load background texture!" <<std::endl;
    }
    background_.setTexture(&bgTexture_);
}

void MainMenuState::initFonts() {
    if (!font_.loadFromFile("../../Fonts/Silkscreen-Bold.ttf")) {
        std::cerr <<"ERROR::MAINMENUSTATE::Could not load font!" <<std::endl;
    }
}

void MainMenuState::initKeybinds() {
    std::ifstream ifs("../../Config/mainmenustate_keybinds.ini");

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

void MainMenuState::initButtons() {
    buttons_["GAME_STATE"] = new Button(100, 100, 150, 50,
                               &font_, "New Game",
                               sf::Color(70, 70, 70, 200),
                               sf::Color(150, 150, 150, 255),
                               sf::Color(20, 20, 20, 200));

    buttons_["EXIT_STATE"] = new Button(100, 300, 150, 50,
                                        &font_, "Quit",
                                        sf::Color(100, 100, 100, 200),
                                        sf::Color(150, 150, 150, 255),
                                        sf::Color(20, 20, 20, 200));
}

void MainMenuState::update(const float& dt) {
    updateMousePositions();
    updateInput(dt);
    updateButtons();
}

void MainMenuState::render(sf::RenderTarget* target) {
    if (!target) {
        target = window_;
    }

    target->draw(background_);
    renderButtons(target);

    // sf::Text mouseText;
    // mouseText.setPosition(mousePosView_.x, mousePosView_.y - 50);
    // mouseText.setFont(font_);
    // mouseText.setCharacterSize(12);
    // mouseText.setString(std::to_string(mousePosView_.x) + " " + std::to_string(mousePosView_.y));
    // target->draw(mouseText);
}

void MainMenuState::updateInput(const float &dt) {

}

void MainMenuState::updateButtons() {
    for (auto& it : buttons_) {
        it.second->update(mousePosView_);
    }

    if (buttons_["GAME_STATE"]->isPressed()) {
        states_->push(new GameState(window_, supportedKeys_, states_));
    }

    if (buttons_["EXIT_STATE"]->isPressed()) {
        endState();
    }
}

void MainMenuState::renderButtons(sf::RenderTarget *target) {
    for (auto& it : buttons_) {
        it.second->render(target);
    }
}

