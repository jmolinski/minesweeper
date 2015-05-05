#ifndef GAMEPLAYSAVER_H
#define GAMEPLAYSAVER_H

#include "Board.h"

class GameplaySaver
{
public:
    void save(Board* board);
    bool load(Board* board);
};

#endif // GAMEPLAYSAVER_H
