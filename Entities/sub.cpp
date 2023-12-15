#include "sub.h"

Sub::Sub(float x, float y,
         sf::Texture& texture, float scale,
         sf::RenderWindow& window, const Map& map,
         sf::Font& font, sf::Font& secondFont,
         sf::SoundBuffer& buffer,
         const std::map<std::string, int>& keybinds,
         sf::Texture& robotTexture)
: map_(map), font_(font), secondFont_(secondFont), buffer_(buffer), keybinds_(keybinds) {
    Sub::initVariables();

    setPosition(x, y);

    initSprite(texture,
               static_cast<float>(window.getSize().x) / 2.f,
               static_cast<float>(window.getSize().y) / 2.f,
               scale);
    windowSize_ = window.getSize();

    initBreakables();
    initRobots(robotTexture);
    initComponents();
    initUI();
    initSound();
    initButtons();
}

Sub::~Sub() = default;

sf::Vector2f Sub::getPosition() const {
    return position_;
}

void Sub::initVariables() {
    hitboxComponent_ = nullptr;
    movementComponent_ = nullptr;
    pseudoRandom_ = std::mt19937(std::random_device{}());
}

void Sub::initSprite(sf::Texture &texture, float x, float y, float scale) {
    sprite_.setTexture(texture);
    sprite_.setScale(scale, scale);
    sprite_.setPosition(x - sprite_.getGlobalBounds().width / 2.f,
                        y - sprite_.getGlobalBounds().height / 2.f);
}

void Sub::initUI() {
    horSpeedContainer_.setSize(sf::Vector2f(250.f, 50.f));
    horSpeedContainer_.setFillColor(sf::Color(20, 20, 20, 100));
    horSpeedContainer_.setOrigin(125.f, 25.f);
    horSpeedContainer_.setPosition(1600, 200);
    horSpeedText_.setFont(secondFont_);
    horSpeedText_.setCharacterSize(30);
    horSpeedText_.setString("0 m/s");
    horSpeedText_.setPosition(
            horSpeedContainer_.getPosition().x - horSpeedText_.getGlobalBounds().width / 2.f - horSpeedText_.getGlobalBounds().left,
            horSpeedContainer_.getPosition().y - horSpeedText_.getGlobalBounds().height / 2.f - horSpeedText_.getGlobalBounds().top);
    horSpeedLabel_.setFont(font_);
    horSpeedLabel_.setCharacterSize(25);
    horSpeedLabel_.setString("Velocity:");
    horSpeedLabel_.setPosition(1475, 150);

    vertSpeedContainer_.setSize(sf::Vector2f(250.f, 50.f));
    vertSpeedContainer_.setFillColor(sf::Color(20, 20, 20, 100));
    vertSpeedContainer_.setOrigin(125.f, 25.f);
    vertSpeedContainer_.setPosition(1600, 300);
    vertSpeedText_.setFont(secondFont_);
    vertSpeedText_.setCharacterSize(30);
    vertSpeedText_.setString("0 m/s");
    vertSpeedText_.setPosition(
            vertSpeedContainer_.getPosition().x - vertSpeedText_.getGlobalBounds().width / 2.f - vertSpeedText_.getGlobalBounds().left,
            vertSpeedContainer_.getPosition().y - vertSpeedText_.getGlobalBounds().height / 2.f - vertSpeedText_.getGlobalBounds().top);
    vertSpeedLabel_.setFont(font_);
    vertSpeedLabel_.setCharacterSize(25);
    vertSpeedLabel_.setString("Descent velocity:");
    vertSpeedLabel_.setPosition(1475, 250);

    depthContainer_.setSize(sf::Vector2f(250.f, 50.f));
    depthContainer_.setFillColor(sf::Color(20, 20, 20, 100));
    depthContainer_.setOrigin(125.f, 25.f);
    depthContainer_.setPosition(1600, 400);
    depthText_.setFont(secondFont_);
    depthText_.setCharacterSize(30);
    depthText_.setString("1000 m");
    depthText_.setPosition(
            depthContainer_.getPosition().x - depthText_.getGlobalBounds().width / 2.f - depthText_.getGlobalBounds().left,
            depthContainer_.getPosition().y - depthText_.getGlobalBounds().height / 2.f - depthText_.getGlobalBounds().top);
    depthLabel_.setFont(font_);
    depthLabel_.setCharacterSize(25);
    depthLabel_.setString("Depth:");
    depthLabel_.setPosition(1475, 350);

    ballastContainer_.setSize(sf::Vector2f(250.f, 50.f));
    ballastContainer_.setFillColor(sf::Color(20, 20, 20, 100));
    ballastContainer_.setOrigin(125.f, 50.f);
    ballastContainer_.setPosition(1600, 525);
    ballastLevel_.setSize(sf::Vector2f(250.f, 50.f));
    ballastLevel_.setFillColor(sf::Color(20, 20, 200, 100));
    ballastLevel_.setOrigin(125.f, 50.f);
    ballastLevel_.setPosition(1600, 525);
    ballastLabel_.setFont(font_);
    ballastLabel_.setCharacterSize(25);
    ballastLabel_.setString("Ballast water level:");
    ballastLabel_.setPosition(1475, 450);
}

