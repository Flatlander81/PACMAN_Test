#ifndef GHOST_H
#define GHOST_H

#include "Level.h"
#include "Pacman.h"
#include <vector>

enum class GhostMode {
    CHASE,
    SCATTER,
    FRIGHTENED,
    EATEN
};

enum class GhostType {
    BLINKY,  // Red - aggressive chaser
    PINKY,   // Pink - ambusher
    INKY,    // Cyan - unpredictable
    CLYDE    // Orange - random
};

class Ghost {
public:
    Ghost(GhostType type);

    void SetPosition(float x, float y);
    void Update(float deltaTime, const Level& level, const Pacman& pacman);
    void Render();

    // Getters
    float GetX() const { return x; }
    float GetY() const { return y; }
    float GetRadius() const { return radius; }
    GhostMode GetMode() const { return mode; }
    GhostType GetType() const { return type; }

    // State management
    void SetMode(GhostMode newMode);
    void Reset(float startX, float startY);
    void Frighten();
    void Eat();
    bool IsActive() const { return active; }
    void Activate() { active = true; }

    // Collision
    bool CollidesWith(float px, float py, float pradius) const;

private:
    float x, y;
    float speed;
    float radius;
    Direction currentDirection;
    GhostMode mode;
    GhostType type;
    bool active;

    float homeX, homeY;  // For eaten mode
    float targetX, targetY;  // Current target position

    void UpdateTarget(const Level& level, const Pacman& pacman);
    void Move(float deltaTime, const Level& level);
    Direction ChooseDirection(const Level& level);
    std::vector<Direction> GetValidDirections(const Level& level) const;
    float GetDistanceToTarget(float testX, float testY) const;
    void GetColor(float& r, float& g, float& b) const;
};

#endif // GHOST_H
