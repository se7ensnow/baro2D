#pragma once

#include "../src/headers.h"
#include "../Utils/sf_math.h"

class Control {
public:
    Control(float x, float y, float maxRadius, float offset, sf::Texture& texture);
    ~Control();

    sf::Vector2f update(const sf::Vector2f& mousePos);
    void render(sf::RenderTarget* target);

protected:
    bool contains(const sf::Vector2f& mousePos) const;

    void updateArrow();

protected:
    sf::Sprite arrow_;

    sf::Vector2f centre_;

    sf::Vector2f vector_;

    float maxRadius_;
    float offset_;
};
