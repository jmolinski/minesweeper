#include <iostream>
#include "BoardPrinter.h"

void BoardPrinter::print_upper_body(UI board_size)
{
    printf("\n\n");
    printf("                    ");
    for(UI i = 0; i < board_size; i++)
    {
        std::string col = "%i  ";
        col += [](int x)->std::string {return ((x>8)?(""):(" "));}(i);
        printf(col.c_str(), i+1);
    }
    printf("\n                   ");
    for(UI i = 0; i < board_size*4-1; i++)
        printf("_");
    printf("\n");
}

void BoardPrinter::print_line_separating_rows(UI board_size)
{
    printf("|");
    for(UI j = 0; j < board_size-1; j++)
        printf("---+");
    printf("---|\n");
}

void BoardPrinter::print_row(Board* board, UI row)
{
    printf(" | ");
    for(UI col = 0; col < board->get_board_size(); col++)
        std::cout << board->board[row][col].value << " | ";
    printf("%i\n", row+1);
}

void BoardPrinter::print_main_body(Board* board)
{
    for(UI row = 0; row < board->get_board_size(); row++)
    {
        std::string spaces = [](UI row_num)->std::string {return ((row_num>8)?(""):(" "));}(row);
        spaces += "               %i";
        printf(spaces.c_str(), row+1);
        print_row(board, row);
        spaces = std::string(spaces.length(), ' ');
        spaces += [](UI row_num)->std::string {return ((row_num>8)?(" "):(""));}(row);
        printf(spaces.c_str());
        if(row < board->get_board_size()-1)
            print_line_separating_rows(board->get_board_size());
    }
}

void BoardPrinter::print_lower_body(UI board_size)
{
    printf(" ");
    for(UI i = 0; i < board_size*4-1; i++)
        printf("-");
    printf("\n                    ");
    for(UI i = 0; i < board_size; i++)
    {
        std::string col = "%i  ";
        col += [](int x)->std::string {return ((x<=8)?(" "):(""));}(i);
        printf(col.c_str(), i+1);
    }
    printf("\n\n");
}

void BoardPrinter::print(Board* board)
{
    print_upper_body(board->get_board_size());
    print_main_body(board);
    print_lower_body(board->get_board_size());
}
