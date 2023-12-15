#include "map.h"

#include <utility>


Map::Map(int32_t width, int32_t height, float squareSize,
         int32_t startX, int32_t startY,
         int32_t finishX, int32_t finishY,
         int32_t randomFillPercent)
: width_(width), height_(height), squareSize_(squareSize) {
    start_ = sf::Vector2i(startX, startY);
    finish_ = sf::Vector2i(finishX, finishY);

    startMark_.setSize(sf::Vector2f(squareSize_ / 2.f, squareSize_ / 2.f));
    startMark_.setPosition(static_cast<float>(startX) * squareSize_ + squareSize_ / 4.f,
                           static_cast<float>(startY) * squareSize_ + squareSize_ / 4.f);
    startMark_.setFillColor(sf::Color::Green);

    finishMark_.setSize(sf::Vector2f(squareSize_ / 2.f, squareSize_ / 2.f));
    finishMark_.setPosition(static_cast<float>(finishX) * squareSize_ + squareSize_ / 4.f,
                            static_cast<float>(finishY) * squareSize_ + squareSize_ / 4.f);
    finishMark_.setFillColor(sf::Color::Red);

    squares_.resize(width_ * height_, sf::RectangleShape());
    map_.resize(height_);
    mapFlags_.resize(height_);
    for (int32_t y = 0; y < height_; ++y) {
        map_[y].resize(width_, 0);
        mapFlags_[y].resize(width_, 0);
        for (int32_t x = 0; x < width_; ++x) {
            squares_[y * width_ + x].setPosition(static_cast<float>(x) * squareSize_, static_cast<float>(y) * squareSize_);
            squares_[y * width_ + x].setSize(sf::Vector2f(squareSize_, squareSize_));
        }
    }
    generateMap(randomFillPercent, startX, startY, finishX, finishY);
}

Map::~Map() {

}

const std::string &Map::getSeed() const {
    return seed_;
}

sf::Vector2i Map::getSize() const {
    return {width_, height_};
}

float Map::getSquareSize() const {
    return squareSize_;
}

sf::Vector2i Map::getFinish() const {
    return finish_;
}

bool Map::isInMapRange(int32_t x, int32_t y) const {
    return x >= 0 && x < width_ && y >= 0 && y < height_;
}

int32_t Map::getSurroundingWallCount(int32_t gridX, int32_t gridY, int32_t step) {
    int wallCount = 0;
    for (int32_t neighborY = gridY - step; neighborY <= gridY + step; ++neighborY) {
        for (int32_t neighborX = gridX - step; neighborX <= gridX + step; ++neighborX) {
            if (isInMapRange(neighborX, neighborY)) {
                if (neighborX != gridX || neighborY != gridY) {
                    wallCount += map_[neighborY][neighborX];
                }
            } else {
                wallCount++;
            }
        }
    }
    return wallCount;
}

void Map::clearMapFlags() {
    for (int32_t y = 0; y < height_; ++y) {
        mapFlags_[y].clear();
        mapFlags_[y].resize(width_, 0);
    }
}

void Map::randomFillMap(int32_t randomFillPercent, bool useRandomSeed) {
    if (useRandomSeed) {
        seed_ = std::to_string(std::time(nullptr));
    }
    std::mt19937 pseudoRandom(std::hash<std::string>{}(seed_));

    for (int32_t y = 0; y < height_; ++y) {
        for (int32_t x = 0; x < width_; ++x) {
            if (x == 0 || x == width_ - 1 || y == 0 || y == height_ - 1) {
                map_[y][x] = 1;
            } else {
                map_[y][x] = (pseudoRandom() % 100) < randomFillPercent ? 1 : 0;
            }
        }
    }
}

void Map::generateMap(int32_t randomFillPercent,
                      int32_t startX, int32_t startY,
                      int32_t finishX, int32_t finishY,
                      bool useRandomSeed, std::string seed) {
    seed_ = std::move(seed);
    randomFillMap(randomFillPercent, useRandomSeed);

    drawCircle(Coord(startX, startY), 7);
    drawCircle(Coord(finishX, finishY), 7);

    for (int32_t i = 0; i < 4; ++i) {
        smoothMap(true);
    }
    for (int32_t i = 0; i < 3; ++i) {
        smoothMap(false);
    }

    processMap();
}

void Map::smoothMap(bool closingAreas) {
    std::vector<std::vector<uint16_t>> map_temp(height_);
    for (int32_t y = 0; y < height_; ++y) {
        map_temp[y].resize(width_, 0);
        for (int32_t x = 0; x < width_; ++x) {
            int32_t neighborWallTiles = getSurroundingWallCount(x, y, 1);
            int32_t secNeighborWallTiles = getSurroundingWallCount(x, y, 2);
            if (neighborWallTiles > 4 || (secNeighborWallTiles < 3 && closingAreas &&
            (x - start_.x)*(x - start_.x) + (y - start_.y)*(y - start_.y) > 49 &&
            (x - finish_.x)*(x - finish_.x) + (y - finish_.y)*(y - finish_.y) > 49)) {
                map_temp[y][x] = 1;
            } else if (neighborWallTiles < 4) {
                map_temp[y][x] = 0;
            } else {
                map_temp[y][x] = map_[y][x];
            }
        }
    }
    map_ = map_temp;
}

