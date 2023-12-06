#pragma once

#include "../src/headers.h"

class Sub {
public:
    Sub();
    ~Sub();
    void update();
    void render(sf::RenderTarget& target);

protected:
    void initTexture();
    void initSprite();
    void updatePosition();

protected:
    sf::Texture textureSheet_;
    sf::Sprite sprite_;

    sf::Vector2f position_;
    sf::Vector2f speed_;
    sf::Vector2f accel_;
};
