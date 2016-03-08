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
	    vector<Move> moves = possibleMoves();

        /*For testing and printing out all possible moves         
        for (unsigned int i = 0; i < moves.size(); i++)
        {
            std::cerr << moves[i].getX() << " " << moves[i].getY() << std::endl;
        }
        */ 

        int score = -100; //random negative value 
        int new_score;
        Move *move = new Move(moves[0].getX(), moves[0].getY());
        if (testingMinimax)
        {
            for (unsigned int i = 0; i < moves.size(); i++)

            {
                
                new_score = minimax(b, &moves[i], 2, self);
                //std::cerr << "current move: " << moves[i].getX() << " , " << moves[i].getY() << endl;
                //std::cerr << "final new_score: " << new_score << endl;
                if (new_score > score)
                {

                    //std::cerr << "final score: " << new_score << endl;
                    score = new_score;
                    *move = Move(moves[i].getX(), moves[i].getY());
                } 
            }
            b->doMove(move, self);
            return move;
        }
    
        score = b->doHeuristic(&moves[0]);
        *move = Move(moves[0].getX(), moves[0].getY());
        //std::cerr << "past move: " << move->getX() << " , " << move->getY() << endl;
        //std::cerr << "past score: " << score << endl;
        for (unsigned int i = 1; i < moves.size(); i++)
        {
            new_score = b->doHeuristic(&moves[i]);
            if (new_score > score)
            {
                score = new_score;
                *move = Move(moves[i].getX(), moves[i].getY());
            } 
        }
        //std::cerr << "current move: " << move->getX() << " , " << move->getY() << endl;
        //std::cerr << "current score: " << score << endl;
        b->doMove(move, self);
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
	        if (b->checkMove(&move, self))
            {
		        moves.push_back(move);
                
                //std::cerr<< i << " " << j <<endl;;
            }
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
    vector<Move> moves;
    int new_score, score = 0;    
    if (depth == 0)
    {
        //Board *copy = to_copy->copy(); 
        //copy->doMove(to_move, player);    
        int final_score = to_copy->naiveHeuristic(player);
        //delete copy;
        return final_score;
    }
    
    if (player == self)
    {
        score = -100;
        Board *copy1 = to_copy->copy(); 
        copy1->doMove(to_move, self); 
        std::cerr << "first_move " << to_move->getX() << to_move->getY() << endl;
        for (int i = 0; i < 8; i++) 
        {
	        for (int j = 0; j < 8; j++) 
            {
    	        Move move(i, j);
	            if (copy1->checkMove(&move, other))
                {
		            moves.push_back(move);        

                }
	        }
        }
        for (unsigned int i = 0; i < moves.size(); i++)
        {
            std::cerr << "potential " << moves[i].getX() << moves[i].getY() << std::endl;
            new_score = minimax(copy1, &moves[i], depth-1, BLACK);
            if (new_score > score)
            {
                score = new_score;
            } 
        }
        delete copy1;
        return score; 
    }

    if (player != self)
    {
        score = 100;
        Board *copy2 = to_copy->copy(); 
        copy2->doMove(to_move, other); 
        std::cerr << "black move " << to_move->getX() << to_move->getY() << endl;
        for (int i = 0; i < 8; i++) 
        {
	        for (int j = 0; j < 8; j++) 
            {
    	        Move move(i, j);
	            if (copy2->checkMove(&move, self))
                {
		            moves.push_back(move);    
            std::cerr << "array " << moves[i].getX() << moves[i].getY() << std::endl;   
                }
	        }
        }
        for (unsigned int i = 0; i < moves.size(); i++)
        {    
            std::cerr << "black pot " << moves[i].getX() << moves[i].getY() << std::endl;
            new_score = minimax(copy2, &moves[i], depth-1, WHITE);
            if (new_score < score)
            {
                score = new_score;
            } 
        }
        delete copy2;
        return score;
    }
    
    return 0;
}

