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

    // Try to change to next direction if possible
    if (nextDirection != Direction::NONE && nextDirection != currentDirection) {
        float testX = x, testY = y;

        switch (nextDirection) {
            case Direction::UP:    testY -= moveDistance; break;
            case Direction::DOWN:  testY += moveDistance; break;
            case Direction::LEFT:  testX -= moveDistance; break;
            case Direction::RIGHT: testX += moveDistance; break;
            default: break;
        }

        if (CanMove(testX, testY, level)) {
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
