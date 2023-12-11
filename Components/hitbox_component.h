#pragma once

#include "../src/headers.h"


class HitboxComponent {
public:
    HitboxComponent(sf::Sprite& sprite,
                    float offset_x, float offset_y,
                    float width, float height);
    ~HitboxComponent();

    bool checkIntersect(const sf::FloatRect& frect) const;

    void update();
    void render(sf::RenderTarget* target);

protected:
    sf::Sprite& sprite_;
    sf::RectangleShape hitbox_;

    float offsetX_;
    float offsetY_;

};
