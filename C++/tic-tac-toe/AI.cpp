#include "AI.h"

AI::AI(int d) : depth(d) {}

std::pair<int,int> AI::getBestMove(Game& game) {
    alpha = numeric_limits<int>::min();
    beta  = numeric_limits<int>::max();

    int bestScore = numeric_limits<int>::min();
    std::pair<int,int> bestMove = {-1, -1};

    for (auto& mv : game.getAvailableMoves()) {
        game.makeAIMove(mv.first, mv.second);

        int score = minimax(game, 0, false);

        game.undoMove(mv.first, mv.second);

        if (score > bestScore) {
            bestScore = score;
            bestMove = mv;
        }
    }
    return bestMove;
}

int AI::minimax(Game& game, int depth, bool maximizingPlayer) {
    GameState st = game.getState();
    if (st != GameState::InProgress)
        return evaluateFinalState(st);

    if (maximizingPlayer) {
        int maxEval = numeric_limits<int>::min();

        for (auto& mv : game.getAvailableMoves()) {
            game.makeAIMove(mv.first, mv.second);
            int eval = minimax(game, depth, false);
            game.undoMove(mv.first, mv.second);

            maxEval = max(maxEval, eval);
            alpha = max(alpha, eval);

            if (beta <= alpha) break;
        }
        return maxEval;
    }
    else {
        int minEval = numeric_limits<int>::max();

        for (auto& mv : game.getAvailableMoves()) {
            game.makeHumanMove(mv.first, mv.second);
            int eval = minimax(game, depth, true);
            game.undoMove(mv.first, mv.second);

            minEval = min(minEval, eval);
            beta = min(beta, eval);

            if (beta <= alpha) break;
        }
        return minEval;
    }
}

int AI::evaluateFinalState(GameState st) {
    switch (st) {
        case GameState::AIWin: return +10;
        case GameState::HumanWin: return -10;
        case GameState::Draw: return 0;
        default: return 0;
    }
}
