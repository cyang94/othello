(2) Describe how and what each group member contributed for the past two weeks

- 1st week -

Cassidy - set up git repository, added initialization of player, and set up
board variables, initial work on doMove and possibleMoves, fixed minimax
algorithm and fixed some memory leaks.

Willis - fixed doMove and possibleMoves, added simple positional strategy 
heuristic to beat SimplePlayer, implemented initial minimax algorithm for
testminimax, added greedy heuristic function.

- 2nd week -

Cassidy - changed minimax algorithm to work for tournament heuristic, changed
minimax to negamax up to 4 depth (without running out of memory). Researched
heuristic strategies.

Willis - changed heuristic function to have strategies of positional weights,
evaporation checks, mobility. Added switching of strategies based on turns.

(3) Improvements to make AI worthy

Heuristic - We first had a pure positional strategy that checked the next
move that emphasized the importance of corners and weak moves. This simple
heuristic beat SimplePlayer but nothing above that. For the second week, we added
a bunch of new heuristics and weighted them "appropriately". The strategies
we chose to implement are: positional weights, where each move, both proposed and
already taken, are weighted to see which board layout is most favorable for
the overall board, a check to prevent being completely evaporated with 0 
possible moves, mobility to maximize our number of moves and minimize the 
opponents number of moves, and a continued huge emphasis on corners.

We also switched strategies depending on early, middle, and end game. Early 
game focuses on positional strategies and securing key spots, middle focuses 
on mobility and corners, and end game focuses on securing as many discs
as possible.

The reason we choose these strategies is because positional weights help maximize
the efficiency of the moves, focusing on higher valued spots suchas corners
and edges. Focusing on corners is important because the opponent cannot flank
us, and a secure corner begins creating an area of "stable discs" that cannot
be recaptured. Mobility is important for the middle game, because if you can
restrict the opponent's choices until he plays a poor move, it is beneficial.
Also keeping your own mobility allows you to not be trapped.

We think this will work because of some of the papers we read, but obviously,
we had some trouble truly implementing our strategies together as we still
don't even win most of the time (it's about 75%/25% on white/black against 
ConstantTimePlayer, and let's not even get started about BetterPlayer+).

We discussed a lot of other strategies, but did not really have time to implement
them. A lot of topics were thrown out such as limiting the frontier so
that the opponent does not have as many options to play, calculating stable
discs which emphasizes capturing corners and expanding from the corners to
have uncapturable discs, and using opening books to optimize the early game
with sure-fire good moves.

Our weighted score system right now is a little wonky. More testing needs to
be done to truly find the right combination that works best as right now
it's not truly that good.
