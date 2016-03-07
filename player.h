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
    ~Player();
	void update_player();
	void update_board(Move* move);
	std::vector<Move*> get_valid_moves();    
    Move *doMove(Move *opponentsMove, int msLeft);

    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;
};

#endif
