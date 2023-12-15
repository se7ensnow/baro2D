#include "game_state.h"

GameState::GameState(sf::RenderWindow* window,
                     std::map<std::string, int>* supportedKeys,
                     std::stack<State*>* states)
    : State(window, supportedKeys, states) {
    pmenu_ = nullptr;
    initKeybinds();
    initFonts();
    initTextures();
    initSounds();
    initPauseMenu();
    initMap();
    initControl();
    initSub();
    initSonar();
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

void GameState::initFonts() {
    if (!font_.loadFromFile("../../Resources/Fonts/Silkscreen-Bold.ttf")) {
        std::cerr <<"ERROR::GAMESTATE::Could not load font!" <<std::endl;
    }
    if (!secondFont_.loadFromFile("../../Resources/Fonts/Silkscreen-Regular.ttf")) {
        std::cerr <<"ERROR::GAMESTATE::Could not load font!" <<std::endl;
    }
}

void GameState::initTextures() {
    if (!textures_["SUBMARINE_IDLE"].loadFromFile("../../Resources/Images/Sprites/Sub/submarine_texture.png")) {
        std::cerr <<"ERROR::GAMESTATE::Could not load submarine texture!" <<std::endl;
    }
    if (!textures_["ARROW"].loadFromFile("../../Resources/Images/Sprites/Sonar/arrow_texture.png")) {
        std::cerr <<"ERROR::GAMESTATE::Could not load arrow texture!" <<std::endl;
    }
}

void GameState::initSounds() {
    if (!sounds_["SONAR_WAVE_1"].loadFromFile("../../Resources/Audio/Sonar/wave1.wav")) {
        std::cerr <<"ERROR::GAMESTATE::Could not load wave1 sound!" <<std::endl;
    }
    if (!sounds_["SONAR_WAVE_2"].loadFromFile("../../Resources/Audio/Sonar/wave2.wav")) {
        std::cerr <<"ERROR::GAMESTATE::Could not load wave2 sound!" <<std::endl;
    }
    if (!sounds_["SONAR_WAVE_3"].loadFromFile("../../Resources/Audio/Sonar/wave3.wav")) {
        std::cerr <<"ERROR::GAMESTATE::Could not load wave3 sound!" <<std::endl;
    }
    if (!sounds_["SONAR_WAVE_4"].loadFromFile("../../Resources/Audio/Sonar/wave4.wav")) {
        std::cerr <<"ERROR::GAMESTATE::Could not load wave4 sound!" <<std::endl;
    }
    if (!sounds_["SONAR_WAVE_5"].loadFromFile("../../Resources/Audio/Sonar/wave5.wav")) {
        std::cerr <<"ERROR::GAMESTATE::Could not load wave5 sound!" <<std::endl;
    }
    if (!sounds_["SONAR_WAVE_6"].loadFromFile("../../Resources/Audio/Sonar/wave6.wav")) {
        std::cerr <<"ERROR::GAMESTATE::Could not load wave6 sound!" <<std::endl;
    }
    if (!sounds_["SONAR_WAVE_7"].loadFromFile("../../Resources/Audio/Sonar/wave7.wav")) {
        std::cerr <<"ERROR::GAMESTATE::Could not load wave7 sound!" <<std::endl;
    }
    if (!sounds_["SONAR_WAVE_8"].loadFromFile("../../Resources/Audio/Sonar/wave8.wav")) {
        std::cerr <<"ERROR::GAMESTATE::Could not load wave8 sound!" <<std::endl;
    }
    if (!sounds_["SONAR_WAVE_9"].loadFromFile("../../Resources/Audio/Sonar/wave9.wav")) {
        std::cerr <<"ERROR::GAMESTATE::Could not load wave9 sound!" <<std::endl;
    }
    if (!sounds_["SONAR_WAVE_10"].loadFromFile("../../Resources/Audio/Sonar/wave10.wav")) {
        std::cerr <<"ERROR::GAMESTATE::Could not load wave10 sound!" <<std::endl;
    }
    if (!sounds_["SONAR_WAVE_11"].loadFromFile("../../Resources/Audio/Sonar/wave11.wav")) {
        std::cerr <<"ERROR::GAMESTATE::Could not load wave11 sound!" <<std::endl;
    }
    if (!sounds_["SONAR_WAVE_12"].loadFromFile("../../Resources/Audio/Sonar/wave12.wav")) {
        std::cerr <<"ERROR::GAMESTATE::Could not load wave12 sound!" <<std::endl;
    }
    if (!sounds_["SONAR_WAVE_13"].loadFromFile("../../Resources/Audio/Sonar/wave13.wav")) {
        std::cerr <<"ERROR::GAMESTATE::Could not load wave13 sound!" <<std::endl;
    }
    if (!sounds_["SONAR_WAVE_14"].loadFromFile("../../Resources/Audio/Sonar/wave14.wav")) {
        std::cerr <<"ERROR::GAMESTATE::Could not load wave14 sound!" <<std::endl;
    }
    if (!sounds_["SONAR_WAVE_15"].loadFromFile("../../Resources/Audio/Sonar/wave15.wav")) {
        std::cerr <<"ERROR::GAMESTATE::Could not load wave15 sound!" <<std::endl;
    }
    if (!sounds_["SONAR_WAVE_16"].loadFromFile("../../Resources/Audio/Sonar/wave16.wav")) {
        std::cerr <<"ERROR::GAMESTATE::Could not load wave16 sound!" <<std::endl;
    }
    if (!sounds_["REACTOR_ALARM"].loadFromFile("../../Resources/Audio/Submarine/alarm.wav")) {
        std::cerr <<"ERROR::GAMESTATE::Could not load alarm sound!" <<std::endl;
    }
}

void GameState::initPauseMenu() {
    pmenu_ = std::make_unique<PauseMenu>(*window_, font_);

    pmenu_->addButton("EXIT_STATE", static_cast<float>(window_->getSize().y) - 200.f, "Quit");
}

void GameState::initSub() {
    sub_ = std::make_unique<Sub>(20 * 30.f, 10 * 30.f,
                                 textures_["SUBMARINE_IDLE"], 0.1f,
                                 *window_, *map_,
                                 font_, secondFont_,
                                 sounds_["REACTOR_ALARM"]);
}

void GameState::initControl() {
    control_ = std::make_unique<Control>(static_cast<float>(window_->getSize().x) / 2.f,
                                         static_cast<float>(window_->getSize().y) / 2.f,
                                         150, 450, textures_["ARROW"]);
}

void GameState::initMap() {
    map_ = std::make_unique<Map>(192, 108, 30.f,
                                 20, 10, 170, 90,
                                 45);
}

void GameState::initSonar() {
    std::vector<sf::SoundBuffer*> soundBuffers = {&sounds_["SONAR_WAVE_1"],
                                                 &sounds_["SONAR_WAVE_2"],
                                                 &sounds_["SONAR_WAVE_3"],
                                                 &sounds_["SONAR_WAVE_4"],
                                                 &sounds_["SONAR_WAVE_5"],
                                                 &sounds_["SONAR_WAVE_6"],
                                                 &sounds_["SONAR_WAVE_7"],
                                                 &sounds_["SONAR_WAVE_8"],
                                                 &sounds_["SONAR_WAVE_9"],
                                                 &sounds_["SONAR_WAVE_10"],
                                                 &sounds_["SONAR_WAVE_11"],
                                                 &sounds_["SONAR_WAVE_12"],
                                                 &sounds_["SONAR_WAVE_13"],
                                                 &sounds_["SONAR_WAVE_14"],
                                                 &sounds_["SONAR_WAVE_15"],
                                                 &sounds_["SONAR_WAVE_16"]};
    sonar_ = std::make_unique<Sonar>(static_cast<float>(window_->getSize().x) / 2.f,
                                     static_cast<float>(window_->getSize().y) / 2.f,
                                     400,
                                     400.f, 2.5f,
                                     soundBuffers,
                                     *map_, *sub_, font_);
}

void GameState::update(const float& dt) {
    updateMousePositions();
    updateInput(dt);

    if (!paused_) {
        updatePlayerInput(dt);
        sub_->update(dt, mousePosView_);
        if (sub_->endGameSignal()) {
            endState();
        }
        sonar_->update(dt);
    } else {
        pmenu_->update(mousePosView_);
        updatePauseMenuGui();
    }
}

void GameState::render(sf::RenderTarget* target) {
    if (!target) {
        target = window_;
    }

    sonar_->render(target);
    control_->render(target);
    sub_->render(target);

    if (paused_) {
        pmenu_->render(target);
    }
}

void GameState::updatePlayerInput(const float &dt) {
    sf::Vector2f dir = control_->update(mousePosView_);
    sub_->move(dir, dt);
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

