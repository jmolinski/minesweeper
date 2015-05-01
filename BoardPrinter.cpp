#include <cstdlib>
#include <iostream>
#include <string>
#include "BoardPrinter.h"

void BoardPrinter::print_board_upper_body()
{
    printf("\n\n");
    printf("                    ");
    for(UI i = 0; i < board->get_board_size(); i++)
    {
        std::string col = "%i  ";
        col += [](int x)->std::string {return ((x>8)?(""):(" "));}(i);
        printf(col.c_str(), i+1);
    }
    printf("\n                   ");
    for(UI i = 0; i < board->get_board_size()*4-1; i++)
        printf("_");
    printf("\n");
}

void BoardPrinter::print_line_separating_rows()
{
    printf("|");
    for(UI j = 0; j < board->get_board_size()-1; j++)
        printf("---+");
    printf("---|\n");
}

void BoardPrinter::print_row(UI row)
{
    printf(" | ");
    for(UI col = 0; col < board->get_board_size(); col++)
        std::cout << board->board[row][col] << " | ";
    printf("%i\n", row+1);
}

void BoardPrinter::print_board_main_body()
{
    for(UI row = 0; row < board->get_board_size(); row++)
    {
        std::string spaces = [](UI row_num)->std::string {return ((row_num>8)?(""):(" "));}(row);
        spaces += "               %i";
        printf(spaces.c_str(), row+1);
        print_row(row);
        spaces = std::string(spaces.length(), ' ');
        spaces += [](UI row_num)->std::string {return ((row_num>8)?(" "):(""));}(row);
        printf(spaces.c_str());
        if(row < board->get_board_size()-1)
            print_line_separating_rows();
    }
}

void BoardPrinter::print_board_lower_body()
{
    printf(" ");
    for(UI i = 0; i < board->get_board_size()*4-1; i++)
        printf("-");
    printf("\n                    ");
    for(UI i = 0; i < board->get_board_size(); i++)
    {
        std::string col = "%i  ";
        col += [](int x)->std::string {return ((x<=8)?(" "):(""));}(i);
        printf(col.c_str(), i+1);
    }
    printf("\n\n");
}

void BoardPrinter::print_board(Board* board)
{
    this->board = board;
    print_board_upper_body();
    print_board_main_body();
    print_board_lower_body();
}
