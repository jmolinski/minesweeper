#ifndef BOARD_H
#define BOARD_H

typedef unsigned int UI;

#include "BoardField.h"

class Board
{
    UI board_size;
    UI bombs_amount;
    bool show_zeros;

    BoardField** board;

public:
    Board(const UI board_size, const UI bombs_amount, const bool show_zeros);
    ~Board();
    void clear_boards();
    UI get_board_size();
    UI get_bombs_amount();
    bool is_coord_inside_board(const int x, const int y);
    void update_board();
    UI count_bombs_around_field(int row, int col);
    UI get_hidden_fields_amount();
    UI get_flags_amount();
    void set_fields_values();
    void set_bombs_on_board();
    bool stepped_on_bomb();
    void mark(const UI row, const UI col);
    void reveal(const UI row, const UI col);
    void update(const UI row, const UI col);
    char get_value(const UI row, const UI col);
    std::string serialize_field(const UI row, const UI col);
    void deserialize_field(const UI row, const UI col, const UI val_int, const UI shown, const char value);
};

#endif // BOARD_H
