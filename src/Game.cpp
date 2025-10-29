#include "Game.h"
#include "Renderer.h"
#include <iostream>
#include <ctime>
#include <cstdlib>

Game::Game()
    : window(nullptr), state(GameState::PLAYING),
      score(0), lives(3), currentLevel(1),
      ghostReleaseTimer(0.0f), powerMode(false), powerModeTimer(0.0f),
      keyW(false), keyA(false), keyS(false), keyD(false),
      keyLastW(false), keyLastA(false), keyLastS(false), keyLastD(false) {
    srand(static_cast<unsigned int>(time(nullptr)));
}

Game::~Game() {
    Cleanup();
}

bool Game::Initialize() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return false;
    }

    // Create window
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Pac-Man", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);  // Enable vsync

    // Initialize OpenGL
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Initialize renderer
    Renderer::Initialize(WINDOW_WIDTH, WINDOW_HEIGHT);

    // Create game objects
    level = std::make_unique<Level>(currentLevel);
    pacman = std::make_unique<Pacman>();

    Position pacmanStart = level->GetPacmanStart();
    pacman->SetPosition(pacmanStart.x, pacmanStart.y);

    // Create ghosts
    ghosts.clear();
    ghosts.push_back(std::make_unique<Ghost>(GhostType::BLINKY));
    ghosts.push_back(std::make_unique<Ghost>(GhostType::PINKY));
    ghosts.push_back(std::make_unique<Ghost>(GhostType::INKY));
    ghosts.push_back(std::make_unique<Ghost>(GhostType::CLYDE));

    for (int i = 0; i < 4; i++) {
        Position ghostStart = level->GetGhostStart(i);
        ghosts[i]->SetPosition(ghostStart.x, ghostStart.y);
    }

    // Activate first ghost immediately, others with delay
    ghosts[0]->Activate();

    return true;
}

void Game::ProcessInput() {
    // Store previous key states
    keyLastW = keyW;
    keyLastA = keyA;
    keyLastS = keyS;
    keyLastD = keyD;

    // Read current key states
    keyW = glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS;
    keyA = glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS;
    keyS = glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS;
    keyD = glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS;

    // Set Pac-Man direction based on WASD keys
    if (keyW && !keyLastW) pacman->SetDirection(Direction::UP);
    if (keyS && !keyLastS) pacman->SetDirection(Direction::DOWN);
    if (keyA && !keyLastA) pacman->SetDirection(Direction::LEFT);
    if (keyD && !keyLastD) pacman->SetDirection(Direction::RIGHT);

    // ESC to close
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    // R to restart
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        score = 0;
        lives = 3;
        currentLevel = 1;
        ResetLevel();
    }
}

void Game::Update(float deltaTime) {
    if (state != GameState::PLAYING) return;

    // Update ghost release timer
    ghostReleaseTimer += deltaTime;
    if (ghostReleaseTimer > 2.0f && !ghosts[1]->IsActive()) ghosts[1]->Activate();
    if (ghostReleaseTimer > 4.0f && !ghosts[2]->IsActive()) ghosts[2]->Activate();
    if (ghostReleaseTimer > 6.0f && !ghosts[3]->IsActive()) ghosts[3]->Activate();

    // Update power mode timer
    if (powerMode) {
        powerModeTimer -= deltaTime;
        if (powerModeTimer <= 0) {
            powerMode = false;
            for (auto& ghost : ghosts) {
                if (ghost->GetMode() == GhostMode::FRIGHTENED) {
                    ghost->SetMode(GhostMode::SCATTER);
                }
            }
        }
    }

    // Update Pac-Man
    pacman->Update(deltaTime, *level);

    // Check pellet collection
    int gridX, gridY;
    level->WorldToGrid(pacman->GetX(), pacman->GetY(), gridX, gridY);
    if (level->HasPellet(gridX, gridY)) {
        bool isPowerPellet = level->ConsumePellet(gridX, gridY);
        score += isPowerPellet ? 50 : 10;

        if (isPowerPellet) {
            powerMode = true;
            powerModeTimer = 8.0f;
            for (auto& ghost : ghosts) {
                if (ghost->IsActive() && ghost->GetMode() != GhostMode::EATEN) {
                    ghost->Frighten();
                }
            }
        }
    }

    // Check level complete
    if (level->GetRemainingPellets() == 0) {
        NextLevel();
        return;
    }

    // Update ghosts
    for (auto& ghost : ghosts) {
        ghost->Update(deltaTime, *level, *pacman);
    }

    // Handle collisions
    HandleCollisions();
}

