#include "sonar.h"

Sonar::Sonar(float x, float y, float radius,
             float waveSpeed, float maxWaveTimer,
             std::vector<sf::SoundBuffer*>& sounds,
             const Map& map, const Sub& sub, sf::Font& font)
: radius_(radius),
waveSpeed_(waveSpeed), maxWaveTimer_(maxWaveTimer),
soundsBuffers_(sounds),
map_(map), sub_(sub), font_(font) {
    centre_ = sf::Vector2f(x, y);
    waveTimer_ = 0;
    curWaveRadius_ = radius_;
    innerCircle_.setRadius(radius_);
    innerCircle_.setOrigin(radius_, radius_);
    innerCircle_.setPosition(centre_);
    innerCircle_.setFillColor(sf::Color::Transparent);
    innerCircle_.setOutlineThickness(1.f);
    innerCircle_.setOutlineColor(sf::Color::Green);
    innerCircle_.setPointCount(200);

    outerCircle_.setRadius(radius_);
    outerCircle_.setOrigin(radius_, radius_);
    outerCircle_.setPosition(centre_);
    outerCircle_.setFillColor(sf::Color::Transparent);
    outerCircle_.setOutlineThickness(300.f);
    outerCircle_.setOutlineColor(sf::Color::Black);
    outerCircle_.setPointCount(200);

    sonarWave_.setRadius(curWaveRadius_);
    sonarWave_.setOrigin(curWaveRadius_, curWaveRadius_);
    sonarWave_.setPosition(centre_);
    sonarWave_.setFillColor(sf::Color::Transparent);
    sonarWave_.setOutlineThickness(1.f);
    sonarWave_.setOutlineColor(sf::Color::Green);
    sonarWave_.setPointCount(200);

    prevPos_ = sub_.getPosition();
    pos_ = sub_.getPosition();

    mapSquareSize_ = map_.getSquareSize();

    pseudoRandom_ = std::mt19937(std::random_device{}());

    initSounds();

    initWalls();

    initFinish();
}

Sonar::~Sonar() = default;

void Sonar::update(const float& dt) {
    prevPos_ = pos_;
    pos_ = sub_.getPosition();
    updateWave(dt);
    updateWalls(dt);
    updateFinish();
}

void Sonar::render(sf::RenderTarget *target) {
    if (curWaveRadius_ < radius_) {
        target->draw(sonarWave_);
    }

    for (auto& wall : walls_) {
        if (math::getLengthSqr(wall.second->centrePos_ - pos_) <= (radius_ + 3 * mapSquareSize_) * (radius_ + 3 * mapSquareSize_)) {
            target->draw(wall.second->wallShape_);
        }
    }
    target->draw(outerCircle_);
    target->draw(innerCircle_);

    target->draw(finishContainer_);
    target->draw(finishText_);
    target->draw(finishDistText_);
}

void Sonar::initSounds() {
    for (auto& buffer : soundsBuffers_) {
        sonarWaveSounds_.emplace_back(*buffer);
    }
}

void Sonar::initFinish() {
    finish_ = sf::Vector2f(static_cast<float>(map_.getFinish().x) * mapSquareSize_ + mapSquareSize_ / 2.f,
                           static_cast<float>(map_.getFinish().y) * mapSquareSize_ + mapSquareSize_ / 2.f);
    finishContainer_.setSize(sf::Vector2f(100.f, 50.f));
    finishContainer_.setFillColor(sf::Color(20, 20, 20, 40));
    finishContainer_.setOrigin(50.f, 25.f);

    finishText_.setFont(font_);
    finishText_.setCharacterSize(12);
    finishText_.setString("Finish");
    finishText_.setOrigin(finishText_.getGlobalBounds().width / 2.f + finishText_.getGlobalBounds().left,
                          finishText_.getGlobalBounds().height / 2.f + finishText_.getGlobalBounds().top);

    finishDistText_.setFont(font_);
    finishDistText_.setCharacterSize(12);
    finishDistText_.setString("0 m");
}

std::vector<sf::Vector2i> Sonar::getMapTiles() {
    std::vector<sf::Vector2i> tiles;
    int32_t posTileX = std::floor(pos_.x / mapSquareSize_);
    int32_t posTileY = std::floor(pos_.y / mapSquareSize_);
    int32_t r = std::ceil(radius_ / mapSquareSize_);
    for (int32_t x = -r-2; x <= r+2; ++x) {
        for (int32_t y = -r-2; y <= r+2; ++y) {
            int32_t tileX = posTileX + x;
            int32_t tileY = posTileY + y;
            if (!map_.isInMapRange(tileX, tileY) || map_[tileY][tileX] == 1) {
                tiles.emplace_back(tileX,tileY);
            }
        }
    }
    return tiles;
}

