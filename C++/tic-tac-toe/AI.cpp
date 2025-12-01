#include "AI.h"
static const int INF = 1000000000;

static string serializeBoard(const Game& game) {
    string s;
    int n = game.getSize();
    s.reserve(n*n + 10);
    for (int r = 0; r < n; ++r) {
        for (int c = 0; c < n; ++c) {
            const Player p = game.at(r,c);
            char ch = (p == Player::None ? '.' : (p == Player::Human ? 'X' : 'O'));
            s.push_back(ch);
        }
    }
    return s;
}

AI::AI(int d, int t) : depth(d), timeLimitMs(t) {}

int AI::heuristicEval(Game& game) {
    int n = game.getSize();
    int W = game.getWinLength();
    static vector<int> weights;
    if ((int)weights.size() < W+2) {
        weights.assign(W+2, 1);
        for (int i = 1; i < (int)weights.size(); ++i)
            weights[i] = weights[i-1] * 10;
    }

    int score = 0;
    int dir[4][2] = { {1,0}, {0,1}, {1,1}, {1,-1} };

    for (int r = 0; r < n; ++r) {
        for (int c = 0; c < n; ++c) {
            const Player pc = game.at(r,c);
            if (pc == Player::None) continue;
            for (int d = 0; d < 4; ++d) {
                int dr = dir[d][0], dc = dir[d][1];
                int L = 0;
                int rr = r, cc = c;
                while (rr >= 0 && rr < n && cc >= 0 && cc < n && game.at(rr,cc) == pc) {
                    L++; 
                    rr += dr;
                    cc += dc;
                    if (L > W) break;
                }
                if (L == 0) continue;

                int openEnds = 0;
                if (rr >= 0 && rr < n && cc >= 0 && cc < n && game.at(rr,cc) == Player::None) openEnds++;
                rr = r - dr; 
                cc = c - dc;
                if (rr >= 0 && rr < n && cc >= 0 && cc < n && game.at(rr,cc) == Player::None) openEnds++;

                int v = 0;
                if (L >= W) {
                    v = weights[W] * 1000;
                } else {
                    v = weights[L] * (openEnds + 1);
                }

                if (pc == Player::AI) score += v;
                else score -= v;
            }
        }
    }

    return score;
}

void AI::generateCandidateMoves(Game& game, vector<pair<int,int>>& moves) {
    moves.clear();
    int n = game.getSize();

    vector<vector<char>> mark(n, vector<char>(n, 0));
    int radius = 2;

    bool any = false;
    for (int r = 0; r < n && !any; ++r) {
        for (int c = 0; c < n; ++c) {
            if (game.at(r,c) != Player::None) {
                any = true;
                break; 
            }
        }
    }

    if (!any) {
        int mid = n / 2;
        moves.push_back({mid, mid});
        return;
    }

    for (int r = 0; r < n; ++r) {
        for (int c = 0; c < n; ++c) {
            if (game.at(r,c) != Player::None) {
                for (int dr = -radius; dr <= radius; ++dr) {
                    for (int dc = -radius; dc <= radius; ++dc) {
                        int nr = r + dr, nc = c + dc;
                        if (nr >= 0 && nr < n && nc >= 0 && nc < n && game.at(nr,nc) == Player::None) {
                            mark[nr][nc] = 1;
                        }
                    }
                }
            }
        }
    }

    for (int r = 0; r < n; ++r)
        for (int c = 0; c < n; ++c)
            if (mark[r][c]) moves.emplace_back(r,c);

    if (moves.empty()) moves = game.getAvailableMoves();
}

bool AI::findImmediateWinOrBlock(Game& game, pair<int,int>& outMove) {
    auto moves = game.getAvailableMoves();
    // ищем выигрыш AI
    for (auto &mv : moves) {
        game.makeMove(mv.first, mv.second, Player::AI);
        if (game.getState() == GameState::AIWin) {
            game.undoMove(mv.first, mv.second);
            outMove = mv;
            return true;
        }
        game.undoMove(mv.first, mv.second);
    }
    // ищем выигрыш игрока в следующий ход
    for (auto &mv : moves) {
        game.makeMove(mv.first, mv.second, Player::Human);
        if (game.getState() == GameState::HumanWin) {
            game.undoMove(mv.first, mv.second);
            outMove = mv;
            return true;
        }
        game.undoMove(mv.first, mv.second);
    }
    return false;
}

