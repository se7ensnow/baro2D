#include "settings_state.h"

SettingsState::SettingsState(sf::RenderWindow* window,
                             std::map<std::string, int>* supportedKeys,
                             std::stack<State*>* states) : State(window, supportedKeys, states) {
    initVariables();
    initBackground();
    initFonts();
    initKeybinds();
    initGui();
    initText();

}

SettingsState::~SettingsState() {

}

void SettingsState::initVariables() {
    modes_ = sf::VideoMode::getFullscreenModes();
}

void SettingsState::initBackground() {
    background_.setSize(sf::Vector2f(window_->getSize()));
    if (!bgTexture_.loadFromFile("../../Resources/Images/Backgrounds/main_menu_bg.png")) {
        std::cerr <<"ERROR::MAINMENUSTATE::Could not load background texture!" <<std::endl;
    }
    background_.setTexture(&bgTexture_);
}

void SettingsState::initFonts() {
    if (!font_.loadFromFile("../../Fonts/Silkscreen-Bold.ttf")) {
        std::cerr <<"ERROR::MAINMENUSTATE::Could not load font!" <<std::endl;
    }
}

void SettingsState::initKeybinds() {
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

void SettingsState::initGui() {
    buttons_["BACK"] = std::make_unique<Button>(window_->getSize().x - 350, window_->getSize().y - 100, 250, 50,
                                                &font_, "Back", 50,
                                                sf::Color(70, 70, 70, 200),
                                                sf::Color(250, 250, 250, 250),
                                                sf::Color(20, 20, 20, 50),
                                                sf::Color(70, 70, 70, 0),
                                                sf::Color(150, 150, 150, 0),
                                                sf::Color(20, 20, 20, 0));

    buttons_["APPLY"] = std::make_unique<Button>(window_->getSize().x - 600, window_->getSize().y - 100, 250, 50,
                                                 &font_, "Apply", 50,
                                                 sf::Color(70, 70, 70, 200),
                                                 sf::Color(250, 250, 250, 250),
                                                 sf::Color(20, 20, 20, 50),
                                                 sf::Color(70, 70, 70, 0),
                                                 sf::Color(150, 150, 150, 0),
                                                 sf::Color(20, 20, 20, 0));

    std::vector<std::string> modes_str;
    for (auto& i : modes_) {
        modes_str.push_back(std::to_string(i.width) + 'x' + std::to_string(i.height));
    }
    dropDownLists_["RESOLUTION"] = std::make_unique<DropDownList>(110, 150, 250, 50, font_, modes_str);
}

void SettingsState::initText() {
    optionsText_.setFont(font_);
    optionsText_.setPosition(100.f, 100.f);
    optionsText_.setCharacterSize(30);
    optionsText_.setFillColor(sf::Color(255, 255, 255, 200));

    optionsText_.setString("Resolution \nFullscreen \nVsync \nAntialiasing \n");
}

void SettingsState::update(const float& dt) {
    updateMousePositions();
    updateInput(dt);
    updateGui();
}

void SettingsState::render(sf::RenderTarget* target) {
    if (!target) {
        target = window_;
    }

    target->draw(background_);
    target->draw(optionsText_);
    renderGui(target);

    sf::Text mouseText;
    mouseText.setPosition(mousePosView_.x, mousePosView_.y - 50);
    mouseText.setFont(font_);
    mouseText.setCharacterSize(12);
    mouseText.setString(std::to_string((int)mousePosView_.x) + " " + std::to_string((int)mousePosView_.y));
    target->draw(mouseText);
}

void SettingsState::updateInput(const float &dt) {

}

void SettingsState::updateGui() {
    for (auto& it : buttons_) {
        it.second->update(mousePosView_);
    }

    if (buttons_["BACK"]->isPressed()) {
        endState();
    }

    if (buttons_["APPLY"]->isPressed()) {
        window_->create(modes_[dropDownLists_["RESOLUTION"]->getActiveElementId()], "test", sf::Style::Default);
    }

    for (auto& it : dropDownLists_) {
        it.second->update(mousePosView_);
    }
}

void SettingsState::renderGui(sf::RenderTarget *target) {
    for (auto& it : buttons_) {
        it.second->render(target);
    }

    for (auto& it : dropDownLists_) {
        it.second->render(target);
    }
}