std::vector<Map::Coord> Map::getRegionTiles(int32_t startX, int32_t startY) {
    std::vector<Coord> tiles;
    uint16_t tileType = map_[startY][startX];
    std::queue<Coord> queue;
    queue.push(Coord(startX, startY));
    mapFlags_[startY][startX] = 1;
    while (!queue.empty()) {
        Coord tile = queue.front();
        queue.pop();
        tiles.push_back(tile);

        for (int32_t x = tile.tileX_ - 1; x <= tile.tileX_ + 1; ++x) {
            for (int32_t y = tile.tileY_ - 1; y <= tile.tileY_ + 1; ++y) {
                if (isInMapRange(x, y) && (y == tile.tileY_ || x == tile.tileX_)) {
                    if (mapFlags_[y][x] == 0 && map_[y][x] == tileType) {
                        mapFlags_[y][x] = 1;
                        queue.push(Coord(x, y));
                    }
                }
            }
        }
    }
    return tiles;
}

std::vector<std::vector<Map::Coord>> Map::getRegions(uint16_t tileType) {
    clearMapFlags();
    std::vector<std::vector<Coord>> regions;
    for (int32_t y = 0; y < height_; ++y) {
        for (int32_t x = 0; x < width_; ++x) {
            if (mapFlags_[y][x] == 0 && map_[y][x] == tileType) {
                std::vector<Coord> new_region = getRegionTiles(x, y);
                regions.push_back(new_region);
            }
        }
    }
    return regions;
}

void Map::processMap() {
    std::vector<std::vector<Map::Coord>> wallRegions = getRegions(1);
    int wallThreshholdSize = 50;

    for (auto& wallRegion : wallRegions) {
        if (wallRegion.size() < wallThreshholdSize) {
            for (auto& tile : wallRegion) {
                map_[tile.tileY_][tile.tileX_] = 0;
            }
        }
    }

    std::vector<std::vector<Map::Coord>> roomRegions = getRegions(0);
    int roomThresholdSize = 50;
    std::vector<std::shared_ptr<Room>> survivingRooms;

    for (auto& roomRegion : roomRegions) {
        if (roomRegion.size() < roomThresholdSize) {
            for (auto& tile : roomRegion) {
                map_[tile.tileY_][tile.tileX_] = 1;
            }
        } else {
            survivingRooms.push_back(std::make_shared<Room>(roomRegion, map_));
        }
    }

    std::sort(survivingRooms.begin(), survivingRooms.end(), Room::cmp);
    survivingRooms[0]->setMainRoom();
    survivingRooms[0]->setAccessibleFromMainRoom();
    connectClosestRooms(survivingRooms);
}

void Map::render(sf::RenderTarget* target) {
    for (int32_t y = 0; y < height_; ++y) {
        for (int32_t x = 0; x < width_; ++x) {
            if (map_[y][x]) {
                squares_[y * width_ + x].setFillColor(sf::Color::White);
            } else {
                squares_[y * width_ + x].setFillColor(sf::Color::Black);
            }
            target->draw(squares_[y * width_ + x]);
        }
    }

    target->draw(startMark_);
    target->draw(finishMark_);
}

const std::vector<uint16_t>& Map::operator[](size_t n) const {
    return map_[n];
}

void Map::connectClosestRooms(std::vector<std::shared_ptr<Room>>& allRooms, bool forceAccessibilityFromMainRoom) {
    std::vector<std::shared_ptr<Room>> roomListA;
    std::vector<std::shared_ptr<Room>> roomListB;

    if (forceAccessibilityFromMainRoom) {
        for (auto& room : allRooms) {
            if (room->isAccessibleFromMainRoom()) {
                roomListB.push_back(room);
            } else {
                roomListA.push_back(room);
            }
        }
    } else {
        roomListA = allRooms;
        roomListB = allRooms;
    }

    double bestDistance = 0;
    Coord bestTileA;
    Coord bestTileB;
    std::shared_ptr<Room> bestRoomA;
    std::shared_ptr<Room> bestRoomB;
    bool possibleConnectionFound = false;
    for (auto& roomA : roomListA) {
        if (!forceAccessibilityFromMainRoom) {
            possibleConnectionFound = false;
            if (!roomA->getConnectedRooms().empty()) {
                continue;
            }
        }
        for (auto& roomB : roomListB) {
            if (roomA == roomB || roomA->isConnected(roomB)) {
                continue;
            }
            for (int32_t tileIndexA = 0; tileIndexA < roomA->getEdgeTiles().size(); ++tileIndexA) {
                for (int32_t tileIndexB = 0; tileIndexB < roomB->getEdgeTiles().size(); ++tileIndexB) {
                    Coord tileA = roomA->getEdgeTiles()[tileIndexA];
                    Coord tileB = roomB->getEdgeTiles()[tileIndexB];
                    double distanceBetweenRooms = std::pow(tileA.tileX_ - tileB.tileX_, 2) + std::pow(tileA.tileY_ - tileB.tileY_, 2);
                    if (distanceBetweenRooms < bestDistance || !possibleConnectionFound) {
                        bestDistance = distanceBetweenRooms;
                        possibleConnectionFound = true;
                        bestTileA = tileA;
                        bestTileB = tileB;
                        bestRoomA = roomA;
                        bestRoomB = roomB;
                    }
                }
            }
        }

        if (possibleConnectionFound && !forceAccessibilityFromMainRoom) {
            createPassage(bestRoomA, bestRoomB, bestTileA, bestTileB);
        }
    }
    if (possibleConnectionFound && forceAccessibilityFromMainRoom) {
        createPassage(bestRoomA, bestRoomB, bestTileA, bestTileB);
        connectClosestRooms(allRooms, true);
    }
    if (!forceAccessibilityFromMainRoom) {
        connectClosestRooms(allRooms, true);
    }
}