void Sonar::initWalls() {
    std::vector<sf::Vector2i> tiles = getMapTiles();
    for (auto & tile : tiles) {
        //walls_.emplace_back(sf::Vector2f(mapSquareSize_ / 2.f,mapSquareSize_ / 4.f));
        float centerPosX = static_cast<float>(tile.x) * mapSquareSize_ + mapSquareSize_ / 2.f;
        float centerPosY = static_cast<float>(tile.y) * mapSquareSize_ + mapSquareSize_ / 2.f;
        std::pair<int32_t, int32_t> key = {tile.x, tile.y};
        walls_[key] = std::make_unique<Wall>(sf::Vector2f(centerPosX, centerPosY));
        walls_[key]->wallShape_.setSize(sf::Vector2f(mapSquareSize_, mapSquareSize_));
        walls_[key]->wallShape_.setOrigin(mapSquareSize_ / 2.f, mapSquareSize_ / 2.f);
        walls_[key]->wallShape_.setFillColor(sf::Color::Blue);
        walls_[key]->wallShape_.setPosition(centre_ + (walls_[key]->centrePos_ - pos_));
        //walls_.back().setRotation(math::radToDeg(std::atan2(tiles[i].y - position.y,tiles[i].x - position.x)) + 90);
    }
}

void Sonar::moveWalls(uint16_t dir) {
    int32_t posTileX = std::floor(pos_.x / mapSquareSize_);
    int32_t posTileY = std::floor(pos_.y / mapSquareSize_);
    int32_t r = std::ceil(radius_ / mapSquareSize_);
    if (dir == RIGHT) {
        for (int32_t y = -r-2; y <= r+2; ++y) {
            int32_t tileX = posTileX + r + 2;
            int32_t tileY = posTileY + y;
            if (!map_.isInMapRange(tileX, tileY) || map_[tileY][tileX] == 1) {
                std::pair<int32_t, int32_t> key = {tileX, tileY};
                if (!walls_.contains(key)) {
                    float posX = static_cast<float>(tileX) * mapSquareSize_ + mapSquareSize_ / 2.f;
                    float posY = static_cast<float>(tileY) * mapSquareSize_ + mapSquareSize_ / 2.f;
                    walls_[key] = std::make_unique<Wall>(sf::Vector2f(posX, posY));
                    walls_[key]->wallShape_.setSize(sf::Vector2f(mapSquareSize_, mapSquareSize_));
                    walls_[key]->wallShape_.setOrigin(mapSquareSize_ / 2.f, mapSquareSize_ / 2.f);
                    walls_[key]->wallShape_.setFillColor(sf::Color::Blue);
                    walls_[key]->wallShape_.setPosition(centre_ + (sf::Vector2f(posX, posY) - pos_));
                }
            }
        }
        erase_if(walls_, [r, posTileX](const auto& wall){
            auto const& [key, obj] = wall;
            return key.first < posTileX - r - 2;
        });
    } else if (dir == LEFT) {
        for (int32_t y = -r-2; y <= r+2; ++y) {
            int32_t tileX = posTileX - r - 2;
            int32_t tileY = posTileY + y;
            if (!map_.isInMapRange(tileX, tileY) || map_[tileY][tileX] == 1) {
                std::pair<int32_t, int32_t> key = {tileX, tileY};
                if (!walls_.contains(key)) {
                    float posX = static_cast<float>(tileX) * mapSquareSize_ + mapSquareSize_ / 2.f;
                    float posY = static_cast<float>(tileY) * mapSquareSize_ + mapSquareSize_ / 2.f;
                    walls_[key] = std::make_unique<Wall>(sf::Vector2f(posX, posY));
                    walls_[key]->wallShape_.setSize(sf::Vector2f(mapSquareSize_, mapSquareSize_));
                    walls_[key]->wallShape_.setOrigin(mapSquareSize_ / 2.f, mapSquareSize_ / 2.f);
                    walls_[key]->wallShape_.setFillColor(sf::Color::Blue);
                    walls_[key]->wallShape_.setPosition(centre_ + (sf::Vector2f(posX, posY) - pos_));
                }
            }
        }
        erase_if(walls_, [r, posTileX](const auto& wall){
            auto const& [key, obj] = wall;
            return key.first > posTileX + r + 2;
        });
    } else if (dir == DOWN) {
        for (int32_t x = -r-2; x <= r+2; ++x) {
            int32_t tileX = posTileX + x;
            int32_t tileY = posTileY + r + 2;
            if (!map_.isInMapRange(tileX, tileY) || map_[tileY][tileX] == 1) {
                std::pair<int32_t, int32_t> key = {tileX, tileY};
                if (!walls_.contains(key)) {
                    float posX = static_cast<float>(tileX) * mapSquareSize_ + mapSquareSize_ / 2.f;
                    float posY = static_cast<float>(tileY) * mapSquareSize_ + mapSquareSize_ / 2.f;
                    walls_[key] = std::make_unique<Wall>(sf::Vector2f(posX, posY));
                    walls_[key]->wallShape_.setSize(sf::Vector2f(mapSquareSize_, mapSquareSize_));
                    walls_[key]->wallShape_.setOrigin(mapSquareSize_ / 2.f, mapSquareSize_ / 2.f);
                    walls_[key]->wallShape_.setFillColor(sf::Color::Blue);
                    walls_[key]->wallShape_.setPosition(centre_ + (sf::Vector2f(posX, posY) - pos_));
                }
            }
        }
        erase_if(walls_, [r, posTileY](const auto& wall){
            auto const& [key, obj] = wall;
            return key.second < posTileY - r - 2;
        });
    } else if (dir == UP) {
        for (int32_t x = -r-2; x <= r+2; ++x) {
            int32_t tileX = posTileX + x;
            int32_t tileY = posTileY - r - 2;
            if (!map_.isInMapRange(tileX, tileY) || map_[tileY][tileX] == 1) {
                std::pair<int32_t, int32_t> key = {tileX, tileY};
                if (!walls_.contains(key)) {
                    float posX = static_cast<float>(tileX) * mapSquareSize_ + mapSquareSize_ / 2.f;
                    float posY = static_cast<float>(tileY) * mapSquareSize_ + mapSquareSize_ / 2.f;
                    walls_[key] = std::make_unique<Wall>(sf::Vector2f(posX, posY));
                    walls_[key]->wallShape_.setSize(sf::Vector2f(mapSquareSize_, mapSquareSize_));
                    walls_[key]->wallShape_.setOrigin(mapSquareSize_ / 2.f, mapSquareSize_ / 2.f);
                    walls_[key]->wallShape_.setFillColor(sf::Color::Blue);
                    walls_[key]->wallShape_.setPosition(centre_ + (sf::Vector2f(posX, posY) - pos_));
                }
            }
        }
        erase_if(walls_, [r, posTileY](const auto& wall){
            auto const& [key, obj] = wall;
            return key.second > posTileY + r + 2;
        });
    }
}

