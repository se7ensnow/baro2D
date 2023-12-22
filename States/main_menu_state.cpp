#include "main_menu_state.h"

MainMenuState::MainMenuState(sf::RenderWindow* window,
                             std::map<std::string, int>* supportedKeys,
                             std::stack<std::unique_ptr<State>>& states)
        : State(window, supportedKeys, states) {
    initVariables();
    initBackground();
    initFonts();
    initKeybinds();
    initGui();
}

MainMenuState::~MainMenuState() = default;

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
    if (!font_.loadFromFile("../../Resources/Fonts/Silkscreen-Bold.ttf")) {
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
        std::cerr << "ERROR::MAINMENUSTATE::Could not load keybinds!" << std::endl;
    }
}

void MainMenuState::initGui() {
    buttons_["GAME_STATE"] = std::make_unique<Button>(110, 300, 250, 50,
                                                      &font_, "New Game", 50,
                                                      sf::Color(70, 70, 70, 200),
                                                      sf::Color(250, 250, 250, 250),
                                                      sf::Color(20, 20, 20, 50),
                                                      sf::Color(70, 70, 70, 0),
                                                      sf::Color(150, 150, 150, 0),
                                                      sf::Color(20, 20, 20, 0));

    // buttons_["SETTINGS_STATE"] = std::make_unique<Button>(110, 400, 250, 50,
    //                                                       &font_, "Settings", 50,
    //                                                       sf::Color(70, 70, 70, 200),
    //                                                       sf::Color(250, 250, 250, 250),
    //                                                       sf::Color(20, 20, 20, 50),
    //                                                       sf::Color(70, 70, 70, 0),
    //                                                       sf::Color(150, 150, 150, 0),
    //                                                       sf::Color(20, 20, 20, 0));

    buttons_["EXIT_STATE"] = std::make_unique<Button>(110, 550, 250, 50,
                                                      &font_, "Quit", 50,
                                                      sf::Color(70, 70, 70, 200),
                                                      sf::Color(250, 250, 250, 250),
                                                      sf::Color(20, 20, 20, 50),
                                                      sf::Color(70, 70, 70, 0),
                                                      sf::Color(150, 150, 150, 0),
                                                      sf::Color(20, 20, 20, 0));
}

void MainMenuState::update(const float& dt) {
    updateMousePositions();
    updateInput(dt);
    updateGui();
}

void MainMenuState::render(sf::RenderTarget* target) {
    if (!target) {
        target = window_;
    }

    target->draw(background_);
    renderGui(target);

    sf::Text mouseText;
    mouseText.setPosition(mousePosView_.x, mousePosView_.y - 50);
    mouseText.setFont(font_);
    mouseText.setCharacterSize(12);
    mouseText.setString(std::to_string((int)mousePosView_.x) + " " + std::to_string((int)mousePosView_.y));
    target->draw(mouseText);
}

void MainMenuState::updateInput(const float &dt) {

}

void MainMenuState::updateGui() {
    for (auto& it : buttons_) {
        it.second->update(mousePosView_);
    }

    if (buttons_["GAME_STATE"]->isPressed()) {
        states_.push(std::make_unique<GameState>(window_, supportedKeys_, states_));
    }

    if (buttons_["EXIT_STATE"]->isPressed()) {
        endState();
    }
}

void MainMenuState::renderGui(sf::RenderTarget *target) {
    for (auto& it : buttons_) {
        it.second->render(target);
    }
}

