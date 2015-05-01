#ifndef SAPPER_H
#define SAPPER_H

#include "Board.h"
#include "UserInteractor.h"

class Sapper
{
public:
    void set_board(Board* board);
    void set_bombs_on_board(const UI bombs_amount);
    void set_fields_values(bool show_zeros);

private:
    UI count_bombs_around_field(int row, int col);
    void set_field_value(UI row, UI col, bool show_zeros);
    void finish_game(bool);

    Board* board;
};

#endif // SAPPER_H
