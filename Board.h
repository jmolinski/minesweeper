#ifndef BOARD_H
#define BOARD_H

typedef unsigned int UI;

#include "BoardField.h"

class Board
{
    UI board_size;
    UI bombs_amount;

public:
    BoardField** board;

    Board(const UI board_size_arg);
    ~Board();
    void clear_boards();
    UI get_board_size();
    UI get_bombs_amount();
    UI set_bombs_amount(UI bombs_amount_arg);
    bool is_coord_inside_board(UI coord);
    void update_board(bool& lose);
    UI count_bombs_around_field(int row, int col);
    UI get_hidden_fields_amount();
};

#endif // BOARD_H
