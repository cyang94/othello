#include "board.h"
#include <iostream>

/*
 * Make a standard 8x8 othello board and initialize it to the standard setup.
 */
Board::Board() {
    taken.set(3 + 8 * 3);
    taken.set(3 + 8 * 4);
    taken.set(4 + 8 * 3);
    taken.set(4 + 8 * 4);
    black.set(4 + 8 * 3);
    black.set(3 + 8 * 4);
}

/*
 * Destructor for the board.
 */
Board::~Board() {
}

/*
 * Returns a copy of this board.
 */
Board *Board::copy() {
    Board *newBoard = new Board();
    newBoard->black = black;
    newBoard->taken = taken;
    return newBoard;
}

bool Board::occupied(int x, int y) {
    return taken[x + 8*y];
}

bool Board::get(Side side, int x, int y) {
    return occupied(x, y) && (black[x + 8*y] == (side == BLACK));
}

void Board::set(Side side, int x, int y) {
    taken.set(x + 8*y);
    black.set(x + 8*y, side == BLACK);
}

bool Board::onBoard(int x, int y) {
    return(0 <= x && x < 8 && 0 <= y && y < 8);
}

 
/*
 * Returns true if the game is finished; false otherwise. The game is finished 
 * if neither side has a legal move.
 */
bool Board::isDone() {
    return !(hasMoves(BLACK) || hasMoves(WHITE));
}

/*
 * Returns true if there are legal moves for the given side.
 */
bool Board::hasMoves(Side side) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Move move(i, j);
            if (checkMove(&move, side)) return true;
        }
    }
    return false;
}

/*
 * Returns true if a move is legal for the given side; false otherwise.
 */
bool Board::checkMove(Move *m, Side side) {
    // Passing is only legal if you have no moves.
    if (m == NULL) return !hasMoves(side);

    int X = m->getX();
    int Y = m->getY();

    // Make sure the square hasn't already been taken.
    if (occupied(X, Y)) return false;

    Side other = (side == BLACK) ? WHITE : BLACK;
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dy == 0 && dx == 0) continue;

            // Is there a capture in that direction?
            int x = X + dx;
            int y = Y + dy;
            if (onBoard(x, y) && get(other, x, y)) {
                do {
                    x += dx;
                    y += dy;
                } while (onBoard(x, y) && get(other, x, y));

                if (onBoard(x, y) && get(side, x, y)) return true;
            }
        }
    }
    return false;
}

/*
 * Modifies the board to reflect the specified move.
 */
void Board::doMove(Move *m, Side side) {
    // A NULL move means pass.
    if (m == NULL) return;

    // Ignore if move is invalid.
    if (!checkMove(m, side)) return;

    int X = m->getX();
    int Y = m->getY();
    Side other = (side == BLACK) ? WHITE : BLACK;
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dy == 0 && dx == 0) continue;

            int x = X;
            int y = Y;
            do {
                x += dx;
                y += dy;
            } while (onBoard(x, y) && get(other, x, y));

            if (onBoard(x, y) && get(side, x, y)) {
                x = X;
                y = Y;
                x += dx;
                y += dy;
                while (onBoard(x, y) && get(other, x, y)) {
                    set(side, x, y);
                    x += dx;
                    y += dy;
                }
            }
        }
    }
    set(side, X, Y);
}

/*
 * Current count of given side's stones.
 */
int Board::count(Side side) {
    return (side == BLACK) ? countBlack() : countWhite();
}

/*
 * Current count of black stones.
 */
int Board::countBlack() {
    return black.count();
}

/*
 * Current count of white stones.
 */
int Board::countWhite() {
    return taken.count() - black.count();
}

/*
 * Sets the board state given an 8x8 char array where 'w' indicates a white
 * piece and 'b' indicates a black piece. Mainly for testing purposes.
 */
void Board::setBoard(char data[]) {
    taken.reset();
    black.reset();
    for (int i = 0; i < 64; i++) {
        if (data[i] == 'b') {
            taken.set(i);
            black.set(i);
        } if (data[i] == 'w') {
            taken.set(i);
        }
    }
}

