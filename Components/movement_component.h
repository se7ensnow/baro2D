#pragma once

#include "../src/headers.h"

class MovementComponent {
public:
    enum axis{X=0, Y, BOTH};
public:
    MovementComponent(float maxAccelerationX, float maxAccelerationY,
                      float pumpSpeed, float hullSpeed,
                      float decelerationCoef,
                      std::map<std::string, uint16_t>& breakables);
    ~MovementComponent();

    void resetVelocity(axis i, const float &dt);
    sf::Vector2f getVelocity() const;
    float getCurBallastLevel() const;

    void move(const sf::Vector2f& dir, const float& dt);
    sf::Vector2f update(const float& dt);

protected:
    void initVariables();
    void updateBallast(const float& dt);
    void updateVelocity(const float& dt);

protected:
    float maxAccelerationX_;
    float maxAccelerationY_;
    float decelerationCoef_;

    float curBallastLevel_;
    float desiredBallastLevel_;
    float pumpSpeed_;
    float hullSpeed_;

    sf::Vector2f velocity_;
    sf::Vector2f acceleration_;

    std::map<std::string, uint16_t>& breakables_;
};
