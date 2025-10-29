#include "Game.h"
#include <iostream>

int main() {
    std::cout << "==================================" << std::endl;
    std::cout << "       PAC-MAN GAME" << std::endl;
    std::cout << "==================================" << std::endl;
    std::cout << "Controls:" << std::endl;
    std::cout << "  W - Move Up" << std::endl;
    std::cout << "  A - Move Left" << std::endl;
    std::cout << "  S - Move Down" << std::endl;
    std::cout << "  D - Move Right" << std::endl;
    std::cout << "  R - Restart Game" << std::endl;
    std::cout << "  ESC - Exit" << std::endl;
    std::cout << "==================================" << std::endl;
    std::cout << std::endl;

    Game game;

    if (!game.Initialize()) {
        std::cerr << "Failed to initialize game!" << std::endl;
        return -1;
    }

    std::cout << "Game initialized successfully!" << std::endl;
    std::cout << "Starting game..." << std::endl;

    game.Run();
    game.Cleanup();

    std::cout << "Game ended. Thanks for playing!" << std::endl;

    return 0;
}
