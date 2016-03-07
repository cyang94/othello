/**
 * @file player.cpp
 * @author Cassidy Yang and Willis Nguy
 * @version 1.0
 * @date March 2016
 * @copyright This code is in the public domain.
 *
 */

#include "player.h"

/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish 
 * within 30 seconds.
 */
Player::Player(Side side) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;

    /* 
     * TODO: Do any initialization you need to do here (setting up the board,
     * precalculating things, etc.) However, remember that you will only have
     * 30 seconds.
     */

    self = side;
    other = (self == BLACK) ? WHITE : BLACK;
}

/*
 * Destructor for the player.
 */
Player::~Player() {
}

/*
 * Compute the next move given the opponent's last move. Your AI is
 * expected to keep track of the board on its own. If this is the first move,
 * or if the opponent passed on the last move, then opponentsMove will be NULL.
 *
 * msLeft represents the time your AI has left for the total game, in
 * milliseconds. doMove() must take no longer than msLeft, or your AI will
 * be disqualified! An msLeft value of -1 indicates no time limit.
 *
 * The move returned must be legal; if there are no valid moves for your side,
 * return NULL.
 */
Move *Player::doMove(Move *opponentsMove, int msLeft) 
{
    b.doMove(opponentsMove, other);
    if (b.hasMoves(self))
    {
	    vector<Move> moves = possibleMoves();
        /*For testing and printing out all possible moves         
        for (unsigned int i = 0; i < moves.size(); i++)
        {
            std::cerr << moves[i].getX() << " " << moves[i].getY() << std::endl;
        }
        */ 
        Move *move = new Move(moves[0].getX(), moves[0].getY());
        b.doMove(move, self);
	    return move;
    }
    else
    {
	    return NULL;
    }
}


/*
 * Populates a list of legal moves possible for player
 */
vector<Move> Player::possibleMoves() 
{
    vector<Move> moves;

    // check for null
    //b->checkMove(NULL, self);
    // check for possible moves
    for (int i = 0; i < 8; i++) 
    {
	    for (int j = 0; j < 8; j++) 
        {
    	    Move move(i, j);
	        if (b.checkMove(&move, self))
            {
		        moves.push_back(move);
                
                //std::cerr<< i << " " << j <<endl;;
            }
	    }
    }
    return moves;
}
