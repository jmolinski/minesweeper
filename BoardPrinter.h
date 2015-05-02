#ifndef BOARDPRINTER_H
#define BOARDPRINTER_H

#include "Board.h"

class BoardPrinter
{
private:
    void print_main_body(Board* board);
    void print_row(Board* board, UI row);
    void print_line_separating_rows(UI board_size);
    void print_upper_body(UI board_size);
    void print_lower_body(UI board_size);
public:
    void print(Board* board);
};

#endif // BOARDPRINTER_H
