#include "Ghost.h"
#include "Renderer.h"
#include <cmath>
#include <algorithm>
#include <cstdlib>

Ghost::Ghost(GhostType t)
    : x(0), y(0), speed(80.0f), radius(12.0f),
      currentDirection(Direction::UP), mode(GhostMode::SCATTER),
      type(t), active(false), homeX(0), homeY(0),
      targetX(0), targetY(0) {
}

void Ghost::SetPosition(float newX, float newY) {
    x = newX;
    y = newY;
    homeX = newX;
    homeY = newY;
}

void Ghost::Reset(float startX, float startY) {
    x = startX;
    y = startY;
    homeX = startX;
    homeY = startY;
    currentDirection = Direction::UP;
    mode = GhostMode::SCATTER;
    active = false;
}

void Ghost::SetMode(GhostMode newMode) {
    if (mode != newMode) {
        mode = newMode;
        // Reverse direction when mode changes (classic Pac-Man behavior)
        switch (currentDirection) {
            case Direction::UP: currentDirection = Direction::DOWN; break;
            case Direction::DOWN: currentDirection = Direction::UP; break;
            case Direction::LEFT: currentDirection = Direction::RIGHT; break;
            case Direction::RIGHT: currentDirection = Direction::LEFT; break;
            default: break;
        }
    }
}

void Ghost::Frighten() {
    mode = GhostMode::FRIGHTENED;
}

void Ghost::Eat() {
    mode = GhostMode::EATEN;
    targetX = homeX;
    targetY = homeY;
}

bool Ghost::CollidesWith(float px, float py, float pradius) const {
    if (!active) return false;
    float dx = x - px;
    float dy = y - py;
    float distance = sqrtf(dx * dx + dy * dy);
    return distance < (radius + pradius);
}

void Ghost::GetColor(float& r, float& g, float& b) const {
    switch (type) {
        case GhostType::BLINKY:  r = 1.0f; g = 0.0f; b = 0.0f; break;  // Red
        case GhostType::PINKY:   r = 1.0f; g = 0.7f; b = 0.8f; break;  // Pink
        case GhostType::INKY:    r = 0.0f; g = 1.0f; b = 1.0f; break;  // Cyan
        case GhostType::CLYDE:   r = 1.0f; g = 0.6f; b = 0.0f; break;  // Orange
    }
}

void Ghost::UpdateTarget(const Level& level, const Pacman& pacman) {
    float px = pacman.GetX();
    float py = pacman.GetY();

    switch (mode) {
        case GhostMode::CHASE:
            switch (type) {
                case GhostType::BLINKY:
                    // Directly target Pac-Man
                    targetX = px;
                    targetY = py;
                    break;
                case GhostType::PINKY:
                    // Target 4 tiles ahead of Pac-Man
                    {
                        float tileSize = level.GetTileSize();
                        float offset = tileSize * 4;
                        switch (pacman.GetDirection()) {
                            case Direction::UP:    targetX = px; targetY = py - offset; break;
                            case Direction::DOWN:  targetX = px; targetY = py + offset; break;
                            case Direction::LEFT:  targetX = px - offset; targetY = py; break;
                            case Direction::RIGHT: targetX = px + offset; targetY = py; break;
                            default: targetX = px; targetY = py; break;
                        }
                    }
                    break;
                case GhostType::INKY:
                    // Complex targeting based on Blinky and Pac-Man
                    targetX = px + (px - homeX) * 0.5f;
                    targetY = py + (py - homeY) * 0.5f;
                    break;
                case GhostType::CLYDE:
                    // Target Pac-Man if far, scatter if close
                    {
                        float dx = px - x;
                        float dy = py - y;
                        float distance = sqrtf(dx * dx + dy * dy);
                        if (distance > level.GetTileSize() * 8) {
                            targetX = px;
                            targetY = py;
                        } else {
                            targetX = 0;
                            targetY = level.GetHeight() * level.GetTileSize();
                        }
                    }
                    break;
            }
            break;

        case GhostMode::SCATTER:
            // Each ghost has a home corner
            switch (type) {
                case GhostType::BLINKY:
                    targetX = level.GetWidth() * level.GetTileSize();
                    targetY = 0;
                    break;
                case GhostType::PINKY:
                    targetX = 0;
                    targetY = 0;
                    break;
                case GhostType::INKY:
                    targetX = level.GetWidth() * level.GetTileSize();
                    targetY = level.GetHeight() * level.GetTileSize();
                    break;
                case GhostType::CLYDE:
                    targetX = 0;
                    targetY = level.GetHeight() * level.GetTileSize();
                    break;
            }
            break;

        case GhostMode::FRIGHTENED:
            // Random movement - target is ignored, direction chosen randomly
            break;

        case GhostMode::EATEN:
            targetX = homeX;
            targetY = homeY;
            break;
    }
}

