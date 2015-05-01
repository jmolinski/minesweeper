#include <iostream>
#include "Sapper.h"

using namespace std;

void Sapper::set_board(Board* board)
{
    this->board = board;
}

inline char Sapper::int_to_char(int x)
{
    return (x+48);
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
        board -> board_int[row][col] = count_bombs_around_field(row, col);
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

UI Sapper::count_bombs_around_field(int row, int col)
{
    // counts bombs around XY field
    int bombs = 0;
    int rows[3] = {row-1, row, row+1};
    int cols[3] = {col-1, col, col+1};
    for(int current_row: rows)
    {
        if(board->is_coord_inside_board(current_row))
            for(int current_col: cols)
                if(board->is_coord_inside_board(current_col) && board -> is_field_a_bomb(current_row, current_col))
                    bombs++;
    }
    return bombs;
}

void Sapper::update_field(UI row, UI col, bool& lose)
{
    if(board->shown[row][col] == Board::FIELD_HIDDEN)
        board->board[row][col] = 'X';
    else if(board->shown[row][col] == Board::FIELD_MARKED)
        board->board[row][col] = 'M';
    else if(board->board_int[row][col] == Board::FIELD_BOMB)
    {
        board->board[row][col] = 'B';
        lose = true;
    }
    else
        board->board[row][col] = int_to_char(board->board_int[row][col]);
}

void Sapper::update_board(bool& lose)
{
    // update all fields of formal board
    for(UI row = 0; row < board->get_board_size(); row++)
        for(UI col = 0; col < board->get_board_size(); col++)
            update_field(row, col, lose);
}

void Sapper::mark_field(UI row, UI col, UI& flags, const UI bombs_amount, bool& lose)
{
    if(board->shown[row][col] == Board::FIELD_HIDDEN && flags < bombs_amount)
    {
        board->shown[row][col] = Board::FIELD_MARKED;
        flags++;
    }
    else if(board->shown[row][col] == Board::FIELD_MARKED)
    {
        board->shown[row][col] = Board::FIELD_HIDDEN;
        flags--;
    }
}
