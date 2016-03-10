#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <iostream>
#include <vector>
#include "common.h"
#include "board.h"
#include <cstdlib>
using namespace std;

class Player {

private: 
	Board board;
	Side pside;

public:
    Player(Side side);
    Player(Side side, Board* start_board);
    ~Player();
	void update_player();
	bool in_corner(Move *move);
	bool on_edge(Move *move);
	bool iscornervalid(Move *move, Board *b, Side side);
	int score_move(Board *b, Move *move, Side side_to_score, bool downweight);
	Move *greedy_heuristic(vector<Move*> valid_moves);	
	Move* minimax(vector<Move*> valid_moves);
	void update_board(Move* move, Side side);
	std::vector<Move*> get_valid_moves(Board *b, Side side);    
    Move *doMove(Move *opponentsMove, int msLeft);

    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;
};

#endif
