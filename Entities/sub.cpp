#include "sub.h"

Sub::Sub(float x, float y, sf::Texture& texture) {
    Sub::initVariables();

    setTexture(texture);
    setPosition(x, y);

    sprite_.setScale(0.1f, 0.1f);
    initComponents();
}

Sub::~Sub() {
    delete movementComponent_;
};

void Sub::initVariables() {
    Entity::initVariables();
    movementComponent_ = nullptr;
}

void Sub::createMovementComponent(sf::Sprite& sprite,
                                  float maxAccelerationX, float maxAccelerationY,
                                  float pumpSpeed,
                                  float decelerationCoef) {
    movementComponent_ = new MovementComponent(sprite, maxAccelerationX, maxAccelerationY, pumpSpeed, decelerationCoef);
}

void Sub::initComponents() {
    createMovementComponent(sprite_, 20.f, 20.f, 1.f, 0.003f);
    createHitboxComponent(sprite_,
                          0.f, 34.f,
                          103.f, 28.f);
    std::cout <<sprite_.getGlobalBounds().width <<" " <<sprite_.getGlobalBounds().height <<std::endl;
}

void Sub::move(const float& dir_x, const float& dir_y, const float& dt) {
    if (movementComponent_) {
        movementComponent_->move(dir_x, dir_y, dt);
    }
}

void Sub::update(const float &dt) {
    Entity::update(dt);
    if (movementComponent_)
        movementComponent_->update(dt);
}