void Sub::initSound() {
    reactorAlarm_ = sf::Sound(buffer_);
    reactorAlarm_.setLoop(true);
}


void Sub::initButtons() {
    breakablesUI_["REACTOR"].setFont(font_);
    breakablesUI_["REACTOR"].setPosition(50, 50);
    breakablesUI_["REACTOR"].setCharacterSize(40);
    breakablesUI_["REACTOR"].setFillColor(sf::Color::Green);
    breakablesUI_["REACTOR"].setString("Reactor working");

    breakablesUI_["ENGINE"].setFont(font_);
    breakablesUI_["ENGINE"].setPosition(50, 150);
    breakablesUI_["ENGINE"].setCharacterSize(40);
    breakablesUI_["ENGINE"].setFillColor(sf::Color::Green);
    breakablesUI_["ENGINE"].setString("Engine running");

    breakablesUI_["SONAR"].setFont(font_);
    breakablesUI_["SONAR"].setPosition(50, 250);
    breakablesUI_["SONAR"].setCharacterSize(40);
    breakablesUI_["SONAR"].setFillColor(sf::Color::Green);
    breakablesUI_["SONAR"].setString("Sonar working");

    breakablesUI_["SENSORS"].setFont(font_);
    breakablesUI_["SENSORS"].setPosition(50, 350);
    breakablesUI_["SENSORS"].setCharacterSize(40);
    breakablesUI_["SENSORS"].setFillColor(sf::Color::Green);
    breakablesUI_["SENSORS"].setString("Sensors working");

    breakablesUI_["HULL"].setFont(font_);
    breakablesUI_["HULL"].setPosition(50, 450);
    breakablesUI_["HULL"].setCharacterSize(40);
    breakablesUI_["HULL"].setFillColor(sf::Color::Green);
    breakablesUI_["HULL"].setString("Hull is intact");

    breakablesUI_["PUMPS"].setFont(font_);
    breakablesUI_["PUMPS"].setPosition(50, 550);
    breakablesUI_["PUMPS"].setCharacterSize(40);
    breakablesUI_["PUMPS"].setFillColor(sf::Color::Green);
    breakablesUI_["PUMPS"].setString("Pumps working");
}

void Sub::initBreakables() {
    breakables_["REACTOR"] = 0;
    breakables_["ENGINE"] = 0;
    breakables_["SONAR"] = 0;
    breakables_["SENSORS"] = 0;
    breakables_["HULL"] = 0;
    breakables_["PUMPS"] = 0;

    repairTimers_["REACTOR"] = 6.f;
    repairTimers_["ENGINE"] = 4.f;
    repairTimers_["SONAR"] = 4.f;
    repairTimers_["SENSORS"] = 10.f;
    repairTimers_["HULL"] = 3.f;
    repairTimers_["PUMPS"] = 4.f;

    reactorTimer_ = 0.f;
    reactorMaxTimer_ = 10.f;
    lost_ = false;

    hullMaxTimer_ = 2.f;
    hullTimer_ = hullMaxTimer_;

    background_.setSize(sf::Vector2f(static_cast<float>(windowSize_.x), static_cast<float>(windowSize_.y)));
    background_.setFillColor(sf::Color(150, 0, 0, 50));
    alarmTimer_ = 0;
    maxAlarmTimer_ = 0.33f;
}

void Sub::initRobots(sf::Texture& robotTexture) {
    robots_.emplace_back("AVAILABLE", robotTexture, secondFont_);
    robots_.emplace_back("AVAILABLE", robotTexture, secondFont_);
}

void Sub::setPosition(float x, float y) {
    position_ = sf::Vector2f(x, y);
}

bool Sub::breakSonarSignal() const {
    return breakables_.at("SONAR") != 0;
}

bool Sub::endGameSignal() const {
    return lost_;
}

void Sub::createMovementComponent(sf::Sprite& sprite,
                                  float maxAccelerationX, float maxAccelerationY,
                                  float pumpSpeed, float hullSpeed,
                                  float decelerationCoef) {
    movementComponent_ = std::make_unique<MovementComponent>(maxAccelerationX, maxAccelerationY,
                                                             pumpSpeed, hullSpeed,
                                                             decelerationCoef, breakables_);
}

