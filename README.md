# PurplePanda Chess Engine

Have fun exploring the code or playing the engine.

Public and full UCI support since Version PurplePanda10


## Compiling it yourself

Compiling "UCI_wrapper.cpp" will give you the latest PurplePanda version.

## Vesion 11 (first public version): Score vs V10 63% (Elo +92)
* true iterative deepening: hash lookup for previous best moves at depth <=5

* iterative hash updates

* no pseudo-legal moves generated at high depths

* bug test introduced with perft


## Vesion 10 (first public version): Elo gain vs V9: +164
* added basic Piece-Square Tables for middle- and endgame.

* Piece-Square Table evaluations are updated iteratively

* uses oracle-approach to set Piece-Square Tables

* full UCI support (except analysis mode)

