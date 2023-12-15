#include "control.h"

Control::Control(float x, float y, float maxRadius, float offset, sf::Texture& texture)
: maxRadius_(maxRadius), offset_(offset) {
    centre_ = sf::Vector2f(x, y);

    vector_ = sf::Vector2f(0, 0);

    arrow_.setTexture(texture);
    arrow_.setOrigin(0, arrow_.getGlobalBounds().height / 2.f);
    arrow_.setScale(vector_.x / arrow_.getLocalBounds().width, 0.03f);
    arrow_.setPosition(centre_.x, centre_.y);


}

Control::~Control() {

}

sf::Vector2f Control::update(const sf::Vector2f& mousePos) {
    if (contains(mousePos)) {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            sf::Vector2f delta_vec = (mousePos - centre_);
            if (math::getLength(delta_vec / maxRadius_) < 1) {
                vector_ = delta_vec / maxRadius_;
            } else {
                vector_ = math::getNormalized(delta_vec);
            }
        }
    }
    updateArrow();
    return vector_;
}

void Control::render(sf::RenderTarget* target) {
    target->draw(arrow_);
}

bool Control::contains(const sf::Vector2f& mousePos) const {
    return math::getLengthSqr(mousePos - centre_) <= (maxRadius_ + offset_)*(maxRadius_ + offset_);
}

void Control::updateArrow() {
    arrow_.setScale(math::getLength(vector_) * maxRadius_ / arrow_.getLocalBounds().width, 0.03f);
    arrow_.setRotation(math::radToDeg(std::atan2(vector_.y, vector_.x)));
}
