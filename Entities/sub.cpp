#include "sub.h"

Sub::Sub() {
    initTexture();
    initSprite();
}

Sub::~Sub() = default;

void Sub::initTexture() {
    if (!textureSheet_.loadFromFile("../../Textures/submarine_texture.png")) {
        std::cerr << "ERROR::SUB::Could not load sub sheet!" <<std::endl;
    }
}

void Sub::initSprite() {
    sprite_.setTexture(textureSheet_);
    sprite_.setScale(0.1f, 0.1f);
}

void Sub::update() {

}

void Sub::render(sf::RenderTarget &target) {
    target.draw(sprite_);
}

void Sub::updatePosition() {

}
