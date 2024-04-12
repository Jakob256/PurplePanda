<p align="center" width="100%">
    <img width="33%" src="https://raw.github.com/Jakob256/PurplePanda/master/logo.png"> 
</p>


# PurplePanda Chess Engine

Have fun exploring the code or playing the engine (UCI).

My goal is to truely understand what it takes to create a 2000 Elo chess engine, and learn a bunch while creating it.


## Playing/Compiling it yourself


Compiling "UCI_wrapper.cpp" will give you the latest PurplePanda version.

```
g++ UCI_wrapper.cpp -o PurplePanda.exe -march=native -O3
```

Alternatively, find the executables under "Releases". You will have to copy the two provided dll files also to the same location as the engine.

## Versions

| Version |    Date     | Score against <p> previous version | [CCRL](https://computerchess.org.uk/ccrl/404/) rating | Changes |
|:-------:|:-----------:|:---------------:|:-----:|:---     |
|  16     | 12 Apr 2024 | 56% (+42 Elo)   |       |- tempo bonus |
|  15     | 29 Mar 2024 | 62% (+85 Elo)   |       |- tapered evaluation <br> - tuning of Piece-Square Tables for middle- and endgame based on Texel's tuning method on games from [lichess.org](https://lichess.org/)|
|  14     | 20 Jan 2024 | 58% (+56 Elo)   | 1460  |- improved move ordering; used at every depth <br> - more aggressive pruning at frontier nodes <br> - 3men Distance-To-Mate tablebase used by interior node recognizer|
|  13     | 4  Sep 2023 | 61% (+78 Elo)   |       |- beta-cutoffs at frontier nodes <br> - tuning of a search parameter|
|  12     | 8  Aug 2023 | 58% (+56 Elo)   | 1389  |- improved move generator: finding attacked squares/ pinned pieces <br> - sorting moves incorporated into the move generation; active at depth ≤ 5|
|  11     | 20 Jul 2023 | 63% (+92 Elo)   |       |- true iterative deepening: hash lookup for previous best moves at depth ≤ 5 <br> - iterative hash updates <br> - no pseudo-legal moves generated at high depths <br> - bug test introduced with perft|
|  10     | 22 Aug 2022 | 72% (+164 Elo)  | 1236  |- simple Piece-Square Tables for middle- and endgame <br> - PST evaluations are updated iteratively <br> - set PST before the search <br> - incomplete UCI support|

