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
	vector<Move*> moves = b->possibleMoves(self);

        int score = -100; //random negative value 
        int new_score;
        Move *move = moves[0];

	for (unsigned int i = 0; i < moves.size(); i++)
        {
	    // new_score = minimax(b, moves[i], 2, self);
	    new_score = negamax(b, moves[i], 2, self, -100, 100);
	    // std::cerr << "minscore: " << new_score << endl;
	    if (new_score > score)
            {
		score = new_score;
		move = moves[i];
	    } 
	}
	b->doMove(move, self);
	// std::cerr << "Making Move: " << move->getX()
	// 	  << ", " << move->getY() << std::endl;
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
 * Returns negamax function result for the player
 * Adapted from minimax function
 */
int Player::negamax(Board *to_copy, Move *to_move, int depth, Side player,
		    int alpha, int beta)
{
    vector<Move*> moves;
    int new_score;
    Board *copy = to_copy->copy();

    Side opp = (player == self) ? other : self;
    // std::cerr << "Current Depth: " << depth << std::endl;
    // std::cerr << "PLAYER: " << player << " OPP: " << opp << std::endl;

    if (depth == 0)
    {
	int final_score;
	
	final_score = copy->doHeuristic(to_move, player);
	
	// std::cerr << "final score: " << final_score << endl;
	    
        delete copy;
        return final_score;
    }

    if (to_move != NULL)
    {
	// std::cerr << "PLAYER plays: " << to_move->getX() << ", "
	//    	  << to_move->getY() << std::endl;
	copy->doMove(to_move, player); 
    }

    moves = copy->possibleMoves(opp);

    // std::cerr << "OPP potential moves: " << std::endl;
    // for (unsigned int i = 0; i < moves.size(); i++)
    // 	std::cerr << moves[i]->getX() << ", "
    // 		  << moves[i]->getY() << std::endl;

    if (moves.size() == 0)
    {
	if (to_move == NULL)
	    depth = 1; // end game when both sides pass

	new_score = -negamax(copy, NULL, depth-1, opp, -beta, alpha);
	if (new_score > alpha)
	    alpha = new_score;
	   
	delete copy;
	return alpha;
    }

    for (unsigned int i = 0; i < moves.size(); i++)
    {
	new_score = -negamax(copy, moves[i], depth-1, opp, -beta, -alpha);
	// std::cerr << "new score: " << new_score << std::endl;
	if (new_score > alpha)
	{
	    alpha = new_score;
	    // std::cerr << "new alpha: " << new_score << std::endl;
	}
	if (new_score >= beta)
	    break;
    }

    for (unsigned int i = 0; i < moves.size(); i++)
	delete moves[i];

    delete copy;
    return alpha; 
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

    // std::cerr << "current depth: " << depth << std::endl;
    if (depth == 1)
    {
	int final_score;
	if (to_move != NULL)
	{
	    // std::cerr << "PLAYER plays: " << to_move->getX() << ", "
	    // 	      << to_move->getY() << std::endl;
	    copy->doMove(to_move, player); 
	    final_score = copy->naiveHeuristic(self);
	}
	else
	{
	    final_score = copy->naiveHeuristic(self);
	    // std::cerr << "PLAY PASSES" << std::endl;
	}
	    
	// std::cerr << "final score: " << final_score << std::endl;
        delete copy;
        return final_score;
    }
    
   if (player == self)
    {
	if (to_move != NULL)
	{
	    // std::cerr << "PLAYER plays: " << to_move->getX()
	    //   	      << ", " << to_move->getY() << std::endl;
	    copy->doMove(to_move, self); 
	}

	moves = copy->possibleMoves(other);

	// print out potential moves
	// std::cerr << "OTHER potential moves: " << std::endl;
	// for (unsigned int i = 0; i < moves.size(); i++)
	//     std::cerr << moves[i]->getX() << ", "
	// 	      << moves[i]->getY() << std::endl;

	if (moves.size() == 0)
	{
	    // std::cerr << "NO POSSIBLE MOVES FOR OTHER " << player << std::endl;
	    new_score = minimax(copy, NULL, depth-1, other);
	    if (new_score < score)
		score = new_score;
	    // std::cerr << "new score: " << new_score << std::endl;
	    delete copy;
	    return score;
	}

        for (unsigned int i = 0; i < moves.size(); i++)
        {
	    // std::cerr << "BLACK plays: " << moves[i]->getX()
	    //  	      << ", " << moves[i]->getY() << std::endl;
            new_score = minimax(copy, moves[i], depth-1, other);
            if (new_score < score)
	    {
		// std::cerr << "new score: " << new_score << std::endl;
                score = new_score;
	    }
	    delete moves[i];
        }
        delete copy;
        return score; 
    }

    if (player != self)
    {
	if (to_move != NULL)
	{
	    // std::cerr << "BLACK plays: " << to_move->getX()
	    // 	      << ", " << to_move->getY() << std::endl;
	    copy->doMove(to_move, other);
	} 

	moves = copy->possibleMoves(self);

	// std::cerr << "WHITE potential moves: " << std::endl;
	// for (unsigned int i = 0; i < moves.size(); i++)
	//     std::cerr << moves[i]->getX() << ", "
	// 	      << moves[i]->getY() << std::endl;

	if (moves.size() == 0) // pass turn
	{
	    new_score = minimax(copy, NULL, depth-1, self);
	    if (new_score < score)
		score = new_score;
	    // std::cerr << "new score: " << new_score << std::endl;
	    delete copy;
	    return score;
	}

	for (unsigned int i = 0; i < moves.size(); i++)
	{    
	    // std::cerr << "WHITE plays: " << moves[i]->getX()
	    // 	      << ", " << moves[i]->getY() << std::endl;
	    new_score = minimax(copy, moves[i], depth-1, self);
	    if (new_score < score)
	    {
		score = new_score;
		// std::cerr << "new score: " << new_score << std::endl;
	    }
	    delete moves[i];
	}
        delete copy;
        return score;
    }

    return 0;
}

