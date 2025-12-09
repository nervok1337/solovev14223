#ifndef _GAME_H
#define _GAME_H

#include <vector>
#include <iostream>
#include <chrono>

using namespace std;

enum class Player {
    None = 0,
    Human,
    AI
};

enum class GameState {
    InProgress,
    HumanWin,
    AIWin,
    Draw
};

class Game {
private:
    vector<vector<Player>> field;
    int size;
    int winLength;
    vector<pair<int,int>> moveStack;
    vector<Player> playerStack;

    GameState evaluateState() const;
    bool checkLine(int r, int c, int dr, int dc, Player p) const;
    void printField();
public:
    Game(int sizeField = 3, int winLenght = 3);
    Player currentPlayer;
    void reset();
    void makeMove(int r, int c, Player p);
    void undoMove(const int row, const int col);
    Player getCurPlayer() const;
    
    GameState getState() const;
    bool isCellEmpty(const int row, const int col) const;
    vector<pair<int,int>> getAvailableMoves() const;

    const Player& at(const int row, const int col) const;
    void run(int depth, int time);

    int getSize() const;
    int getWinLength() const;
};

#endif //_GAME_H
