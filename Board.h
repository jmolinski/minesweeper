#ifndef BOARD_H
#define BOARD_H

typedef unsigned int UI;

class Board
{
    UI board_size;

public:
    char** board;
    UI** board_int;
    UI** shown;
    Board(const UI board_size_arg);
    ~Board();
    void clear_boards();
    void clear_field(UI row, UI col);
    UI get_board_size();
    bool is_field_a_bomb(UI row, UI col);
    bool is_coord_inside_board(UI coord);
    void update_board(bool& lose);
    void update_field(UI row, UI col, bool& lose);
    char int_to_char(int x);
    void mark_field(UI row, UI col, UI& flags, const UI bombs_amount, bool& lose);
    UI count_bombs_around_field(int row, int col);
    UI bombs_amount;

    static const UI FIELD_BOMB = 9;
    static const UI FIELD_MARKED = 10;
    static const UI FIELD_SHOWN = 11;
    static const UI FIELD_HIDDEN = 12;
};

#endif // BOARD_H
