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
    outerCircle_.setOutlineThickness(100.f);
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
        if (math::getLengthSqr(wall->centrePos_ - pos_) <= (radius_ + mapSquareSize_) * (radius_ + mapSquareSize_)) {
            target->draw(wall->wallShape_);
        }
    }
    target->draw(outerCircle_);
    target->draw(innerCircle_);

    target->draw(finishContainer_);
    target->draw(finishText_);
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
}

std::vector<sf::Vector2f> Sonar::getMapTiles() {
    std::vector<sf::Vector2f> tiles;
    int32_t posTileX = std::floor(pos_.x / mapSquareSize_);
    int32_t posTileY = std::floor(pos_.y / mapSquareSize_);
    int32_t r = std::ceil(radius_ / mapSquareSize_);
    for (int32_t x = -r; x <= r; ++x) {
        for (int32_t y = -r; y <= r; ++y) {
            int32_t tileX = posTileX + x;
            int32_t tileY = posTileY + y;
            if (!map_.isInMapRange(tileX, tileY) || map_[tileY][tileX] == 1) {
                tiles.emplace_back(static_cast<float>(tileX) * mapSquareSize_ + mapSquareSize_ / 2.f,
                                   static_cast<float>(tileY) * mapSquareSize_ + mapSquareSize_ / 2.f);
            }
        }
    }
    return tiles;
}

void Sonar::initWalls() {
    std::vector<sf::Vector2f> tiles = getMapTiles();
    for (auto & tile : tiles) {
        //walls_.emplace_back(sf::Vector2f(mapSquareSize_ / 2.f,mapSquareSize_ / 4.f));
        walls_.push_back(std::make_unique<Wall>(sf::Vector2f(tile.x, tile.y)));
        walls_.back()->wallShape_.setSize(sf::Vector2f(mapSquareSize_, mapSquareSize_));
        walls_.back()->wallShape_.setOrigin(mapSquareSize_ / 2.f, mapSquareSize_ / 2.f);
        walls_.back()->wallShape_.setFillColor(sf::Color::Blue);
        walls_.back()->wallShape_.setPosition(centre_ + (sf::Vector2f(tile.x, tile.y) - pos_));
        //walls_.back().setRotation(math::radToDeg(std::atan2(tiles[i].y - position.y,tiles[i].x - position.x)) + 90);
    }
}

