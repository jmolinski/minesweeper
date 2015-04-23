#include <iostream>
#include "Sapper.h"

using namespace std;

void Sapper::set_board(Board* board)
{
    this->board_ = board;
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
        UI row = rand()%board_ -> get_board_size();
        UI col = rand()%board_ -> get_board_size();
        if(board_ -> board_int[row][col] != Board::FIELD_BOMB)
        {
            board_ -> board_int[row][col] = Board::FIELD_BOMB;
            dec_bombs++;
        }
    }
}

void Sapper::set_field_value(UI row, UI col, bool show_zeros, UI& hidden_fields_amount)
{
    if(board_ -> board_int[row][col] != Board::FIELD_BOMB)
    {
        board_ -> board_int[row][col] = count_bombs_around_field(row, col);
        //if show_zeros is true - show zero-valued fields
        if(show_zeros && board_ -> board_int[row][col] == 0)
        {
            board_ -> shown[row][col] = Board::FIELD_SHOWN;
            --hidden_fields_amount;
        }
    }
}

void Sapper::set_fields_values(bool show_zeros, UI& hidden_fields_amount)
{
    // set value to each non-bomb field
    for(UI row = 0; row < board_->get_board_size(); row++)
        for(UI col = 0; col < board_->get_board_size(); col++)
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
        if(board_->is_coord_inside_board(current_row))
            for(int current_col: cols)
                if(board_->is_coord_inside_board(current_col) && board_ -> is_field_a_bomb(current_row, current_col))
                    bombs++;
    }
    return bombs;
}

void Sapper::update_field(UI row, UI col, bool& lose)
{
    if(board_->shown[row][col] == Board::FIELD_HIDDEN)
        board_->board[row][col] = 'X';
    else if(board_->shown[row][col] == Board::FIELD_MARKED)
        board_->board[row][col] = 'M';
    else if(board_->board_int[row][col] == Board::FIELD_BOMB)
    {
        board_->board[row][col] = 'B';
        lose = true;
    }
    else
        board_->board[row][col] = int_to_char(board_->board_int[row][col]);
}

void Sapper::update_board(bool& lose)
{
    // update all fields of formal board
    for(UI row = 0; row < board_->get_board_size(); row++)
        for(UI col = 0; col < board_->get_board_size(); col++)
            update_field(row, col, lose);
}

void Sapper::mark_field(UI row, UI col, UI& flags, const UI bombs_amount, bool& lose)
{
    if(board_->shown[row][col] == Board::FIELD_HIDDEN && flags < bombs_amount)
    {
        board_->shown[row][col] = Board::FIELD_MARKED;
        flags++;
    }
    else if(board_->shown[row][col] == Board::FIELD_MARKED)
    {
        board_->shown[row][col] = Board::FIELD_HIDDEN;
        flags--;
    }
}
