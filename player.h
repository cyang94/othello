#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <iostream>
#include <vector>
#include "common.h"
#include "board.h"
using namespace std;

class Player {

private:
    Side self;
    Side other;
    vector<Move*> moves_made;

    // determines list of possible moves
    vector<Move*> possibleMoves(Board *board, Side player);

public:
    Player(Side side);
    ~Player();

    Move *doMove(Move *opponentsMove, int msLeft);

    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;
    Board *b;
    int minimax(Board *to_copy, Move *to_move, int depth, Side player);

};

#endif