// orderMoves: сортировка по быстрой эвристике
void AI::orderMoves(Game& game, vector<pair<int,int>>& moves) {
    vector<pair<int,int>> tmp = moves;

    struct S {
        pair<int,int> mv;
        int v;
    };
    vector<S> arr;
    arr.reserve(tmp.size());

    for (auto &mv : tmp) {
        game.makeMove(mv.first, mv.second, Player::AI);
        int v = heuristicEval(game);
        game.undoMove(mv.first, mv.second);
        arr.push_back({mv, v});
    }
    sort(arr.begin(), arr.end(), [](const S& a, const S& b){ return a.v > b.v; });
    moves.clear();
    for (auto &e : arr) moves.push_back(e.mv);
}

int AI::minimax(Game& game, int d, int alpha, int beta, bool maximizing) {
    using clock = chrono::steady_clock;
    auto now = clock::now();
    long long ms = chrono::duration_cast<chrono::milliseconds>(now - startTime).count();
    if (ms > timeLimitMs) {
        timeoutOccurred = true;
        return 0;
    }

    GameState st = game.getState();
    if (st != GameState::InProgress)
        return evaluateFinalState(st);

    if (d == 0) 
        return heuristicEval(game);

    // таблица: ключ = board + depth + maximizing
    string key = serializeBoard(game) + "#" + to_string(d) + (maximizing ? "M" : "m");
    auto it = tt.find(key);
    if (it != tt.end())
        return it->second;

    vector<pair<int,int>> moves;
    generateCandidateMoves(game, moves);
    if (moves.empty())
        moves = game.getAvailableMoves();

    orderMoves(game, moves);

    int bestVal;
    if (maximizing) {
        bestVal = -INF;
        for (auto &mv : moves) {
            game.makeMove(mv.first, mv.second, Player::AI);
            int val = minimax(game, d-1, alpha, beta, false);
            game.undoMove(mv.first, mv.second);
            // если время истекло - выходим
            if (timeoutOccurred) {
                tt[key] = bestVal;
                return bestVal;
            }

            if (val > bestVal) bestVal = val;
            if (val > alpha) alpha = val;
            if (beta <= alpha) break;
        }
    } else {
        bestVal = INF;
        for (auto &mv : moves) {
            game.makeMove(mv.first, mv.second, Player::Human);
            int val = minimax(game, d-1, alpha, beta, true);
            game.undoMove(mv.first, mv.second);

            if (timeoutOccurred) {
                tt[key] = bestVal;
                return bestVal;
            }

            if (val < bestVal) bestVal = val;
            if (val < beta) beta = val;
            if (beta <= alpha) break;
        }
    }

    tt[key] = bestVal;
    return bestVal;
}

pair<int,int> AI::getBestMove(Game& game) {
    pair<int,int> best = {-1,-1};
    int bestScore = -INF;

    pair<int,int> immediate;
    if (findImmediateWinOrBlock(game, immediate)) {
        return immediate;
    }

    vector<pair<int,int>> moves;
    generateCandidateMoves(game, moves);
    if (moves.empty())
        moves = game.getAvailableMoves();
    orderMoves(game, moves);

    using clock = chrono::steady_clock;
    startTime = clock::now();
    timeoutOccurred = false;
    tt.clear(); // очищаем таблицу перед итеративным углублением

    // Итеративное углубление
    for (int d = 1; d <= depth; ++d) {
        int curBestScore = -INF;
        pair<int,int> curBest = moves[0];

        for (auto &mv : moves) {
            game.makeMove(mv.first, mv.second, Player::AI);
            int alpha = -INF;
            int beta  = INF;

            int score = minimax(game, d-1, alpha, beta, false);
            game.undoMove(mv.first, mv.second);

            if (timeoutOccurred)
                return bestScore > -INF ? best : curBest;

            if (score > curBestScore) {
                curBestScore = score;
                curBest = mv;
            }
        }

        best = curBest;
        bestScore = curBestScore;

        // переупорядочим для следующей итерации
        orderMoves(game, moves);
    }

    return best;
}

int AI::evaluateFinalState(GameState st) {
    switch (st) {
        case GameState::AIWin: return +10000000;
        case GameState::HumanWin: return -10000000;
        case GameState::Draw: return 0;
        default: return 0;
    }
}

