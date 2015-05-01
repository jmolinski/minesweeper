#ifndef BOARDPRINTER_H
#define BOARDPRINTER_H

#include "Board.h"

class BoardPrinter
{
private:
    Board* board;
    void print_board_main_body();
    void print_row(UI row);
    void print_line_separating_rows();
    void print_board_upper_body();
    void print_board_lower_body();
public:
    void print_board(Board* board);
};

#endif // BOARDPRINTER_H