/*
 * Calculates the board Heuristic score using the difference in pieces.
 */
int Board::naiveHeuristic(Side player)
{
    int score;
    // board position score = (# discs player has) - (# discs opponent has);
    //std::cerr << "current player: " << player << std::endl;
    //std::cerr << "current white: " << countWhite() << std::endl;
    //std::cerr << "current black: " << countBlack() << std::endl;
    if (player == WHITE)
    {
        score = countWhite() - countBlack();
    }
    else
    {
        score = countBlack() - countWhite();
    }
    //std::cerr << "current score: " << score << std::endl;
    return score;
}


/*
 * Calculates the board Heuristic score for the given possible moves. 
 */
int Board::doHeuristic(Move *move, Side player) 
{
    int score = 0, pscore = 0, m_score = 0, greedy_score = 0, evap_score = 0;
    int my_corner = 0, other_corner = 0;
    Side other = (player == BLACK) ? WHITE : BLACK;
    int numplays = countBlack() + countWhite() - 4;
    //std::cerr << "numplays: " << numplays << endl;
    // if player can't move

    if (move == NULL)
    {
	    score = naiveHeuristic(player);
	    return score;
    }

    // endgame greedy heuristic

    else if (numplays >= 45)
    {
	    greedy_score = naiveHeuristic(player);
        return greedy_score;
    }

    // positional strategy with emphasis on edges and corners
    else if (numplays < 30)
    {
    int static_score[8][8] = 

                  {{100, -20, 20,  10, 10, 20, -20, 100}, 
                   {-20, -50, -6,  -4, -4, -6, -50, -20},
                   {20,   -6,  7,  4, 4,  7,   -6, 20},
                   { 10,  -4,  4,  0, 0,  4,   -4,  10},
                   { 10,  -4,  4,  0, 0,  4,   -4,  10},
                   { 20,  -6,  7,  4, 4,  7,   -6, 20},
                   {-20, -50, -6,  -4, -4, -6, -50, -20},
                   {100, -20, 20,  10, 10, 20, -20, 100}};



    for (int i = 0; i < 8; i++) 
    {
	    for (int j = 0; j < 8; j++) 
        {
            if(onBoard(i, j))
            {
               if(get(other, i, j))
               {
                
                   pscore += -static_score[i][j];
               }
               else 
               {
                   pscore += static_score[i][j];
               }
            }
	    }
    }
    }


    // OPTION: other heuristics that can be implemented:
    
    // mobility strategy

    vector <Move*> moves = possibleMoves(player);
    int m_my_score = moves.size();
    
    if (numplays < 50)
    {
    vector <Move*> other_moves = possibleMoves(other);
    int m_opp_score = other_moves.size();

    if (get(player, 0, 0))
        my_corner++;
    else if (get(other, 0, 8))
        other_corner++;
    if (get(player, 0, 8))
        my_corner++;
    else if (get(other, 8, 0))
        other_corner++;
    if (get(player, 8, 0))
        my_corner++;
    else if (get(other, 8, 0))
        other_corner++;
    if (get(player, 8, 8))
        my_corner++;
    else if (get(other, 8, 8))
        other_corner++;
    
        
    m_score = 15 * (my_corner - other_corner) + 4 * (m_my_score - m_opp_score);
    }

    // check evaporation

    if (m_my_score == 0)
    {
        evap_score = 15 * naiveHeuristic(player);
    }



    // reduce frontier discs, mobility, stable discs, maximum discs strategy,
    // and parity (number of empty squares where even is bad)

    //std::cerr << "pscore: " << pscore << endl;
    //std::cerr << "mobility score: " << m_score << endl;
    score = pscore + 10 * m_score + 12 * greedy_score  + evap_score;
    return score;

}

/*
 * Populates a list of legal moves possible for player
 */
vector<Move*> Board::possibleMoves(Side player) 
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
	    if (checkMove(move, player))
		moves.push_back(move);
	}
    }
    return moves;
}
