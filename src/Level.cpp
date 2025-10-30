#include "Level.h"
#include "Renderer.h"
#include <algorithm>

Level::Level(int levelNumber) : width(28), height(31), tileSize(28.0f), remainingPellets(0) {
    LoadLevel(levelNumber);
}

void Level::LoadLevel(int levelNumber) {
    if (levelNumber == 1) {
        LoadLevel1();
    } else if (levelNumber == 2) {
        LoadLevel2();
    } else {
        LoadLevel1();  // Default to level 1
    }
    CountPellets();
}

void Level::LoadLevel1() {
    // Classic Pac-Man maze layout (28x31) - Perfectly symmetric
    // 1=wall, 2=pellet, 3=power pellet, 0=empty, 4=ghost house
    const char* maze[31] = {
        "1111111111111111111111111111",
        "1222222222222112222222222221",
        "1211112111112112111112111121",
        "1311112111112112111112111131",
        "1211112111112112111112111121",
        "1222222222222222222222222221",
        "1211112112111111112112111121",
        "1211112112111111112112111121",
        "1222222112222112222112222221",
        "1111112111110110111112111111",
        "1111112111110110111112111111",
        "1111112110000000011112111111",
        "1111112110111111110112111111",
        "1111112110114444110112111111",
        "0000000000114444110000000000",
        "1111112110114444110112111111",
        "1111112110111111110112111111",
        "1111112110000000011112111111",
        "1111112110111111110112111111",
        "1111112110111111110112111111",
        "1222222222222112222222222221",
        "1211112111112112111112111121",
        "1211112111112112111112111121",
        "1322112222222222222222112231",
        "1112112112111111112112112111",
        "1112112112111111112112112111",
        "1222222112222112222112222221",
        "1211111111112112111111111121",
        "1211111111112112111111111121",
        "1222222222222222222222222221",
        "1111111111111111111111111111"
    };

    tiles.clear();
    tiles.resize(height);
    for (int y = 0; y < height; y++) {
        tiles[y].resize(width);
        for (int x = 0; x < width; x++) {
            switch (maze[y][x]) {
                case '0': tiles[y][x] = TileType::EMPTY; break;
                case '1': tiles[y][x] = TileType::WALL; break;
                case '2': tiles[y][x] = TileType::PELLET; break;
                case '3': tiles[y][x] = TileType::POWER_PELLET; break;
                case '4': tiles[y][x] = TileType::GHOST_HOUSE; break;
                default: tiles[y][x] = TileType::EMPTY; break;
            }
        }
    }

    // Set starting positions
    pacmanStart = GridToWorld(14, 23);

    ghostStarts.clear();
    ghostStarts.push_back(GridToWorld(14, 14));  // Blinky (center)
    ghostStarts.push_back(GridToWorld(12, 14));  // Pinky (left)
    ghostStarts.push_back(GridToWorld(14, 15));  // Inky (center-bottom)
    ghostStarts.push_back(GridToWorld(16, 14));  // Clyde (right)
}

void Level::LoadLevel2() {
    // Level 2 maze - Perfectly symmetric with different pattern
    const char* maze[31] = {
        "1111111111111111111111111111",
        "1222222222222222222222222221",
        "1211112111111111111112111121",
        "1311112111111111111112111131",
        "1211112111111111111112111121",
        "1211112111111111111112111121",
        "1222222111112112111112222221",
        "1111112111112112111112111111",
        "1111112111112112111112111111",
        "1111112222222222222222111111",
        "1111112111110110111112111111",
        "1111112111110110111112111111",
        "1111112110000000011112111111",
        "1111112110114444110112111111",
        "0000000000114444110000000000",
        "1111112110114444110112111111",
        "1111112110000000011112111111",
        "1111112222222222222222111111",
        "1111112111111111111112111111",
        "1111112111111111111112111111",
        "1222222111112112111112222221",
        "1211111111112112111111111121",
        "1211111111112112111111111121",
        "1322222222222112222222222231",
        "1112111111110110111111112111",
        "1112111111110110111111112111",
        "1222111111112112111111112221",
        "1211111112222222222111111121",
        "1211111112222222222111111121",
        "1222222222222222222222222221",
        "1111111111111111111111111111"
    };

    tiles.clear();
    tiles.resize(height);
    for (int y = 0; y < height; y++) {
        tiles[y].resize(width);
        for (int x = 0; x < width; x++) {
            switch (maze[y][x]) {
                case '0': tiles[y][x] = TileType::EMPTY; break;
                case '1': tiles[y][x] = TileType::WALL; break;
                case '2': tiles[y][x] = TileType::PELLET; break;
                case '3': tiles[y][x] = TileType::POWER_PELLET; break;
                case '4': tiles[y][x] = TileType::GHOST_HOUSE; break;
                default: tiles[y][x] = TileType::EMPTY; break;
            }
        }
    }

    // Set starting positions
    pacmanStart = GridToWorld(14, 23);

    ghostStarts.clear();
    ghostStarts.push_back(GridToWorld(14, 14));
    ghostStarts.push_back(GridToWorld(12, 14));
    ghostStarts.push_back(GridToWorld(14, 15));
    ghostStarts.push_back(GridToWorld(16, 14));
}

