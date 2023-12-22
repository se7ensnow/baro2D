#include "hitbox_component.h"

HitboxComponent::HitboxComponent(float width, float height,
                                 float offset_x, float offset_y,
                                 const Map& map)
                                 : map_(map), offsetX_(offset_x), offsetY_(offset_y) {
    hitbox_.width = width;
    hitbox_.height = height;

    initShape();
}

HitboxComponent::~HitboxComponent() = default;

void HitboxComponent::initShape() {
    hitboxShape_.setSize(sf::Vector2f(hitbox_.width, hitbox_.height));
    hitboxShape_.setOrigin(hitbox_.width / 2.f, hitbox_.height / 2.f);
    hitboxShape_.setPosition(960 + offsetX_, 540 + offsetY_);
    hitboxShape_.setFillColor(sf::Color::Transparent);
    hitboxShape_.setOutlineThickness(1.f);
    hitboxShape_.setOutlineColor(sf::Color::Green);
}

bool HitboxComponent::inBounds(const sf::Vector2f& pos) {
    hitbox_.left = pos.x - hitbox_.width / 2.f + offsetX_;
    hitbox_.top = pos.y - hitbox_.height / 2.f + offsetY_;
    float mapSquareSize =  map_.getSquareSize();
    int32_t left = std::floor(hitbox_.left / mapSquareSize);
    int32_t top = std::floor(hitbox_.top / mapSquareSize);
    int32_t right = std::floor((hitbox_.left + hitbox_.width) / mapSquareSize);
    int32_t bottom = std::floor((hitbox_.top + hitbox_.height) / mapSquareSize);
    if (left < 0 || right >= map_.getSize().x || top < 0 || bottom >= map_.getSize().y) {
        return false;
    }
    for (int32_t x = left; x <= right; ++x) {
        for (int32_t y = top; y <= bottom; ++y) {
            if (map_[y][x] == 1) {
                return false;
            }
        }
    }
    return true;
}

void HitboxComponent::render(sf::RenderTarget* target) {
    target->draw(hitboxShape_);
}