void Sub::createHitboxComponent(float width, float height, float offset_x, float offset_y) {
    hitboxComponent_ = std::make_unique<HitboxComponent>(width, height, offset_x, offset_y, map_, position_);
}

void Sub::initComponents() {
    createMovementComponent(sprite_, 20.f, 20.f, 0.5f, 0.2f, 0.003f);
    createHitboxComponent(103.f, 28.f, 0.f, -8.f);
}

void Sub::move(const sf::Vector2f& dir, const float& dt) {
    movementComponent_->move(dir, dt);
}

void Sub::update(const float &dt, const sf::Vector2f& mousePos) {
    sf::Vector2f delta = movementComponent_->update(dt);
    position_ += delta;

    hitboxComponent_->update();
    if (!hitboxComponent_->inBounds()) {
        if (hullTimer_ >= hullMaxTimer_ && breakables_["HULL"] < 4) {
            breakables_["HULL"]++;
            hullTimer_ = 0;
        }
        position_ -= delta;
        movementComponent_->resetVelocity();
        hitboxComponent_->update();
    }
    updateUI();
    updateButtons(mousePos);
    updateBreakables(dt);
}

void Sub::render(sf::RenderTarget* target) {
    target->draw(sprite_);
    hitboxComponent_->render(target);
    renderUI(target);
    renderButtons(target);
    renderRobots(target);
}

void Sub::updateUI() {
    if (breakables_["SENSORS"] == 0) {
        std::string horSpeedString =
                std::to_string(static_cast<int32_t>(std::floor(movementComponent_->getVelocity().x))) + " m/s";
        horSpeedText_.setString(horSpeedString);

        std::string vertSpeedString =
                std::to_string(static_cast<int32_t>(std::floor(movementComponent_->getVelocity().y))) + " m/s";
        vertSpeedText_.setString(vertSpeedString);

        std::string depthString = std::to_string(static_cast<int32_t>(std::floor(position_.y))) + " m";
        depthText_.setString(depthString);

        ballastLevel_.setScale(1.f, 0.5f + movementComponent_->getCurBallastLevel() / 2.f);
    } else {
        std::string horSpeedString = "-- m/s";
        horSpeedText_.setString(horSpeedString);

        std::string vertSpeedString = "-- m/s";
        vertSpeedText_.setString(vertSpeedString);

        std::string depthString = "-- m";
        depthText_.setString(depthString);

        ballastLevel_.setScale(1.f, 0.f);
    }
}

void Sub::updateButtons(const sf::Vector2f& mousePos) {
    for (auto& it : breakablesUI_) {
        if (it.second.getGlobalBounds().contains(mousePos)) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(keybinds_.at("SELECT_LEFT"))) &&
                robots_[1].state_ != it.first) {
                robots_[0].state_ = it.first;
                robots_[0].maxTimer_ = repairTimers_[robots_[0].state_];
                robots_[0].curTimer_ = 0;
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(keybinds_.at("SELECT_RIGHT"))) &&
                robots_[0].state_ != it.first) {
                robots_[1].state_ = it.first;
                robots_[1].maxTimer_ = repairTimers_[robots_[1].state_];
                robots_[1].curTimer_ = 0;
            }
        }
    }
}

void Sub::renderButtons(sf::RenderTarget* target) {
    for (auto& it : breakablesUI_) {
        target->draw(it.second);
    }
    if (alarmTimer_ >= maxAlarmTimer_) {
        target->draw(background_);
    }
}

void Sub::renderUI(sf::RenderTarget* target) {
    target->draw(horSpeedContainer_);
    target->draw(horSpeedText_);
    target->draw(horSpeedLabel_);
    target->draw(vertSpeedContainer_);
    target->draw(vertSpeedText_);
    target->draw(vertSpeedLabel_);
    target->draw(depthContainer_);
    target->draw(depthText_);
    target->draw(depthLabel_);
    target->draw(ballastContainer_);
    target->draw(ballastLevel_);
    target->draw(ballastLabel_);
}

