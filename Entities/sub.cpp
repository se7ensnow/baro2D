#include "sub.h"

Sub::Sub(float x, float y,
         sf::Texture& texture, float scale,
         sf::RenderWindow& window, const Map& map,
         sf::Font& font, sf::Font& secondFont,
         sf::SoundBuffer& buffer)
: map_(map), font_(font), secondFont_(secondFont), buffer_(buffer) {
    Sub::initVariables();

    setPosition(x, y);

    initSprite(texture,
               static_cast<float>(window.getSize().x) / 2.f,
               static_cast<float>(window.getSize().y) / 2.f,
               scale);
    windowSize_ = window.getSize();

    initBreakables();
    initRobots();
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

    buttons_["REACTOR"] = std::make_unique<Button>(breakablesUI_["REACTOR"], sf::Color(0, 200, 0, 200));
    buttons_["ENGINE"] = std::make_unique<Button>(breakablesUI_["ENGINE"], sf::Color(0, 200, 0, 200));
    buttons_["SONAR"] = std::make_unique<Button>(breakablesUI_["SONAR"], sf::Color(0, 200, 0, 200));
    buttons_["SENSORS"] = std::make_unique<Button>(breakablesUI_["SENSORS"], sf::Color(0, 200, 0, 200));
    buttons_["HULL"] = std::make_unique<Button>(breakablesUI_["HULL"], sf::Color(0, 200, 0, 200));
    buttons_["PUMPS"] = std::make_unique<Button>(breakablesUI_["PUMPS"], sf::Color(0, 200, 0, 200));

    buttons_["ROBOT_1"] = std::make_unique<Button>(110, 300, 250, 50,
                                                      &font_, "New Game", 50,
                                                      sf::Color(70, 70, 70, 200),
                                                      sf::Color(250, 250, 250, 250),
                                                      sf::Color(20, 20, 20, 50),
                                                      sf::Color(70, 70, 70, 0),
                                                      sf::Color(150, 150, 150, 0),
                                                      sf::Color(20, 20, 20, 0));

    buttons_["ROBOT_2"] = std::make_unique<Button>(110, 300, 250, 50,
                                                      &font_, "New Game", 50,
                                                      sf::Color(70, 70, 70, 200),
                                                      sf::Color(250, 250, 250, 250),
                                                      sf::Color(20, 20, 20, 50),
                                                      sf::Color(70, 70, 70, 0),
                                                      sf::Color(150, 150, 150, 0),
                                                      sf::Color(20, 20, 20, 0));
}

void Sub::initBreakables() {
    breakables_["REACTOR"] = 0;
    breakables_["ENGINE"] = 0;
    breakables_["SONAR"] = 0;
    breakables_["SENSORS"] = 0;
    breakables_["HULL"] = 0;
    breakables_["PUMPS"] = 0;

    repairTimers_["REACTOR"] = 3.f;
    repairTimers_["ENGINE"] = 3.f;
    repairTimers_["SONAR"] = 2.f;
    repairTimers_["SENSORS"] = 5.f;
    repairTimers_["HULL"] = 1.f;
    repairTimers_["PUMPS"] = 3.f;

    reactorTimer_ = 0.f;
    reactorMaxTimer_ = 7.f;
    lost_ = false;

    hullMaxTimer_ = 2.f;
    hullTimer_ = hullMaxTimer_;

    background_.setSize(sf::Vector2f(static_cast<float>(windowSize_.x), static_cast<float>(windowSize_.y)));
    background_.setFillColor(sf::Color(150, 0, 0, 50));
    alarmTimer_ = 0;
    maxAlarmTimer_ = 0.33f;
}

void Sub::initRobots() {
    robots_.emplace_back("AVAILABLE");
    robots_.emplace_back("AVAILABLE");
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
    for (auto& it : buttons_) {
        it.second->update(mousePos);
    }
}

void Sub::renderButtons(sf::RenderTarget* target) {
    for (auto& it : buttons_) {
        it.second->render(target);
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
    if (breakables_["REACTOR"] > 0) {
        buttons_["REACTOR"]->setTextColor(sf::Color(200, 0, 0, 200));
        buttons_["REACTOR"]->setText("Reactor overheating");
        reactorTimer_ += dt;
        alarmTimer_ += dt;
        if (alarmTimer_ >= maxAlarmTimer_ * 2) {
            alarmTimer_ = 0;
        }
        if (reactorTimer_ >= reactorMaxTimer_) {
            lost_ = true;
        }
    }
    if (breakables_["ENGINE"] > 0) {
        buttons_["ENGINE"]->setTextColor(sf::Color(200, 0, 0, 200));
        buttons_["ENGINE"]->setText("Engine broken");
    }
    if (breakables_["SONAR"] > 0) {
        buttons_["SONAR"]->setTextColor(sf::Color(200, 0, 0, 200));
        buttons_["SONAR"]->setText("Sonar broken");
    }
    if (breakables_["SENSORS"] > 0) {
        buttons_["SENSORS"]->setTextColor(sf::Color(200, 0, 0, 200));
        buttons_["SENSORS"]->setText("Sensors broken");
    }
    if (breakables_["HULL"] > 0) {
        buttons_["HULL"]->setTextColor(sf::Color(200, 0, 0, 200));
        buttons_["HULL"]->setText("Hull breaches: " + std::to_string(breakables_["HULL"]));
    }
    if (breakables_["PUMPS"] > 0) {
        buttons_["PUMPS"]->setTextColor(sf::Color(200, 0, 0, 200));
        buttons_["PUMPS"]->setText("Pumps broken");
    }
}