void Sonar::updateWalls(const float& dt) {
    if (prevPos_.x / mapSquareSize_ < std::floor(pos_.x / mapSquareSize_)) {
        moveWalls(RIGHT);
    }
    else if (std::floor(prevPos_.x / mapSquareSize_) > std::floor(pos_.x / mapSquareSize_)) {
        moveWalls(LEFT);
    }
    if (std::floor(prevPos_.y / mapSquareSize_) < std::floor(pos_.y / mapSquareSize_)) {
        moveWalls(DOWN);
    }
    else if (std::floor(prevPos_.y / mapSquareSize_) > std::floor(pos_.y / mapSquareSize_)) {
        moveWalls(UP);
    }
    for (auto& wall : walls_) {
        wall.second->wallShape_.setPosition(centre_ + (wall.second->centrePos_ - pos_));
        wall.second->curTimer_ = std::max(wall.second->curTimer_ - dt, 0.f);
        if (math::getLengthSqr(wall.second->centrePos_ - waveCenter_) <= curWaveRadius_ * curWaveRadius_ &&
            math::getLengthSqr(wall.second->centrePos_ - waveCenter_) >= prevWaveRadius_ * prevWaveRadius_) {
            wall.second->curTimer_ = wall.second->maxTimer_;
        }
        wall.second->wallShape_.setFillColor(sf::Color(
                0, 40, 255, std::floor(250 * (wall.second->curTimer_ / wall.second->maxTimer_))));
    }
}

void Sonar::updateWave(const float& dt) {
    waveTimer_ += dt;
    if (waveTimer_ >= maxWaveTimer_ && !sub_.breakSonarSignal()) {
        curWaveRadius_ = 0;
        waveTimer_ = 0;
        waveCenter_ = pos_;
        sonarWaveSounds_[pseudoRandom_() % sonarWaveSounds_.size()].play();
    }
    prevWaveRadius_ = curWaveRadius_;
    if (curWaveRadius_ < radius_ + 3 * mapSquareSize_) {
        curWaveRadius_ += dt * waveSpeed_;
    }
    sonarWave_.setRadius(curWaveRadius_);
    sonarWave_.setPosition(centre_.x + radius_ - curWaveRadius_, centre_.y  + radius_  - curWaveRadius_);
}

void Sonar::updateFinish() {
    sf::Vector2f position = centre_ + math::getNormalized(finish_ - pos_) * (radius_ - 70.f);
    finishContainer_.setPosition(position);
    finishText_.setPosition(position.x, position.y - 7.f);
    std::stringstream ss;
    ss << std::fixed << std::setprecision(1) << math::getLength(finish_ - pos_);
    finishDistText_.setString(ss.str() + " m");
    finishDistText_.setPosition(position.x - (finishDistText_.getLocalBounds().width / 2.f + finishDistText_.getLocalBounds().left),
                                position.y + 7.f - (finishDistText_.getLocalBounds().height / 2.f + finishDistText_.getLocalBounds().top));
}
