#include "movement_component.h"

MovementComponent::MovementComponent(sf::Sprite& sprite,
                                     float maxAccelerationX, float maxAccelerationY,
                                     float pumpSpeed,
                                     float decelerationCoef)
: sprite_(sprite),
maxAccelerationX_(maxAccelerationX), maxAccelerationY_(maxAccelerationY),
pumpSpeed_(pumpSpeed),
decelerationCoef_(decelerationCoef) {
    initVariables();

}

MovementComponent::~MovementComponent() {

}

void MovementComponent::initVariables() {
    curBallastLevel_ = 0;
    desiredBallastLevel_ = 0;
    acceleration_ = sf::Vector2f(0.f, 0.f);
    velocity_ = sf::Vector2f(0.f, 0.f);
}

void MovementComponent::move(float dir_x, float dir_y, const float& dt) {
    acceleration_.x = maxAccelerationX_ * dir_x;
    desiredBallastLevel_ = dir_y;

}

void MovementComponent::update(const float &dt) {
    updateBallast(dt);
    updateVelocity(dt);

    sprite_.move(velocity_ * dt);
}

void MovementComponent::updateBallast(const float &dt) {
    float step = pumpSpeed_ * dt;
    if (curBallastLevel_ > desiredBallastLevel_ + step) {
        curBallastLevel_ -= step;
    } else if (curBallastLevel_ < desiredBallastLevel_ - step) {
        curBallastLevel_ += step;
    } else {
        curBallastLevel_ = desiredBallastLevel_;
    }
}

void MovementComponent::updateVelocity(const float &dt) {
    acceleration_.y = curBallastLevel_ * maxAccelerationY_;
    velocity_ += acceleration_ * dt;
    if (velocity_.x > 0) {
        velocity_.x -= decelerationCoef_ * std::pow(velocity_.x, 2.f) * dt;
    } else if (velocity_.x < 0) {
        velocity_.x += decelerationCoef_ * std::pow(velocity_.x, 2.f) * dt;
    }
    if (velocity_.y > 0) {
        velocity_.y -= decelerationCoef_ * std::pow(velocity_.y, 2.f) * dt;
    } else if (velocity_.y < 0) {
        velocity_.y += decelerationCoef_ * std::pow(velocity_.y, 2.f) * dt;
    }
}

