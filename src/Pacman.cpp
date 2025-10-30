#include "Pacman.h"
#include "Renderer.h"
#include <cmath>

Pacman::Pacman()
    : x(0), y(0), speed(100.0f), radius(12.0f),
      currentDirection(Direction::NONE), nextDirection(Direction::NONE),
      alive(true) {
}

void Pacman::SetPosition(float newX, float newY) {
    x = newX;
    y = newY;
}

void Pacman::SetDirection(Direction dir) {
    nextDirection = dir;
}

void Pacman::Reset(float startX, float startY) {
    x = startX;
    y = startY;
    currentDirection = Direction::NONE;
    nextDirection = Direction::NONE;
    alive = true;
}

void Pacman::Kill() {
    alive = false;
}

void Pacman::Revive() {
    alive = true;
}

bool Pacman::CanMove(float newX, float newY, const Level& level) const {
    return level.CanMoveTo(newX, newY, radius);
}

void Pacman::Update(float deltaTime, const Level& level) {
    if (!alive) return;

    float moveDistance = speed * deltaTime;
    float tileSize = level.GetTileSize();

    // Get current grid position
    int gridX, gridY;
    level.WorldToGrid(x, y, gridX, gridY);
    Position tileCenter = level.GridToWorld(gridX, gridY);

    // Calculate distance from tile center
    float distFromCenterX = fabsf(x - tileCenter.x);
    float distFromCenterY = fabsf(y - tileCenter.y);

    // Threshold for considering we're "at" the tile center (about 1/4 tile)
    float alignmentThreshold = tileSize * 0.25f;

    // Try to change to next direction if possible
    if (nextDirection != Direction::NONE && nextDirection != currentDirection) {
        bool canChangeDirection = false;

        // Check if we're trying to reverse direction (always allowed)
        bool isReverse = false;
        if ((currentDirection == Direction::UP && nextDirection == Direction::DOWN) ||
            (currentDirection == Direction::DOWN && nextDirection == Direction::UP) ||
            (currentDirection == Direction::LEFT && nextDirection == Direction::RIGHT) ||
            (currentDirection == Direction::RIGHT && nextDirection == Direction::LEFT)) {
            isReverse = true;
        }

        if (isReverse) {
            // Allow immediate reversal
            canChangeDirection = true;
        } else {
            // For perpendicular turns, need to be somewhat aligned with grid
            bool alignedForTurn = false;

            if ((nextDirection == Direction::UP || nextDirection == Direction::DOWN)) {
                // Turning vertical - need to be horizontally aligned
                alignedForTurn = (distFromCenterX < alignmentThreshold);
            } else if ((nextDirection == Direction::LEFT || nextDirection == Direction::RIGHT)) {
                // Turning horizontal - need to be vertically aligned
                alignedForTurn = (distFromCenterY < alignmentThreshold);
            }

            if (alignedForTurn) {
                // Check if there's a valid path in the new direction
                float testX = tileCenter.x;
                float testY = tileCenter.y;

                // Test a full tile distance in the new direction
                switch (nextDirection) {
                    case Direction::UP:    testY -= tileSize * 0.6f; break;
                    case Direction::DOWN:  testY += tileSize * 0.6f; break;
                    case Direction::LEFT:  testX -= tileSize * 0.6f; break;
                    case Direction::RIGHT: testX += tileSize * 0.6f; break;
                    default: break;
                }

                if (CanMove(testX, testY, level)) {
                    canChangeDirection = true;

                    // Snap to grid alignment when making a perpendicular turn
                    if (nextDirection == Direction::UP || nextDirection == Direction::DOWN) {
                        x = tileCenter.x;
                    } else {
                        y = tileCenter.y;
                    }
                }
            }
        }

        if (canChangeDirection) {
            currentDirection = nextDirection;
        }
    }

    // Move in current direction
    float newX = x, newY = y;

    switch (currentDirection) {
        case Direction::UP:
            newY -= moveDistance;
            break;
        case Direction::DOWN:
            newY += moveDistance;
            break;
        case Direction::LEFT:
            newX -= moveDistance;
            break;
        case Direction::RIGHT:
            newX += moveDistance;
            break;
        case Direction::NONE:
            break;
    }

    // Check if new position is valid
    if (CanMove(newX, newY, level)) {
        x = newX;
        y = newY;
    } else {
        // Hit a wall, stop moving
        currentDirection = Direction::NONE;
    }

    // Wrap around screen edges (classic Pac-Man tunnels)
    float levelWidth = level.GetWidth() * level.GetTileSize();
    if (x < -radius) x = levelWidth + radius;
    if (x > levelWidth + radius) x = -radius;
}

float Pacman::GetDirectionAngle() const {
    switch (currentDirection) {
        case Direction::RIGHT: return 0.0f;
        case Direction::DOWN:  return 90.0f;
        case Direction::LEFT:  return 180.0f;
        case Direction::UP:    return 270.0f;
        default: return 0.0f;
    }
}

void Pacman::Render(float animationTime) {
    if (!alive) return;

    // Animate mouth opening/closing
    float mouthAngle = 45.0f * (0.5f + 0.5f * sinf(animationTime * 10.0f));

    float dirAngle = GetDirectionAngle();
    Renderer::DrawPacman(x, y, radius, dirAngle, mouthAngle, 1.0f, 1.0f, 0.0f);
}
