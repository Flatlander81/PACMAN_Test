#ifndef PACMAN_H
#define PACMAN_H

#include "Level.h"

enum class Direction {
    NONE,
    UP,
    DOWN,
    LEFT,
    RIGHT
};

class Pacman {
public:
    Pacman();

    void SetPosition(float x, float y);
    void SetDirection(Direction dir);
    void Update(float deltaTime, const Level& level);
    void Render(float animationTime);

    // Getters
    float GetX() const { return x; }
    float GetY() const { return y; }
    float GetRadius() const { return radius; }
    Direction GetDirection() const { return currentDirection; }

    // State
    void Reset(float startX, float startY);
    bool IsAlive() const { return alive; }
    void Kill();
    void Revive();

private:
    float x, y;
    float speed;
    float radius;
    Direction currentDirection;
    Direction nextDirection;
    bool alive;

    bool CanMove(float newX, float newY, const Level& level) const;
    float GetDirectionAngle() const;
};

#endif // PACMAN_H
