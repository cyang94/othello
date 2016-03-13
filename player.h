#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <iostream>
#include "common.h"
#include "board.h"
using namespace std;

class Player {

private:
    Side self;
    Side other;
    vector<Move*> moves_made;

public:
    Player(Side side);
    ~Player();

    Move *doMove(Move *opponentsMove, int msLeft);

    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;
    Board *b;
    int negamax(Board *to_copy, Move *to_move, int depth, Side player,
		int alpha, int beta);
    int minimax(Board *to_copy, Move *to_move, int depth, Side player);

};

#endif
