#include "game.h"
#include "AI.h"

Game::Game(int sizeField, int winLength) : size(sizeField), winLength(winLength) {
    reset();
}

void Game::reset() {
    field = vector<vector<Player>>(size, vector<Player>(size, Player::None));
    currentPlayer = Player::Human;
}

bool Game::isCellEmpty(const int row, const int col) const {
    return field[row][col] == Player::None;
}

bool Game::makeHumanMove(const int row, const int col) {
    if (!isCellEmpty(row, col)) return false;
    field[row][col] = Player::Human;
    return true;
}
bool Game::makeAIMove(const int row, const int col) {
    if (!isCellEmpty(row, col)) return false;
    field[row][col] = Player::AI;
    return true;
}
void Game::undoMove(const int row, const int col) {
    field[row][col] = Player::None;
}

Player Game::getCurPlayer() const{
    return currentPlayer;
}

const Player& Game::at(const int row, const int col) const {
    return field[row][col];
}
GameState Game::getState() const {
    return evaluateState();
}

vector<pair<int,int>> Game::getAvailableMoves() const {
    vector<pair<int,int>> moves;
    for (size_t r = 0; r < size; ++r) {
        for (size_t c = 0; c < size; ++c) {
            if (field[r][c] == Player::None) moves.emplace_back(r, c);
        }
    }
    return moves;
}

bool Game::checkLine(int r, int c, int dr, int dc, Player p) const {
    for (int i = 0; i < winLength; ++i) {
        int nr = r + dr*i;
        int nc = c + dc*i;
        if (nr < 0 || nr >= size || nc < 0 || nc >= size || field[nr][nc] != p)
            return false;
    }
    return true;
}

GameState Game::evaluateState() const {
    for (int r = 0; r < size; ++r)
        for (int c = 0; c < size; ++c) {
            Player p = field[r][c];
            if (p == Player::None) continue;

            if (checkLine(r, c, 0, 1, p) ||
                checkLine(r, c, 1, 0, p) ||
                checkLine(r, c, 1, 1, p) || 
                checkLine(r, c, 1, -1, p)) {
                return (p == Player::Human) ? GameState::HumanWin : GameState::AIWin;
            }
        }

    for (int r = 0; r < size; ++r)
        for (int c = 0; c < size; ++c)
            if (field[r][c] == Player::None)
                return GameState::InProgress;

    return GameState::Draw;
}


void Game::run(int depth) {
    AI ai(depth);

    while (true) {
        cout << "  ";
        for (int c = 0; c < size; ++c) cout << c % 10 << " ";
        cout << "\n";
        for (int r = 0; r < size; ++r) {
            cout << r % 10 << " ";
            for (int c = 0; c < size; ++c) {
                char sym = '.';
                if (field[r][c] == Player::Human) sym = 'X';
                else if (field[r][c] == Player::AI) sym = 'O';
                cout << sym << " ";
            }
            cout << "\n";
        }

        GameState st = getState();
        if (st == GameState::HumanWin) {
            cout << "Игрок выиграл!\n";
            break;
        }
        if (st == GameState::AIWin) { 
            cout << "AI выиграл!\n";
            break; 
        }
        if (st == GameState::Draw) {
            cout << "Ничья!\n";
            break;
        }

        if (currentPlayer == Player::Human) {
            int r, c;
            cout << "Ваш ход (row col): ";
            cin >> r >> c;
            if (r < 0 || r >= size || c < 0 || c >= size || !isCellEmpty(r,c)) {
                cout << "Неверный ход.\n";
                continue;
            }

            makeHumanMove(r,c);
            currentPlayer = Player::AI;
        } else {
            cout << "AI думает...\n";
            auto t1 = chrono::high_resolution_clock::now();
            auto mv = ai.getBestMove(*this);
            auto t2 = chrono::high_resolution_clock::now();
            auto ms = chrono::duration_cast<chrono::milliseconds>(t2-t1).count();
            cout << "Время расчётов: " << ms << " ms\n";
            makeAIMove(mv.first, mv.second);
            cout << "AI пошёл: " << mv.first << " " << mv.second << "\n";
            currentPlayer = Player::Human;
        }
    }
}

