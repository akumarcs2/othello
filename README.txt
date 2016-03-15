I worked alone. 

To make my AI tournament-worthy, I added to the heuristic additional weighted scores that counted the number of moves available to the opponent after a given move was made, and additionally the number of frontier spaces available to the opponent.

Additionally, I changed the minimax routine to be a 4 level minimax. At this depth, I did not find that any alpha-beta pruning or iterative deepening was necessary, as decisions were made in a reasonable amount of time. After testing, it was clear that adding further minimax depth would have required some sort of thinning of the tree.

The combination of a more sophisticated heuristic and a many stage minimax exploration should be a formidable match for other AI's.

The move scoring heuristic takes the weighted average of the (white - black) score, the frontier piece score, and the moves available score. I tried many different weightings, for example, placing almost no emphasis on the actual number of pieces gained on a given move, since this is theoretically immaterial early in the game and restricting the opponent's movement should be more important. This approach did not seem to work, however, and a more balanced weighting was chosen.