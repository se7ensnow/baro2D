#include "game.h"

Game::Game() {
    initVariables();
    initWindow();
    initKeys();
    initStates();
}

Game::~Game() {
    delete window_;

    while (!states_.empty()) {
        delete states_.top();
        states_.pop();
    }
}

void Game::endApplication() {
    std::cout <<"Ending Application!" <<std::endl;
}

void Game::initVariables() {
    window_ = nullptr;
    fullscreen_ = false;

    deltaTime_ = 0.f;
}

void Game::initWindow() {

    std::ifstream ifs("../../Config/window.ini");
    videoModes_ = sf::VideoMode::getFullscreenModes();

    std::string title = "None";
    sf::VideoMode window_bounds = sf::VideoMode::getDesktopMode();
    bool fullscreen = false;
    uint32_t framerate_limit = 120;
    bool vsync_enabled = false;
    unsigned antialiasing_lvl = 0;

    if (ifs.is_open()) {
        std::getline(ifs, title);
        ifs >> window_bounds.width >> window_bounds.height;
        ifs >> fullscreen;
        ifs >> framerate_limit;
        ifs >> vsync_enabled;
        ifs >> antialiasing_lvl;
        ifs.close();
    } else {
        std::cerr <<"ERROR::GAME::Could not load config!" <<std::endl;
    }

    fullscreen_ = fullscreen;
    windowSettings_.antialiasingLevel = antialiasing_lvl;
    if (fullscreen_)
        window_ = new sf::RenderWindow(window_bounds, title, sf::Style::Fullscreen, windowSettings_);
    else
        window_ = new sf::RenderWindow(window_bounds, title, sf::Style::Titlebar | sf::Style::Close, windowSettings_);
    window_->setFramerateLimit(framerate_limit);
    window_->setVerticalSyncEnabled(vsync_enabled);
}

void Game::initKeys() {
    std::ifstream ifs("../../Config/supported_keys.ini");

    if (ifs.is_open()) {
        std::string key;
        int key_value = 0;

        while (ifs >>key >>key_value) {
            supportedKeys_[key] = key_value;
        }

        ifs.close();
    } else {
        std::cerr <<"ERROR::GAME::Could not load supported keys!" <<std::endl;
    }
}

void Game::initStates() {
    states_.push(new MainMenuState(window_, &supportedKeys_, &states_));
}

void Game::update() {
    if (!states_.empty()) {
        states_.top()->update(deltaTime_);

        if (states_.top()->getQuit()) {
            states_.top()->endState();
            delete states_.top();
            states_.pop();
        }
    }
    else {
        endApplication();
        window_->close();
    }
    updateSFMLEvents();
}

void Game::render() {
    window_->clear();

    if (!states_.empty())
        states_.top()->render(window_);

    window_->display();
}

void Game::run() {
    while (window_->isOpen()) {
        updateDeltaTime();
        update();
        render();
    }
}

void Game::updateDeltaTime() {
    deltaTime_ = dtClock_.restart().asSeconds();
}

void Game::updateSFMLEvents() {
    while (window_->pollEvent(sfEvent_)) {
        if (sfEvent_.type == sf::Event::Closed) {
            endApplication();
            window_->close();
        }
    }
}
