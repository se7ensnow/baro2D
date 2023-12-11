#pragma once

#include "../Components/hitbox_component.h"

class Entity {
public:
    Entity();
    virtual ~Entity();

    virtual void setPosition(float x, float y);
    virtual void move(const float& dir_x, const float& dir_y, const float& dt) = 0;

    virtual void update(const float& dt);
    virtual void render(sf::RenderTarget* target);

protected:
    virtual void initVariables();
    void setTexture(sf::Texture& texture);
    void createHitboxComponent(sf::Sprite& sprite,
                               float offset_x, float offset_y,
                               float width, float height);

protected:
    sf::Sprite sprite_;

    HitboxComponent* hitboxComponent_;
};
