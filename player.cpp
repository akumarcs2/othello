#include "player.h"

/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish 
 * within 30 seconds.
 */
Player::Player(Side side) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;

    pside = side;
}

/* Alternative constructor for the player which sets the initial board state
 * to that contained in board. This is for testing with testminimax.
 */
Player::Player(Side side, Board* start_board) {
    
    testingMinimax = false;
    pside = side;
    board = *start_board;
}

/*
 * Destructor for the player.
 */
Player::~Player() {

    // Default Destructors should be sufficient

}

void Player::update_board(Move *move, Side side)
{
    board.doMove(move, side);
}

std::vector<Move*> Player::get_valid_moves(Board* b, Side side)
{
    std::vector<Move*> moves;
    bool valid_move;
    // Return a vector of all valid moves:
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            Move *test_move = new Move(i, j);
            valid_move = b->checkMove(test_move, side);
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

/* Determine whether the move is located in a corner of the board:
 */
bool Player::in_corner(Move *move)
{
    int x = move->getX();
    int y = move->getY();

    if(x == 0 && y == 0)
    {
        return true;
    }
    else if(x == 7 && y == 0)
    {
        return true;
    }
    else if(x == 0 && y == 7)
    {
        return true;
    }
    else if(x == 7 && y == 7)
    {
        return true;
    }

    return false;
}

// Determine whether the given move lies on the edge of the board
bool Player::on_edge(Move *move)
{
    int x = move->getX();
    int y = move->getY();

    if(x == 0 || x == 7 || y == 0 || y == 7)
    {
        return true;
    }

    return false;

}

// Determine whether the corner closest to the move is a valid move for
// the other player:
bool Player::iscornervalid(Move *move, Board *b, Side side)
{
    int x = move->getX();
    int y = move->getY();

    int closest_x;
    int closest_y;

    if(x > (7 - x))
    {
        closest_x = 0;
    }
    else
    {
        closest_x = 7;
    }

    if(y > (7 - y))
    {
        closest_y = 0;
    }
    else
    {
        closest_y = 7;
    }

    Move *test = new Move(closest_x, closest_y);

    if(side == BLACK)
    {   
        return b->checkMove(test, WHITE);
    }
    else
    {
        return b->checkMove(test, BLACK);
    }

}

// Associate a score with the board state that results if we play move
int Player::score_move(Board *b, Move* move, Side side_to_score, bool downweight)//, Side side_to_score)
{
    
    // Determine whether the corners are valid moves:
    //bool corner_valid_before = iscornervalid(move, test_board, side_to_play);

    //test_board->doMove(move, side_to_play);

    // Determine whether the corner move has now opened up as a result:
    //bool corner_valid_after = iscornervalid(move, test_board, side_to_play);

    // Associate the score as the difference between the number of your pieces
    // and your opponent's pieces: 
    int count_score = b->count(side_to_score);

    // Also keep track of mobility: 

    int mobility_score = b->countMoves(side_to_score);
    if(!downweight)
    {
        mobility_score *= -1;
    }

    // Weight the two respective score
    int score = 0.4 * count_score + 0.6 * mobility_score;

    if(downweight)    
    {
        if(in_corner(move))
        {
            score *= -3;
        }
    //    else if(on_edge(move))
    //    {
    //        score *= -1.5;
    //    }
    }
    else
    {
        if(in_corner(move))
        {
            score *= 3;
        }
    //    else if(on_edge(move))
    //    {
    //        score *= 1.5;
    //    }
    }
    /*
    else if(!(corner_valid_before) && corner_valid_after)
    {
        // If, however, making the move would open up the corner spot
        // for the opponent, heavily downweight it
        score *= -3;
    }
    */
    return score;
}

// Single step heuristic that just takes the maximum score of a given set of
// valid moves:
Move* Player::greedy_heuristic(vector<Move*> valid_moves)
{
    int nmoves = (int)valid_moves.size();

    // Save the current board state so we can reset:
    char *base_board = board.getBoard();
    Board *test_board = board.copy();

    test_board->doMove(valid_moves[0], pside);

    int max_score = score_move(test_board, valid_moves[0], pside, false);
    std::cerr << "Move score: " << max_score << std::endl;
    int max_ind = 0;

    for(int i = 1; i < nmoves; i++)
    {
        // Reset board:
        test_board->setBoard(base_board);

        // Make the next move:
        test_board->doMove(valid_moves[i], pside);

        int move_score = score_move(test_board, valid_moves[i], pside, false);

        std::cerr << "Move score: " << move_score << std::endl;
        if(move_score > max_score)
        {
            max_score = move_score;
            max_ind = i;
        }
    }

    return(valid_moves[max_ind]);

}


// 2 stage minimax heuristic:
Move* Player::minimax(vector<Move*> valid_moves)
{
    int nmoves = (int)valid_moves.size();
    // Copy the base board state:
    Board *test_board = board.copy();
    char *base_board = test_board->getBoard();

    int *min_end_score = new int[nmoves];
    int end_score;

    Side oside;

    if(pside == BLACK)
    {
        oside = WHITE;
    }
    else
    {
        oside = BLACK;
    }

    for(int i = 0; i < nmoves; i++)
    {
        // Make the move:
        test_board->doMove(valid_moves[i], pside);
        vector<Move*> test_moves = get_valid_moves(test_board, oside);
        
        // Need to check if the opponent still has valid moves left:
        if(!test_moves.empty())
        {


            // Now test each of these moves as the other player:
            


            char *base_board2 = test_board->getBoard();
            // Make the move as the other player:
            test_board->doMove(test_moves[0], oside);

            min_end_score[i] = score_move(test_board, test_moves[0], pside, true);

            for(int j = 1; j < (int)test_moves.size(); j++)
            {
                // Reset the test board:
                test_board->setBoard(base_board2);
                // Make the next move:
                test_board->doMove(test_moves[j], oside);

                //test_board->draw();

                end_score = score_move(test_board, test_moves[j], pside, true);

                if(end_score < min_end_score[i])
                {
                    min_end_score[i] = end_score;
                }
            }

            // Reset the test board
            test_board->setBoard(base_board);
        }
        else
        {
            // This means the opponent will have to pass:
            min_end_score[i] = score_move(test_board, valid_moves[i], pside, false);
        }

    }

    int final_max_score = min_end_score[0];
    int final_max_ind = 0;

    // Return the valid move that yields the maximum score in the end:
    for(int i = 1; i < nmoves; i++)
    {
        if(min_end_score[i] > final_max_score)
        {
            final_max_score = min_end_score[i];
            final_max_ind = i;
        }
    }

    return valid_moves[final_max_ind];


    //delete base_board[];
    //delete base_board2[];

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
    if(pside == BLACK)
    {
        update_board(opponentsMove, WHITE);
    }
    else
    {
        update_board(opponentsMove, BLACK);
    }

    // Print out the board for debugging purposes:
    // board.draw();

    // Get valid moves:
    std::vector<Move*>valid_moves = get_valid_moves(&board, pside);
    /*
    for(unsigned int i = 0; i < valid_moves.size(); i++)
    {
        std::cerr << "Valid Move " << i << ": " << "(" << valid_moves[i]->getX()
                                                << ", " << valid_moves[i]->getY()
                                                << ")" << std::endl;
    }
    */
    // If no valid moves, pass:
    if(!valid_moves.empty())
    {
        // Random choice:
        //Move* move_to_make = valid_moves[rand() % valid_moves.size()];

        // Greedy heuristic:
        
        //Move* move_to_make = greedy_heuristic(valid_moves);

        // 2 stage minmax tree:
        
        Move *move_to_make = minimax(valid_moves);

        // Update board accordingly
        update_board(move_to_make, pside);
        board.draw();
        return move_to_make;
    }


    return NULL;
}
