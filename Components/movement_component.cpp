#include "movement_component.h"

MovementComponent::MovementComponent(float maxAccelerationX, float maxAccelerationY,
                                     float pumpSpeed, float hullSpeed,
                                     float decelerationCoef,
                                     std::map<std::string, uint16_t>& breakables)
:maxAccelerationX_(maxAccelerationX), maxAccelerationY_(maxAccelerationY),
pumpSpeed_(pumpSpeed), hullSpeed_(hullSpeed),
decelerationCoef_(decelerationCoef),
breakables_(breakables) {
    initVariables();
}

MovementComponent::~MovementComponent() = default;

void MovementComponent::initVariables() {
    curBallastLevel_ = 0;
    desiredBallastLevel_ = 0;
    acceleration_ = sf::Vector2f(0.f, 0.f);
    velocity_ = sf::Vector2f(0.f, 0.f);
}

void MovementComponent::move(const sf::Vector2f& dir, const float& dt) {
    if (breakables_["ENGINE"] == 0) {
        acceleration_.x = maxAccelerationX_ * dir.x;
    } else {
        acceleration_.x = 0;
    }
    desiredBallastLevel_ = dir.y;

}

sf::Vector2f MovementComponent::update(const float &dt) {
    updateBallast(dt);
    updateVelocity(dt);

    return velocity_ * dt;
}

void MovementComponent::updateBallast(const float &dt) {
    if (breakables_["PUMPS"] == 0) {
        float step = pumpSpeed_ * dt;
        if (curBallastLevel_ > desiredBallastLevel_ + step) {
            curBallastLevel_ -= step;
        } else if (curBallastLevel_ < desiredBallastLevel_ - step) {
            curBallastLevel_ += step;
        } else {
            curBallastLevel_ = desiredBallastLevel_;
        }
        if (breakables_["HULL"] > 0) {
            curBallastLevel_ = std::min(1.f, curBallastLevel_ + hullSpeed_ * static_cast<float>(breakables_["HULL"]) * dt);
        }
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

void MovementComponent::resetVelocity(axis i, const float &dt) {
    if (i == BOTH) {
        velocity_ = sf::Vector2f(0, 0);
    } else if (i == X) {
        velocity_.x = 0;
        velocity_.y -= acceleration_.x * 0.8f * dt;
    } else {
        velocity_.x -= acceleration_.y * 0.8f * dt;
        velocity_.y = 0;
    }
}

sf::Vector2f MovementComponent::getVelocity() const {
    return velocity_;
}

float MovementComponent::getCurBallastLevel() const {
    return curBallastLevel_;
}

