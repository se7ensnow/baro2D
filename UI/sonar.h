#pragma once

#include "../Entities/sub.h"
#include "../Utils/sf_shapes.h"
#include "../Utils/sf_math.h"


class Sonar {
public:
    enum directions{RIGHT=0, DOWN, LEFT, UP};

    struct Wall {
        sf::RectangleShape wallShape_;
        sf::Vector2f centrePos_;
        float maxTimer_ = 1.5f;
        float curTimer_ = 0.f;
        Wall(sf::Vector2f centre_pos) : centrePos_(centre_pos) {wallShape_ = sf::RectangleShape();};
    };

public:
    Sonar(float x, float y, float radius,
          float waveSpeed, float maxWaveTimer,
          std::vector<sf::SoundBuffer*>& sounds,
          const Map& map, const Sub& sub, sf::Font& font);
    ~Sonar();

    void update(const float& dt);
    void render(sf::RenderTarget* target);

protected:
    void initSounds();
    void initWalls();
    void initFinish();
    void moveWalls(uint16_t dir);
    void updateWalls(const float& dt);
    void updateWave(const float& dt);
    void updateFinish();
    std::vector<sf::Vector2i> getMapTiles();

protected:
    const Map& map_;
    const Sub& sub_;

    sf::Vector2f prevPos_;
    sf::Vector2f pos_;
    float mapSquareSize_;

    sf::Vector2f centre_;
    float radius_;
    sf::CircleShape innerCircle_;
    sf::CircleShape outerCircle_;

    // std::vector<EllipseShape> walls_;
    std::map<std::pair<int32_t, int32_t>, std::unique_ptr<Wall>> walls_;

    sf::CircleShape sonarWave_;
    float prevWaveRadius_;
    float curWaveRadius_;
    sf::Vector2f waveCenter_;
    float waveSpeed_;
    float waveTimer_;
    float maxWaveTimer_;

    std::mt19937 pseudoRandom_;
    std::vector<sf::SoundBuffer*>& soundsBuffers_;
    std::vector<sf::Sound> sonarWaveSounds_;

    sf::Font& font_;
    sf::RectangleShape finishContainer_;
    sf::Text finishText_;
    sf::Text finishDistText_;
    sf::Vector2f finish_;
};
