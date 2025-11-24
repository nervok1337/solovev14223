#ifndef _AI_H
#define _AI_H

#include "game.h"
#include <limits>
#include <utility>
#include <algorithm>

using std::numeric_limits;
using std::max;
using std::min;

class AI {
private:
    int alpha;
    int beta;
    int depth;

    int minimax(Game& game, int depth, bool maximizingPlayer);
    int evaluateFinalState(GameState st);
public:
    AI(int d);
    pair<int,int> getBestMove(Game& game);
};

#endif // _AI_H

