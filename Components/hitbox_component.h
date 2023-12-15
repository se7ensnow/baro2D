#pragma once

#include "../src/headers.h"
#include "../Map/map.h"


class HitboxComponent {
public:
    HitboxComponent(float width, float height,
                    float offset_x, float offset_y,
                    const Map& map, const sf::Vector2f& position);
    ~HitboxComponent();

    bool checkIntersect(const sf::FloatRect& frect) const;
    bool inBounds() const;

    void update();
    void render(sf::RenderTarget* target);

protected:
    void initShape();

protected:
    float offsetX_;
    float offsetY_;

    sf::FloatRect hitbox_;
    const sf::Vector2f& position_;

    sf::RectangleShape hitboxShape_;

    const Map& map_;

};
