#pragma once

#include "../src/headers.h"

class Entity {
public:
    Entity();
    virtual ~Entity();

    void createSprite(sf::Texture* texture);

    virtual void move(const float& dt, const float& dir_x, const float& dir_y);

    virtual void update(const float& dt);
    virtual void render(sf::RenderTarget* target);

protected:
    void initVariables();

protected:
    sf::Texture* texture_;
    sf::Sprite* sprite_;

    float speed_;
};