void Game::HandleCollisions() {
    for (auto& ghost : ghosts) {
        if (ghost->CollidesWith(pacman->GetX(), pacman->GetY(), pacman->GetRadius())) {
            if (ghost->GetMode() == GhostMode::FRIGHTENED) {
                // Eat ghost
                ghost->Eat();
                score += 200;
            } else if (ghost->GetMode() != GhostMode::EATEN) {
                // Pac-Man dies
                lives--;
                if (lives <= 0) {
                    state = GameState::GAME_OVER;
                } else {
                    ResetPacman();
                }
            }
        }
    }
}

void Game::ResetPacman() {
    Position start = level->GetPacmanStart();
    pacman->Reset(start.x, start.y);
    pacman->Revive();

    // Reset ghosts to starting positions
    for (int i = 0; i < 4; i++) {
        Position ghostStart = level->GetGhostStart(i);
        ghosts[i]->Reset(ghostStart.x, ghostStart.y);
    }

    ghosts[0]->Activate();
    ghostReleaseTimer = 0.0f;
}

void Game::ResetLevel() {
    level->LoadLevel(currentLevel);

    Position pacmanStart = level->GetPacmanStart();
    pacman->Reset(pacmanStart.x, pacmanStart.y);

    for (int i = 0; i < 4; i++) {
        Position ghostStart = level->GetGhostStart(i);
        ghosts[i]->Reset(ghostStart.x, ghostStart.y);
    }

    ghosts[0]->Activate();
    ghostReleaseTimer = 0.0f;
    powerMode = false;
    powerModeTimer = 0.0f;
    state = GameState::PLAYING;
}

void Game::NextLevel() {
    currentLevel++;
    if (currentLevel > 2) {
        state = GameState::VICTORY;
        return;
    }
    ResetLevel();
}

void Game::Render() {
    Renderer::Clear(0.0f, 0.0f, 0.0f);

    // Render level
    level->Render();

    // Render Pac-Man
    float time = static_cast<float>(glfwGetTime());
    pacman->Render(time);

    // Render ghosts
    for (auto& ghost : ghosts) {
        ghost->Render();
    }

    // Render HUD
    Renderer::DrawNumber(score, 10, 10, 30);

    // Draw lives
    for (int i = 0; i < lives; i++) {
        Renderer::DrawPacman(50 + i * 30, WINDOW_HEIGHT - 30, 10, 0, 30, 1.0f, 1.0f, 0.0f);
    }

    // Level indicator
    Renderer::DrawNumber(currentLevel, WINDOW_WIDTH - 100, 10, 30);

    // Game over / victory messages
    if (state == GameState::GAME_OVER) {
        Renderer::DrawRect(WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT / 2 - 50,
                          300, 100, 0.0f, 0.0f, 0.0f);
        Renderer::DrawRect(WINDOW_WIDTH / 2 - 145, WINDOW_HEIGHT / 2 - 45,
                          290, 90, 1.0f, 0.0f, 0.0f);
    } else if (state == GameState::VICTORY) {
        Renderer::DrawRect(WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT / 2 - 50,
                          300, 100, 0.0f, 0.0f, 0.0f);
        Renderer::DrawRect(WINDOW_WIDTH / 2 - 145, WINDOW_HEIGHT / 2 - 45,
                          290, 90, 0.0f, 1.0f, 0.0f);
    }

    glfwSwapBuffers(window);
}

void Game::Run() {
    double lastTime = glfwGetTime();

    while (!glfwWindowShouldClose(window)) {
        double currentTime = glfwGetTime();
        float deltaTime = static_cast<float>(currentTime - lastTime);
        lastTime = currentTime;

        // Cap delta time to avoid large jumps
        if (deltaTime > 0.1f) deltaTime = 0.1f;

        glfwPollEvents();
        ProcessInput();
        Update(deltaTime);
        Render();
    }
}

void Game::Cleanup() {
    if (window) {
        glfwDestroyWindow(window);
        window = nullptr;
    }
    glfwTerminate();
}
