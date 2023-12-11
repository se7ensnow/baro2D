#pragma once

#include "../src/headers.h"


class Map {
public:
    struct Coord {
    public:
        int32_t tileX_;
        int32_t tileY_;
        Coord() : tileX_(-1), tileY_(-1) {};
        Coord(int32_t tileX, int32_t tileY) : tileX_(tileX), tileY_(tileY) {};
    };

    class Room {
    public:
        Room();
        Room(std::vector<Coord>& roomTiles, std::vector<std::vector<uint16_t>>& map);

        const std::vector<Coord>& getEdgeTiles() const;
        const std::vector<std::shared_ptr<Room>>& getConnectedRooms() const;
        bool isConnected(const std::shared_ptr<Room>& other) const;
        bool isAccessibleFromMainRoom() const;

        void setMainRoom();
        void setAccessibleFromMainRoom();

        static void connectRooms(const std::shared_ptr<Room>& roomA, const std::shared_ptr<Room>& roomB);
        static bool cmp(const std::shared_ptr<Room>& lv, const std::shared_ptr<Room>& rv);

    protected:
        std::vector<Coord> tiles_;
        std::vector<Coord> edgeTiles_;
        std::vector<std::shared_ptr<Room>> connectedRooms_;
        size_t roomSize_;
        bool isMainRoom_;
        bool isAccessibleFromMainRoom_;
    };
public:
    Map(int32_t width, int32_t height, float squareSize, int32_t randomFillPercent, int32_t smoothAmount);
    ~Map();

    const std::string& getSeed() const;

    void render(sf::RenderTarget* target);

protected:
    bool isInMapRange(int32_t x, int32_t y) const;
    int32_t getSurroundingWallCount(int32_t gridX, int32_t gridY, int32_t step);
    void clearMapFlags();

    void randomFillMap(int32_t randomFillPercent, bool useRandomSeed);
    void generateMap(int32_t randomFillPercent, int32_t smoothAmount, bool useRandomSeed = true, std::string seed = "0");
    void smoothMap(bool closingAreas);

    std::vector<Coord> getRegionTiles(int32_t startX, int32_t startY);
    std::vector<std::vector<Coord>> getRegions(uint16_t tileType);
    void processMap();
    void connectClosestRooms(std::vector<std::shared_ptr<Room>>& allRooms, bool forceAccessibilityFromMainRoom = false);
    void createPassage(const std::shared_ptr<Room>& roomA, const std::shared_ptr<Room>& roomB, Coord& tileA, Coord& tileB);
    static std::vector<Coord> getLine(Coord from, Coord to);
    void drawCircle(Coord c, int32_t r);

protected:
    std::vector<std::vector<uint16_t>> map_;
    std::vector<std::vector<uint16_t>> mapFlags_;
    std::vector<sf::RectangleShape> squares_;

    int32_t width_;
    int32_t height_;
    float squareSize_;

    std::string seed_;
};

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}