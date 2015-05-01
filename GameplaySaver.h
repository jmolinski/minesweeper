#ifndef GAMEPLAYSAVER_H
#define GAMEPLAYSAVER_H

#include "Board.h"
#include "Gameplay.h"

class GameplaySaver
{
public:
    void save(Board* board, bool show_zeros, UI flags, bool lose, UI hidden_fields_amount);
    bool load(Board* board, bool& show_zeros, UI& flags, bool& lose, UI& hidden_fields_amount);
};

#endif // GAMEPLAYSAVER_H
