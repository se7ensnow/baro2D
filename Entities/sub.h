#pragma once

#include "entity.h"
#include "../Components/movement_component.h"

class Sub : public Entity {
public:
    Sub(float x, float y, sf::Texture& texture);
    ~Sub();

    void move(const float& dir_x, const float& dir_y, const float& dt) override;

    void update(const float& dt) override;

protected:
    void initVariables() override;
    void createMovementComponent(sf::Sprite& sprite,
                                 float maxAccelerationX, float maxAccelerationY,
                                 float pumpSpeed,
                                 float decelerationCoef);
    void initComponents();

protected:
    MovementComponent* movementComponent_;
};
