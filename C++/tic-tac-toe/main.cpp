#include "game.h"
#include "settings.h"

int main(int argc, char* argv[]) {
    Config config(argv[1]);
    Game g(config.size, config.winLength);
    g.currentPlayer = Player::AI;
    g.run(config.depth, config.time);
    return 0;
}

