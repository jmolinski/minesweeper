#ifndef GAMEPLAYSAVER_H
#define GAMEPLAYSAVER_H

#include "Board.h"
#include "Gameplay.h"

class GameplaySaver
{
public:
    void save_progress_to_file(Board* board, UI bombs_amount, bool show_zeros, UI flags, bool lose, UI hidden_fields_amount);
    bool continue_saved_game_from_file(Board* board, UI& bombs_amount, bool& show_zeros, UI& flags, bool& lose, UI& hidden_fields_amount);
};

#endif // GAMEPLAYSAVER_H
