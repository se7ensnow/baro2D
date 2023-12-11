#include "entity.h"

Entity::Entity() {

}

Entity::~Entity() {
    delete hitboxComponent_;
}

void Entity::initVariables() {
    hitboxComponent_ = nullptr;
}

void Entity::setTexture(sf::Texture& texture) {
    sprite_.setTexture(texture);
}

void Entity::createHitboxComponent(sf::Sprite& sprite,
                                   float offset_x, float offset_y,
                                   float width, float height) {
    hitboxComponent_ = new HitboxComponent(sprite,
                                           offset_x, offset_y,
                                           width, height);
}

void Entity::setPosition(float x, float y) {
    sprite_.setPosition(x, y);
}

void Entity::update(const float &dt) {
    hitboxComponent_->update();
}

void Entity::render(sf::RenderTarget* target) {
    target->draw(sprite_);
    hitboxComponent_->render(target);

}


