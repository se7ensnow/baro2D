#pragma once

#include "../Components/hitbox_component.h"

class Entity {
public:
    Entity();
    virtual ~Entity();

    virtual void move(const float& dir_x, const float& dir_y, const float& dt) = 0;

    virtual void update(const float& dt);
    virtual void render(sf::RenderTarget* target);

protected:
    virtual void initVariables();
    void setTexture(sf::Texture& texture);
    virtual void setPosition(float x, float y) = 0;

protected:
    sf::Sprite sprite_;

    sf::Vector2f position_;
};