void Sub::updateBreakables(const float& dt) {
    if (hullTimer_ < hullMaxTimer_) {
        hullTimer_ += dt;
    }
    if (pseudoRandom_() % 100000 <= 1 && breakables_["REACTOR"] == 0) {
        breakables_["REACTOR"] = 1;
        reactorAlarm_.play();
    }
    if (pseudoRandom_() % 50000 <= 1 && breakables_["ENGINE"] == 0) {
        breakables_["ENGINE"] = 1;
    }
    if (pseudoRandom_() % 50000 <= 1 && breakables_["SONAR"] == 0) {
        breakables_["SONAR"] = 1;
    }
    if (pseudoRandom_() % 50000 <= 1 && breakables_["SENSORS"] == 0) {
        breakables_["SENSORS"] = 1;
    }
    if (pseudoRandom_() % 50000 <= 1 && breakables_["PUMPS"] == 0) {
        breakables_["PUMPS"] = 1;
    }
    for (size_t i = 0 ; i < 2; ++i) {
        robots_[i].curTimer_ += dt;
        if (robots_[i].curTimer_ >= robots_[i].maxTimer_) {
            breakables_[robots_[i].state_] = 0;
            if (robots_[i].state_ == "REACTOR") {
                reactorAlarm_.stop();
            }
            robots_[i].curTimer_ = 0;
            robots_[i].state_ = "AVAILABLE";
        }
    }
    if (robots_[0].state_ == "AVAILABLE") {
        robots_[0].robotSprite_.setPosition(50, 800);
    } else {
        robots_[0].robotSprite_.setPosition(sf::Vector2f(0,breakablesUI_[robots_[0].state_].getPosition().y));
        robots_[0].timer_.setPosition(sf::Vector2f(10,
                                                   breakablesUI_[robots_[0].state_].getPosition().y +
                                                   robots_[0].robotSprite_.getGlobalBounds().height));
        std::stringstream ss;
        ss << std::fixed << std::setprecision(2) << robots_[0].maxTimer_ - robots_[0].curTimer_;
        robots_[0].timer_.setString(ss.str());
    }
    if (robots_[1].state_ == "AVAILABLE") {
        robots_[1].robotSprite_.setPosition(150, 800);
    } else {
        robots_[1].robotSprite_.setPosition(sf::Vector2f(0,breakablesUI_[robots_[1].state_].getPosition().y));
        robots_[1].timer_.setPosition(sf::Vector2f(10,
                                                   breakablesUI_[robots_[1].state_].getPosition().y +
                                                   robots_[1].robotSprite_.getGlobalBounds().height));
        std::stringstream ss;
        ss << std::fixed << std::setprecision(2) << robots_[1].maxTimer_ - robots_[1].curTimer_;
        robots_[1].timer_.setString(ss.str());
    }
    if (breakables_["REACTOR"] > 0) {
        breakablesUI_["REACTOR"].setFillColor(sf::Color::Red);
        breakablesUI_["REACTOR"].setString("Reactor overheating");
        reactorTimer_ += dt;
        alarmTimer_ += dt;
        if (alarmTimer_ >= maxAlarmTimer_ * 2) {
            alarmTimer_ = 0;
        }
        if (reactorTimer_ >= reactorMaxTimer_) {
            lost_ = true;
        }
    } else {
        breakablesUI_["REACTOR"].setFillColor(sf::Color::Green);
        breakablesUI_["REACTOR"].setString("Reactor working");
    }
    if (breakables_["ENGINE"] > 0) {
        breakablesUI_["ENGINE"].setFillColor(sf::Color::Red);
        breakablesUI_["ENGINE"].setString("Engine broken");
    } else {
        breakablesUI_["ENGINE"].setFillColor(sf::Color::Green);
        breakablesUI_["ENGINE"].setString("Engine running");
    }
    if (breakables_["SONAR"] > 0) {
        breakablesUI_["SONAR"].setFillColor(sf::Color::Red);
        breakablesUI_["SONAR"].setString("Sonar broken");
    } else {
        breakablesUI_["SONAR"].setFillColor(sf::Color::Green);
        breakablesUI_["SONAR"].setString("Sonar working");
    }
    if (breakables_["SENSORS"] > 0) {
        breakablesUI_["SENSORS"].setFillColor(sf::Color::Red);
        breakablesUI_["SENSORS"].setString("Sensors broken");
    } else {
        breakablesUI_["SENSORS"].setFillColor(sf::Color::Green);
        breakablesUI_["SENSORS"].setString("Sensors working");
    }
    if (breakables_["HULL"] > 0) {
        breakablesUI_["HULL"].setFillColor(sf::Color::Red);
        breakablesUI_["HULL"].setString("Hull breaches: " + std::to_string(breakables_["HULL"]));
    } else {
        breakablesUI_["HULL"].setFillColor(sf::Color::Green);
        breakablesUI_["HULL"].setString("Hull is intact");
    }
    if (breakables_["PUMPS"] > 0) {
        breakablesUI_["PUMPS"].setFillColor(sf::Color::Red);
        breakablesUI_["PUMPS"].setString("Pumps broken");
    } else {
        breakablesUI_["PUMPS"].setFillColor(sf::Color::Green);
        breakablesUI_["PUMPS"].setString("Pumps working");
    }
}

void Sub::renderRobots(sf::RenderTarget* target) {
    for (size_t i = 0; i < 2; ++i) {
        target->draw(robots_[i].robotSprite_);
        if (robots_[i].state_ != "AVAILABLE") {
            target->draw(robots_[i].timer_);
        }
    }
}

