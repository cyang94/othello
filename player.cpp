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
    b = new Board();
    self = side;
    other = (self == BLACK) ? WHITE : BLACK;
}

/*
 * Destructor for the player.
 */
Player::~Player() {
    for (unsigned int i = 0; i < moves_made.size(); i++)
	delete moves_made[i];
    delete b;
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
    b->doMove(opponentsMove, other);
    if (b->hasMoves(self))
    {
	vector<Move*> moves = possibleMoves(b, self);

        int score = -100; //random negative value 
        int new_score;
        Move *move = moves[0];

	for (unsigned int i = 0; i < moves.size(); i++)
        {
	    new_score = minimax(b, moves[i], 2, self);
	    if (new_score > score)
	    {
		score = new_score;
		move = moves[i];
	    } 
	}
	b->doMove(move, self);
	moves_made.push_back(new Move(move->getX(), move->getY()));
	for (unsigned int i = 0; i < moves.size(); i++)
	    delete moves[i];
	return moves_made.back();
    }
    else
    {
	return NULL;
    }

}


/*
 * Populates a list of legal moves possible for player
 */
vector<Move*> Player::possibleMoves(Board *board, Side player) 
{
    vector<Move*> moves;

    // check for null
    //b->checkMove(NULL, self);
    // check for possible moves
    for (int i = 0; i < 8; i++) 
    {
	for (int j = 0; j < 8; j++) 
        {
    	    Move *move = new Move(i, j);
	    if (board->checkMove(move, player))
		moves.push_back(move);
	}
    }
    return moves;
}

/*
 *    Returns the minimax function result of the best move to play that
 *  maximizes the minimum gain of the player.
 */
 int Player::minimax(Board *to_copy, Move *to_move, int depth, Side player)
{
    vector<Move*> moves;
    int new_score, score = 100;
    Board *copy = to_copy->copy();

    //std::cerr << "current depth: " << depth << std::endl;
    if (depth == 1)
    {
        copy->doMove(to_move, player);    
        int final_score = copy->doHeuristic(to_move, player);
        delete copy;
        return final_score;
    }
    
    if (player == self)
    {
        copy->doMove(to_move, self); 
        // std::cerr << "first_move " << to_move->getX()
	//  	  << to_move->getY() << endl;

	moves = possibleMoves(copy, other);

	// print out potential moves
	// std::cerr << "potential moves: " << std::endl;
	// for (unsigned int i = 0; i < moves.size(); i++)
	//     std::cerr << moves[i]->getX() << ", "
	// 	      << moves[i]->getY() << std::endl;

        for (unsigned int i = 0; i < moves.size(); i++)
        {
            // std::cerr << "plays: " << moves[i]->getX() 
	    //    	      << moves[i]->getY() << std::endl;
            new_score = -minimax(copy, moves[i], depth-1, other);

            if (new_score < score)
                score = new_score;
	    delete moves[i];
        }
        delete copy;
        return score; 
    }

    if (player != self)
    {
        copy->doMove(to_move, other); 

	moves = possibleMoves(copy, self);
	for (unsigned int i = 0; i < moves.size(); i++)
	{    
	    new_score = minimax(copy, moves[i], depth-1, self);
	    
	    if (new_score < score)
		score = new_score;
	    
	    delete moves[i];
	}
        delete copy;
        return score;
    }

    return 0;
}