std::vector<Direction> Ghost::GetValidDirections(const Level& level) const {
    std::vector<Direction> valid;
    float tileSize = level.GetTileSize();
    float testDistance = tileSize * 0.6f;

    // Test each direction
    if (level.CanMoveTo(x, y - testDistance, radius)) valid.push_back(Direction::UP);
    if (level.CanMoveTo(x, y + testDistance, radius)) valid.push_back(Direction::DOWN);
    if (level.CanMoveTo(x - testDistance, y, radius)) valid.push_back(Direction::LEFT);
    if (level.CanMoveTo(x + testDistance, y, radius)) valid.push_back(Direction::RIGHT);

    // Remove reverse direction (ghosts don't turn around unless mode changes)
    Direction reverse = Direction::NONE;
    switch (currentDirection) {
        case Direction::UP: reverse = Direction::DOWN; break;
        case Direction::DOWN: reverse = Direction::UP; break;
        case Direction::LEFT: reverse = Direction::RIGHT; break;
        case Direction::RIGHT: reverse = Direction::LEFT; break;
        default: break;
    }

    if (reverse != Direction::NONE && valid.size() > 1) {
        valid.erase(std::remove(valid.begin(), valid.end(), reverse), valid.end());
    }

    return valid;
}

float Ghost::GetDistanceToTarget(float testX, float testY) const {
    float dx = targetX - testX;
    float dy = targetY - testY;
    return sqrtf(dx * dx + dy * dy);
}

Direction Ghost::ChooseDirection(const Level& level) {
    std::vector<Direction> valid = GetValidDirections(level);

    if (valid.empty()) {
        return currentDirection;
    }

    if (mode == GhostMode::FRIGHTENED) {
        // Random direction when frightened
        return valid[rand() % valid.size()];
    }

    // Choose direction that gets closest to target
    Direction best = valid[0];
    float bestDistance = 999999.0f;
    float tileSize = level.GetTileSize();

    for (Direction dir : valid) {
        float testX = x, testY = y;
        switch (dir) {
            case Direction::UP:    testY -= tileSize; break;
            case Direction::DOWN:  testY += tileSize; break;
            case Direction::LEFT:  testX -= tileSize; break;
            case Direction::RIGHT: testX += tileSize; break;
            default: break;
        }

        float distance = GetDistanceToTarget(testX, testY);
        if (distance < bestDistance) {
            bestDistance = distance;
            best = dir;
        }
    }

    return best;
}

void Ghost::Move(float deltaTime, const Level& level) {
    float moveSpeed = speed;
    if (mode == GhostMode::FRIGHTENED) {
        moveSpeed *= 0.5f;  // Slower when frightened
    } else if (mode == GhostMode::EATEN) {
        moveSpeed *= 2.0f;  // Faster when returning home
    }

    float moveDistance = moveSpeed * deltaTime;

    // Check if we're at an intersection (can change direction)
    int gridX, gridY;
    level.WorldToGrid(x, y, gridX, gridY);
    Position gridCenter = level.GridToWorld(gridX, gridY);

    float distToCenter = sqrtf((x - gridCenter.x) * (x - gridCenter.x) +
                               (y - gridCenter.y) * (y - gridCenter.y));

    if (distToCenter < 3.0f) {  // Close to center of tile
        currentDirection = ChooseDirection(level);
    }

    // Move in current direction
    float newX = x, newY = y;
    switch (currentDirection) {
        case Direction::UP:    newY -= moveDistance; break;
        case Direction::DOWN:  newY += moveDistance; break;
        case Direction::LEFT:  newX -= moveDistance; break;
        case Direction::RIGHT: newX += moveDistance; break;
        default: break;
    }

    if (level.CanMoveTo(newX, newY, radius)) {
        x = newX;
        y = newY;
    }

    // Wrap around screen edges
    float levelWidth = level.GetWidth() * level.GetTileSize();
    if (x < -radius) x = levelWidth + radius;
    if (x > levelWidth + radius) x = -radius;

    // Check if eaten ghost reached home
    if (mode == GhostMode::EATEN) {
        float dx = x - homeX;
        float dy = y - homeY;
        if (sqrtf(dx * dx + dy * dy) < level.GetTileSize()) {
            mode = GhostMode::SCATTER;
        }
    }
}

void Ghost::Update(float deltaTime, const Level& level, const Pacman& pacman) {
    if (!active) return;

    UpdateTarget(level, pacman);
    Move(deltaTime, level);
}

void Ghost::Render() {
    if (!active) return;

    float r, g, b;
    GetColor(r, g, b);

    bool scared = (mode == GhostMode::FRIGHTENED);

    if (mode == GhostMode::EATEN) {
        // Draw as eyes only
        Renderer::DrawCircle(x - 5, y, 3, 1.0f, 1.0f, 1.0f);
        Renderer::DrawCircle(x + 5, y, 3, 1.0f, 1.0f, 1.0f);
        Renderer::DrawCircle(x - 5, y, 1.5f, 0.0f, 0.0f, 1.0f);
        Renderer::DrawCircle(x + 5, y, 1.5f, 0.0f, 0.0f, 1.0f);
    } else {
        Renderer::DrawGhost(x, y, radius, r, g, b, scared);
    }
}
