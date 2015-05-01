#include <iostream>
#include "Sapper.h"

using namespace std;

void Sapper::set_board(Board* board)
{
    this->board = board;
}

void Sapper::set_bombs_on_board(const UI bombs_amount)
{
    // sets bombs on the board
    UI dec_bombs = 0;
    while(dec_bombs != bombs_amount)
    {
        UI row = rand()%board -> get_board_size();
        UI col = rand()%board -> get_board_size();
        if(board -> board_int[row][col] != Board::FIELD_BOMB)
        {
            board -> board_int[row][col] = Board::FIELD_BOMB;
            dec_bombs++;
        }
    }
}

void Sapper::set_field_value(UI row, UI col, bool show_zeros, UI& hidden_fields_amount)
{
    if(board -> board_int[row][col] != Board::FIELD_BOMB)
    {
        board -> board_int[row][col] = board -> count_bombs_around_field(row, col);
        //if show_zeros is true - show zero-valued fields
        if(show_zeros && board -> board_int[row][col] == 0)
        {
            board -> shown[row][col] = Board::FIELD_SHOWN;
            --hidden_fields_amount;
        }
    }
}

void Sapper::set_fields_values(bool show_zeros, UI& hidden_fields_amount)
{
    // set value to each non-bomb field
    for(UI row = 0; row < board->get_board_size(); row++)
        for(UI col = 0; col < board->get_board_size(); col++)
            set_field_value(row, col, show_zeros, hidden_fields_amount);
}