void Map::createPassage(const std::shared_ptr<Room>& roomA, const std::shared_ptr<Room>& roomB,
                        Map::Coord &tileA, Map::Coord &tileB) {
    Room::connectRooms(roomA, roomB);

    std::vector<Coord> line = getLine(tileA, tileB);
    for (auto& coord : line) {
        drawCircle(coord, 4);
    }
}

std::vector<Map::Coord> Map::getLine(Map::Coord from, Map::Coord to) {
    std::vector<Map::Coord> line;

    int32_t x = from.tileX_;
    int32_t y = from.tileY_;

    int32_t dx = to.tileX_ - from.tileX_;
    int32_t dy = to.tileY_ - from.tileY_;

    bool inverted = false;
    int32_t step = (dx > 0) - (dx < 0);
    int32_t gradientStep = (dy > 0) - (dy < 0);

    int32_t longest = std::abs(dx);
    int32_t shortest = std::abs(dy);

    if (longest < shortest) {
        inverted = true;
        std::swap(longest, shortest);
        std::swap(step, gradientStep);
    }

    int32_t gradientAccumulation = longest / 2;
    for (int32_t i = 0; i < longest; ++i) {
        line.emplace_back(x, y);

        if (inverted) {
            y += step;
        } else {
            x += step;
        }

        gradientAccumulation += shortest;
        if (gradientAccumulation >= longest) {
            if (inverted) {
                x += gradientStep;
            } else {
                y += gradientStep;
            }
            gradientAccumulation -= longest;
        }
    }
    return line;
}

void Map::drawCircle(Coord c, int32_t r) {
    for (int32_t x = -r; x <= r; ++x) {
        for (int32_t y = -r; y <= r; ++y) {
            if (x*x + y*y <= r*r) {
                int32_t drawX = c.tileX_ + x;
                int32_t drawY = c.tileY_ + y;
                if (isInMapRange(drawX, drawY)) {
                    map_[drawY][drawX] = 0;
                }
            }
        }
    }
}

Map::Room::Room() {

}

Map::Room::Room(std::vector<Coord>& roomTiles, std::vector<std::vector<uint16_t>>& map) : tiles_(roomTiles) {
    roomSize_ = tiles_.size();
    isMainRoom_ = false;
    isAccessibleFromMainRoom_ = false;

    for (auto& tile : tiles_) {
        for (int32_t x = tile.tileX_ - 1; x <= tile.tileX_ + 1; ++x) {
            for (int32_t y = tile.tileY_ - 1; y <= tile.tileY_ + 1; ++y) {
                if (y == tile.tileY_ || x == tile.tileX_) {
                    if (map[y][x] == 1) {
                        edgeTiles_.push_back(tile);
                    }
                }
            }
        }
    }
}

const std::vector<Map::Coord> &Map::Room::getEdgeTiles() const {
    return edgeTiles_;
}

const std::vector<std::shared_ptr<Map::Room>> &Map::Room::getConnectedRooms() const {
    return connectedRooms_;
}

bool Map::Room::isConnected(const std::shared_ptr<Room> &other) const {
    return std::find(connectedRooms_.begin(), connectedRooms_.end(), other) != connectedRooms_.end();
}

bool Map::Room::isAccessibleFromMainRoom() const {
    return isAccessibleFromMainRoom_;
}

void Map::Room::setMainRoom() {
    isMainRoom_ = true;
}

void Map::Room::setAccessibleFromMainRoom() {
    if (!isAccessibleFromMainRoom_) {
        isAccessibleFromMainRoom_ = true;
        for (auto &room: connectedRooms_) {
            room->setAccessibleFromMainRoom();
        }
    }
}

void Map::Room::connectRooms(const std::shared_ptr<Room>& roomA, const std::shared_ptr<Room>& roomB) {
    if (roomA->isAccessibleFromMainRoom_) {
        roomB->setAccessibleFromMainRoom();
    } else if (roomB->isAccessibleFromMainRoom_) {
        roomA->setAccessibleFromMainRoom();
    }
    roomA->connectedRooms_.push_back(roomB);
    roomB->connectedRooms_.push_back(roomA);

}

bool Map::Room::cmp(const std::shared_ptr<Room>& lv, const std::shared_ptr<Room>& rv) {
    return lv->roomSize_ > rv->roomSize_;
}

