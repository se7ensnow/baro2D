#pragma once

#include <utility>

#include "entity.h"
#include "../Components/movement_component.h"
#include "../UI/gui.h"
#include "../Utils/sf_math.h"

class Sub {
public:
    struct Robot {
        std::string state_ = "AVAILABLE";
        float curTimer_ = 0;
        float maxTimer_ = 0;
        sf::Sprite robotSprite_;
        sf::Text timer_;
        Robot(std::string state, sf::Texture& texture, sf::Font& font) : state_(std::move(state)) {
            robotSprite_.setTexture(texture);
            robotSprite_.setScale(0.05f, 0.05f);
            timer_.setCharacterSize(10);
            timer_.setFont(font);
        };
    };
public:
    Sub(float x, float y,
        sf::Texture& texture, float scale,
        sf::RenderWindow& window, const Map& map,
        sf::Font& font, sf::Font& secondFont,
        sf::SoundBuffer& buffer,
        const std::map<std::string, int>& keybinds,
        sf::Texture& robotTexture);
    ~Sub();

    sf::Vector2f getPosition() const;
    bool breakSonarSignal() const;
    bool endGameSignal() const;

    void move(const sf::Vector2f& dir, const float& dt);

    void update(const float& dt, const sf::Vector2f& mousePos);
    void render(sf::RenderTarget* target);

protected:
    void initVariables();
    void initSprite(sf::Texture& texture, float x, float y, float scale);
    void initUI();
    void initSound();
    void initButtons();
    void initBreakables();
    void initRobots(sf::Texture& robotTexture);
    void setPosition(float x, float y);
    void createMovementComponent(sf::Sprite& sprite,
                                 float maxAccelerationX, float maxAccelerationY,
                                 float pumpSpeed, float hullSpeed,
                                 float decelerationCoef);
    void createHitboxComponent(float width, float height, float offset_x, float offset_y);
    void initComponents();
    void updateUI();
    void renderUI(sf::RenderTarget* target);
    void updateButtons(const sf::Vector2f& mousePos);
    void renderButtons(sf::RenderTarget* target);
    void updateBreakables(const float& dt);
    void renderRobots(sf::RenderTarget* target);

protected:
    std::unique_ptr<MovementComponent> movementComponent_;
    std::unique_ptr<HitboxComponent> hitboxComponent_;

    sf::Vector2f position_;

    const std::map<std::string, int>& keybinds_;

    const Map& map_;
    sf::Vector2f finish_;

    sf::Sprite sprite_;

    sf::Vector2u windowSize_;

    sf::Font& font_;
    sf::Font& secondFont_;

    sf::RectangleShape horSpeedContainer_;
    sf::Text horSpeedText_;
    sf::Text horSpeedLabel_;

    sf::RectangleShape vertSpeedContainer_;
    sf::Text vertSpeedText_;
    sf::Text vertSpeedLabel_;

    sf::RectangleShape depthContainer_;
    sf::Text depthText_;
    sf::Text depthLabel_;

    sf::RectangleShape ballastContainer_;
    sf::RectangleShape ballastLevel_;
    sf::Text ballastLabel_;

    std::map<std::string, uint16_t> breakables_;
    std::map<std::string, float> repairTimers_;
    std::mt19937 pseudoRandom_;
    float reactorTimer_;
    float reactorMaxTimer_;
    sf::RectangleShape background_;
    float alarmTimer_;
    float maxAlarmTimer_;
    bool lost_;
    sf::SoundBuffer& buffer_;
    sf::Sound reactorAlarm_;

    float hullTimer_;
    float hullMaxTimer_;

    std::map<std::string, sf::Text> breakablesUI_;
    std::map<std::string, std::unique_ptr<Button>> buttons_;

    std::vector<Robot> robots_;
};
