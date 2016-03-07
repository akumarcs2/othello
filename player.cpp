#include "player.h"

/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish 
 * within 30 seconds.
 */
Player::Player(Side side) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;

    // No initialization to be done, the board is automatically setup to the
    // default start state by the Board constructor.
    pside = side;
    std::cerr << side << std::endl;
}

/*
 * Destructor for the player.
 */
Player::~Player() {

    // Default Destructors should be sufficient

}

void Player::update_board(Move *move)
{
    if(pside == BLACK)
    {
        board.doMove(move, WHITE);
    }
    else
    {
        board.doMove(move, BLACK);
    }
}

std::vector<Move*> Player::get_valid_moves()
{
    std::vector<Move*> moves;
    bool valid_move;
    // Return a vector of all valid moves:
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            Move *test_move = new Move(i, j);
            valid_move = board.checkMove(test_move, pside);
            if(valid_move)
            {
                moves.push_back(test_move);
            }
            else
            {
                delete test_move;
            }

        }
    } 

    return moves;
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
Move *Player::doMove(Move *opponentsMove, int msLeft) {
    /* 
     * TODO: Implement how moves your AI should play here. You should first
     * process the opponent's opponents move before calculating your own move
     */ 

    // Update the board given the opponents move:
    update_board(opponentsMove);

    // Get valid moves:
    std::vector<Move*>valid_moves = get_valid_moves();
    for(unsigned int i = 0; i < valid_moves.size(); i++)
    {
        std::cerr << "Valid Move " << i << ": " << "(" << valid_moves[i]->getX()
                                                << ", " << valid_moves[i]->getY()
                                                << ")" << std::endl;
    }

    // If no valid moves, pass:
    if(!valid_moves.empty())
    {
        // Choose a move randomly:
        return valid_moves[rand() % valid_moves.size()];
    }

    return NULL;
}