void Level::CountPellets() {
    remainingPellets = 0;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (tiles[y][x] == TileType::PELLET || tiles[y][x] == TileType::POWER_PELLET) {
                remainingPellets++;
            }
        }
    }
}

void Level::Reset() {
    LoadLevel(1);  // Reset to current level layout
}

TileType Level::GetTile(int gridX, int gridY) const {
    if (gridX < 0 || gridX >= width || gridY < 0 || gridY >= height) {
        return TileType::WALL;
    }
    return tiles[gridY][gridX];
}

void Level::SetTile(int gridX, int gridY, TileType type) {
    if (gridX >= 0 && gridX < width && gridY >= 0 && gridY < height) {
        tiles[gridY][gridX] = type;
    }
}

bool Level::IsWall(float worldX, float worldY) const {
    int gridX, gridY;
    WorldToGrid(worldX, worldY, gridX, gridY);
    TileType tile = GetTile(gridX, gridY);
    return tile == TileType::WALL;
}

bool Level::CanMoveTo(float worldX, float worldY, float radius) const {
    // Check if the circle at (worldX, worldY) with given radius collides with walls
    // Check center and 4 edge points
    if (IsWall(worldX, worldY)) return false;
    if (IsWall(worldX - radius, worldY)) return false;
    if (IsWall(worldX + radius, worldY)) return false;
    if (IsWall(worldX, worldY - radius)) return false;
    if (IsWall(worldX, worldY + radius)) return false;

    return true;
}

bool Level::HasPellet(int gridX, int gridY) const {
    TileType tile = GetTile(gridX, gridY);
    return tile == TileType::PELLET || tile == TileType::POWER_PELLET;
}

bool Level::ConsumePellet(int gridX, int gridY) {
    TileType tile = GetTile(gridX, gridY);
    if (tile == TileType::PELLET || tile == TileType::POWER_PELLET) {
        bool wasPowerPellet = (tile == TileType::POWER_PELLET);
        SetTile(gridX, gridY, TileType::EMPTY);
        remainingPellets--;
        return wasPowerPellet;
    }
    return false;
}

Position Level::GridToWorld(int gridX, int gridY) const {
    return Position(gridX * tileSize + tileSize / 2, gridY * tileSize + tileSize / 2);
}

void Level::WorldToGrid(float worldX, float worldY, int& gridX, int& gridY) const {
    gridX = static_cast<int>(worldX / tileSize);
    gridY = static_cast<int>(worldY / tileSize);
}

Position Level::GetGhostStart(int ghostIndex) const {
    if (ghostIndex >= 0 && ghostIndex < static_cast<int>(ghostStarts.size())) {
        return ghostStarts[ghostIndex];
    }
    return GridToWorld(14, 14);  // Default center position
}

void Level::Render() {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            float worldX = x * tileSize;
            float worldY = y * tileSize;

            switch (tiles[y][x]) {
                case TileType::WALL:
                    Renderer::DrawRect(worldX, worldY, tileSize, tileSize, 0.0f, 0.0f, 1.0f);
                    break;
                case TileType::PELLET:
                    Renderer::DrawCircle(worldX + tileSize / 2, worldY + tileSize / 2,
                                        2.0f, 1.0f, 0.8f, 0.6f);
                    break;
                case TileType::POWER_PELLET:
                    Renderer::DrawCircle(worldX + tileSize / 2, worldY + tileSize / 2,
                                        6.0f, 1.0f, 0.8f, 0.6f);
                    break;
                case TileType::GHOST_HOUSE:
                    Renderer::DrawRect(worldX, worldY, tileSize, tileSize, 0.5f, 0.0f, 0.5f);
                    break;
                default:
                    break;
            }
        }
    }
}
