#include "entity.h"

Entity::Entity() {

}

Entity::~Entity() {
}

void Entity::initVariables() {
}

void Entity::setTexture(sf::Texture &texture) {
    sprite_.setTexture(texture);
}

void Entity::update(const float &dt) {
}

void Entity::render(sf::RenderTarget* target) {
    target->draw(sprite_);
}


