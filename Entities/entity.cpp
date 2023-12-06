#include "entity.h"

Entity::Entity() {
    initVariables();

}

Entity::~Entity() {
    delete sprite_;
}

void Entity::initVariables() {
    texture_ = nullptr;
    sprite_ = nullptr;
    speed_ = 100.f;
}

void Entity::createSprite(sf::Texture *texture) {
    texture_ = texture;
    sprite_->setTexture(*texture_);
}

void Entity::move(const float& dt, const float& dir_x, const float& dir_y) {
    if (sprite_) {
        sprite_->move(dir_x * speed_ * dt, dir_y * speed_ * dt);
    }
}

void Entity::update(const float &dt) {

}

void Entity::render(sf::RenderTarget *target) {
    if (sprite_) {
        target->draw(*sprite_);
    }
}


