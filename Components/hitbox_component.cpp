#include "hitbox_component.h"

HitboxComponent::HitboxComponent(sf::Sprite &sprite,
                                 float offset_x, float offset_y,
                                 float width, float height)
                                 : sprite_(sprite), offsetX_(offset_x), offsetY_(offset_y) {
    hitbox_.setPosition(sprite_.getPosition().x + offsetX_, sprite_.getPosition().y + offsetY_);
    hitbox_.setSize(sf::Vector2f(width, height));
    hitbox_.setFillColor(sf::Color::Transparent);
    hitbox_.setOutlineThickness(1.f);
    hitbox_.setOutlineColor(sf::Color::Green);
}

HitboxComponent::~HitboxComponent() {

}

bool HitboxComponent::checkIntersect(const sf::FloatRect& frect) const {
    return hitbox_.getGlobalBounds().intersects(frect);
}

void HitboxComponent::update() {
    hitbox_.setPosition(sprite_.getPosition().x + offsetX_, sprite_.getPosition().y + offsetY_);
}

void HitboxComponent::render(sf::RenderTarget* target) {
    target->draw(hitbox_);
}