void Sonar::moveWalls(uint16_t dir) {
    int32_t posTileX = std::floor(pos_.x / mapSquareSize_);
    int32_t posTileY = std::floor(pos_.y / mapSquareSize_);
    int32_t r = std::ceil(radius_ / mapSquareSize_);
    if (dir == RIGHT) {
        for (int32_t y = -r; y <= r; ++y) {
            int32_t tileX = posTileX + r;
            int32_t tileY = posTileY + y;
            if (!map_.isInMapRange(tileX, tileY) || map_[tileY][tileX] == 1) {
                float posX = static_cast<float>(tileX) * mapSquareSize_ + mapSquareSize_ / 2.f;
                float posY = static_cast<float>(tileY) * mapSquareSize_ + mapSquareSize_ / 2.f;
                walls_.push_back(std::make_unique<Wall>(sf::Vector2f(posX, posY)));
                walls_.back()->wallShape_.setSize(sf::Vector2f(mapSquareSize_, mapSquareSize_));
                walls_.back()->wallShape_.setOrigin(mapSquareSize_ / 2.f, mapSquareSize_ / 2.f);
                walls_.back()->wallShape_.setFillColor(sf::Color::Blue);
                walls_.back()->wallShape_.setPosition(centre_ + (sf::Vector2f(posX, posY) - pos_));
            }
        }
    } else if (dir == LEFT) {
        for (int32_t y = -r; y <= r; ++y) {
            int32_t tileX = posTileX - r;
            int32_t tileY = posTileY + y;
            if (!map_.isInMapRange(tileX, tileY) || map_[tileY][tileX] == 1) {
                float posX = static_cast<float>(tileX) * mapSquareSize_ + mapSquareSize_ / 2.f;
                float posY = static_cast<float>(tileY) * mapSquareSize_ + mapSquareSize_ / 2.f;
                walls_.push_back(std::make_unique<Wall>(sf::Vector2f(posX, posY)));
                walls_.back()->wallShape_.setSize(sf::Vector2f(mapSquareSize_, mapSquareSize_));
                walls_.back()->wallShape_.setOrigin(mapSquareSize_ / 2.f, mapSquareSize_ / 2.f);
                walls_.back()->wallShape_.setFillColor(sf::Color::Blue);
                walls_.back()->wallShape_.setPosition(centre_ + (sf::Vector2f(posX, posY) - pos_));
            }
        }
    } else if (dir == DOWN) {
        for (int32_t x = -r; x <= r; ++x) {
            int32_t tileX = posTileX + x;
            int32_t tileY = posTileY + r;
            if (!map_.isInMapRange(tileX, tileY) || map_[tileY][tileX] == 1) {
                float posX = static_cast<float>(tileX) * mapSquareSize_ + mapSquareSize_ / 2.f;
                float posY = static_cast<float>(tileY) * mapSquareSize_ + mapSquareSize_ / 2.f;
                walls_.push_back(std::make_unique<Wall>(sf::Vector2f(posX, posY)));
                walls_.back()->wallShape_.setSize(sf::Vector2f(mapSquareSize_, mapSquareSize_));
                walls_.back()->wallShape_.setOrigin(mapSquareSize_ / 2.f, mapSquareSize_ / 2.f);
                walls_.back()->wallShape_.setFillColor(sf::Color::Blue);
                walls_.back()->wallShape_.setPosition(centre_ + (sf::Vector2f(posX, posY) - pos_));
            }
        }
    } else if (dir == UP) {
        for (int32_t x = -r; x <= r; ++x) {
            int32_t tileX = posTileX + x;
            int32_t tileY = posTileY - r;
            if (!map_.isInMapRange(tileX, tileY) || map_[tileY][tileX] == 1) {
                float posX = static_cast<float>(tileX) * mapSquareSize_ + mapSquareSize_ / 2.f;
                float posY = static_cast<float>(tileY) * mapSquareSize_ + mapSquareSize_ / 2.f;
                walls_.push_back(std::make_unique<Wall>(sf::Vector2f(posX, posY)));
                walls_.back()->wallShape_.setSize(sf::Vector2f(mapSquareSize_, mapSquareSize_));
                walls_.back()->wallShape_.setOrigin(mapSquareSize_ / 2.f, mapSquareSize_ / 2.f);
                walls_.back()->wallShape_.setFillColor(sf::Color::Blue);
                walls_.back()->wallShape_.setPosition(centre_ + (sf::Vector2f(posX, posY) - pos_));
            }
        }
    }
    walls_.erase(
            std::remove_if(
                    walls_.begin(),
                    walls_.end(),
                    [this](std::unique_ptr<Wall> const& p){
                        return std::abs(pos_.x - p->centrePos_.x) > radius_ + 2 * mapSquareSize_ ||
                        std::abs(pos_.y - p->centrePos_.y) > radius_ + 2 * mapSquareSize_;
                    }
            ),
            walls_.end()
    );
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
        wall->wallShape_.setPosition(centre_ + (wall->centrePos_ - pos_));
        wall->curTimer_ = std::max(wall->curTimer_ - dt, 0.f);
        if (math::getLengthSqr(wall->centrePos_ - pos_) <= curWaveRadius_ * curWaveRadius_ &&
            math::getLengthSqr(wall->centrePos_ - pos_) >= prevWaveRadius_ * prevWaveRadius_) {
            wall->curTimer_ = wall->maxTimer_;
        }
        wall->wallShape_.setFillColor(sf::Color(
                0, 40, 255, std::floor(250 * (wall->curTimer_ / wall->maxTimer_))));
    }
}

void Sonar::updateWave(const float& dt) {
    waveTimer_ += dt;
    if (waveTimer_ >= maxWaveTimer_ && !sub_.breakSonarSignal()) {
        curWaveRadius_ = 0;
        waveTimer_ = 0;
        sonarWaveSounds_[pseudoRandom_() % sonarWaveSounds_.size()].play();
    }
    prevWaveRadius_ = curWaveRadius_;
    if (curWaveRadius_ < radius_ + mapSquareSize_) {
        curWaveRadius_ += dt * waveSpeed_;
    }
    sonarWave_.setRadius(curWaveRadius_);
    sonarWave_.setPosition(centre_.x + radius_ - curWaveRadius_, centre_.y  + radius_  - curWaveRadius_);
}

void Sonar::updateFinish() {
    sf::Vector2f position = centre_ + math::getNormalized(finish_ - pos_) * (radius_ - 70.f);
    finishContainer_.setPosition(position);
    finishText_.setPosition(position);
}
