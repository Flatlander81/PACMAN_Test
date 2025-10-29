#ifndef GAME_H
#define GAME_H

#include <GLFW/glfw3.h>
#include <memory>
#include "Level.h"
#include "Pacman.h"
#include "Ghost.h"
#include <vector>

enum class GameState {
    MENU,
    PLAYING,
    PAUSED,
    LEVEL_COMPLETE,
    GAME_OVER,
    VICTORY
};

class Game {
public:
    Game();
    ~Game();

    bool Initialize();
    void Run();
    void Cleanup();

    // Getters
    GLFWwindow* GetWindow() const { return window; }
    int GetScore() const { return score; }
    int GetLives() const { return lives; }
    int GetCurrentLevel() const { return currentLevel; }

private:
    void ProcessInput();
    void Update(float deltaTime);
    void Render();
    void HandleCollisions();
    void ResetLevel();
    void NextLevel();
    void ResetPacman();

    // Window
    GLFWwindow* window;
    const int WINDOW_WIDTH = 896;
    const int WINDOW_HEIGHT = 1024;

    // Game state
    GameState state;
    int score;
    int lives;
    int currentLevel;
    float ghostReleaseTimer;
    bool powerMode;
    float powerModeTimer;

    // Game objects
    std::unique_ptr<Level> level;
    std::unique_ptr<Pacman> pacman;
    std::vector<std::unique_ptr<Ghost>> ghosts;

    // Input state
    bool keyW, keyA, keyS, keyD;
    bool keyLastW, keyLastA, keyLastS, keyLastD;
};

#endif // GAME_H
