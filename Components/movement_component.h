#pragma once

#include "../src/headers.h"

class MovementComponent {
public:
    MovementComponent(sf::Sprite& sprite,
                      float maxAccelerationX, float maxAccelerationY,
                      float pumpSpeed,
                      float decelerationCoef);
    ~MovementComponent();

    void move(float dir_x, float dir_y, const float& dt);
    void update(const float& dt);

protected:
    void initVariables();
    void updateBallast(const float& dt);
    void updateVelocity(const float& dt);

protected:
    sf::Sprite& sprite_;

    float maxAccelerationX_;
    float maxAccelerationY_;
    float decelerationCoef_;

    float curBallastLevel_;
    float desiredBallastLevel_;
    float pumpSpeed_;

    sf::Vector2f velocity_;
    sf::Vector2f acceleration_;
};
