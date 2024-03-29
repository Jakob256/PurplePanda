# Tuning

The tuning of the Piece-Square Tables for middle- and endgame uses [Texel's Tuning Method](https://www.chessprogramming.org/Texel%27s_Tuning_Method).
The training data consists of around 400.000 games of 2300+ Elo rated games played on lichess.org played in January 2022, found [here](https://database.nikonoel.fr/).

The tuning is broken down into 3 steps, all of them are implemented in R:


## Step 1

The downloaded games are converted into the two files gamesLan.rds (which are the games in lan notation i.e. "e2e4 d7d5 e4d5 ..." instead of "1. e4 d5 2. exd5 ...") and results.rds (which is a list of 0s, 1s and 1/2s corresponding the results of the games. Additionally, games that ended in less than 20 ply are deleted.


## Step 2

From each game one board position is chosen. The only condition is, that the very next move in the game wasn't a capture (which is checked using PurplePanda10). The boards are stored in boards.rds using PurplePanda's board representation. 


## Step 3

A very efficient logistic regression is used to find optimal Piece-Square Tables for the middle- and endgame. The transition between them is based on the "game phase", which depends on how many pieces (i.e. not pawns) are on the board.

