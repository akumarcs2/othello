#include "board.h"

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

int Board::countMoves(Side side) {
    int nmoves = 0;
    for (int x = 0; x < 8; x++)
    {
        for(int y = 0; y < 8; y++)
        {
            Move *move = new Move(x, y);
            if(checkMove(move, side))
            {
                nmoves++;
            }
            delete move;
        }
    }

    return nmoves;
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
 * Retrieves the current board state so it can be set later using setBoard
 */
char *Board::getBoard()
{
    char *data = new char[64];
    for(int x = 0; x < 8; x++)
    {
        for(int y = 0; y < 8; y++)
        {
            if(taken[x + 8*y])
            {
                if(black[x + 8*y])
                {
                    data[x + 8*y] = 'b';
                }
                else
                {
                    data[x + 8*y] = 'w';
                }
            }
            else
            {
                data[x + 8*y] = 'x'; 
            }
        }
    }

    return data;
} 

/*
 * Draws the current state of the board out for debugging purposes
 */
void Board::draw()
{
    for(int x = 0; x < 8; x++)
    {
        for(int y = 0; y < 8; y++)
        {
            if(taken[y + x*8])
            {
                if(black[y + x*8])
                {
                    std::cerr << " B ";
                }
                else
                {
                    std::cerr << " W ";
                }
            }
            else
            {
                std::cerr << " * ";
            }
        }

        std::cerr << std::endl;
    }
}

// For the side given by input argument side, count the number of unoccupied,
// so called "fronteir" squares that 
int Board::countFrontier(Side side)
{
    // Keep track of spaces identified as fronteir spaces using a biset:
    bitset<64> frontier_spaces;
    for(int x = 0; x < 8; x++)
    {
        for(int y = 0; y < 8; y++)
        {
            // If side has a token at the space, count how many adjacent
            // peices: 
            if(get(side, x, y))
            {
                for(int xx = - 1; xx < 2; xx++)
                {
                    for(int yy = -1; yy < 2; yy++)
                    {
                        if(onBoard(xx, yy) && (!(xx == x && yy == y)))
                        {
                            if(!occupied(xx,yy))
                            {
                                if(!frontier_spaces[xx + 8*yy])
                                {
                                    frontier_spaces.set(xx + 8*yy);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    // Count how many places we marked:
    int nspaces = 0; 
    for(int i = 0; i < 64; i++)
    {
        if(frontier_spaces[i])
        {
            nspaces++;
        }
    }

    return nspaces;
}

/*
// Returns true if the board position at (x,y) is adjacent to the reference
// position at (x0, y0)
bool Board::is_adjacent(int x, int y, int x0, int y0)
{
    // Test location needs to be on the board, first and foremost:
    if(onBoard(x, y))
    {
        
        if((abs(x - x0) == 1 || abs(y - y0) == 1) && 
            (abs(x - x0) <= 1 && abs(y - y0) <= 1))
        {
            return true;
        }
    }

    return false;

}
*/
