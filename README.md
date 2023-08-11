# PurplePanda Chess Engine

Have fun exploring the code or playing the engine.

Public and full UCI support since Version 10.


## Playing/Compiling it yourself

Compiling "UCI_wrapper.cpp" will give you the latest PurplePanda version.

```
cd src
g++ UCI_wrapper.cpp -o PurplePanda12.exe -march=native -O3
```

Alternatively, copy the Engine exe you like to play from the folder EngineVersion. Additionally, you might have to copy the two provided dll files also to the same location as the engine.


## Vesion 12: Score vs V11 58% (Elo +56)
* improved move generator: calculation of all attacked squares and pinned pieces; small speed improvement

* sorting moves incorporated into the move generation; active at depth <=5


## Vesion 11: Score vs V10 63% (Elo +92)
* true iterative deepening: hash lookup for previous best moves at depth <=5

* iterative hash updates

* no pseudo-legal moves generated at high depths

* bug test introduced with perft


## Vesion 10 (first public version): Score vs V9 72% (Elo +164)
* added basic Piece-Square Tables for middle- and endgame.

* Piece-Square Table evaluations are updated iteratively

* uses oracle-approach to set Piece-Square Tables

* full UCI support (except analysis mode)

