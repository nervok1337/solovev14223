#ifndef _AI_H
#define _AI_H

#include "game.h"
#include <limits>
#include <algorithm>
#include <chrono>
#include <vector>
#include <utility>
#include <unordered_map>
#include <string>
#include <cmath>
#include <cassert>


class AI {
private:
    int depth;
    int timeLimitMs;

    chrono::steady_clock::time_point startTime;
    bool timeoutOccurred = false;

    unordered_map<string,int> tt;

    void orderMoves(Game& game, vector<pair<int,int>>& moves);
    void generateCandidateMoves(Game& game, vector<std::pair<int,int>>& moves);
    bool findImmediateWinOrBlock(Game& game, pair<int,int>& outMove);

    int minimax(Game& game, int d, int alpha, int beta, bool maximizing);
    int evaluateFinalState(GameState st);
    int heuristicEval(Game& game);

public:
    AI(int d, int t);
    pair<int,int> getBestMove(Game& game);
};

#endif // _AI_H

