#ifndef LEVEL_H
#define LEVEL_H

#include <vector>
#include <string>

enum class TileType {
    EMPTY = 0,
    WALL = 1,
    PELLET = 2,
    POWER_PELLET = 3,
    GHOST_HOUSE = 4
};

struct Position {
    float x;
    float y;

    Position(float x = 0, float y = 0) : x(x), y(y) {}
};

class Level {
public:
    Level(int levelNumber);

    void LoadLevel(int levelNumber);
    void Render();
    void Reset();

    // Tile queries
    TileType GetTile(int gridX, int gridY) const;
    void SetTile(int gridX, int gridY, TileType type);
    bool IsWall(float worldX, float worldY) const;
    bool CanMoveTo(float worldX, float worldY, float radius) const;

    // Pellet management
    bool HasPellet(int gridX, int gridY) const;
    bool ConsumePellet(int gridX, int gridY);
    int GetRemainingPellets() const { return remainingPellets; }

    // Coordinate conversion
    Position GridToWorld(int gridX, int gridY) const;
    void WorldToGrid(float worldX, float worldY, int& gridX, int& gridY) const;

    // Level properties
    int GetWidth() const { return width; }
    int GetHeight() const { return height; }
    float GetTileSize() const { return tileSize; }

    Position GetPacmanStart() const { return pacmanStart; }
    Position GetGhostStart(int ghostIndex) const;

private:
    void LoadLevel1();
    void LoadLevel2();
    void CountPellets();

    int width;
    int height;
    float tileSize;
    std::vector<std::vector<TileType>> tiles;
    int remainingPellets;

    Position pacmanStart;
    std::vector<Position> ghostStarts;
};

#endif // LEVEL_H
