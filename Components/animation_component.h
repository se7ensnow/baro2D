#pragma once

#include "../src/headers.h"

class AnimationComponent {
public:
    AnimationComponent();
    ~AnimationComponent();

protected:
    class Animation {
    public:
        Animation(sf::Texture& textureSheet);
        ~Animation();

        void play();
        void pause();
        void reset();

    protected:
        sf::Texture& textureSheet_;
    };

protected:

protected:
    std::map<std::string, Animation> animations_;

};
