#include "game.h"
#include "settings.h"

int main(int argc, char* argv[]) {
    Config config(argv[1]);
    Game g(config.size, config.winLength);
    g.run(config.depth);
    return 0;
}

