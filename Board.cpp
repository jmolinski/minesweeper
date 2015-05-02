#include <cstdlib>
#include "Board.h"

void Board::clear_boards()
{
    for(UI row = 0; row < board_size; row++)
        for(UI col = 0; col < board_size; col++)
            board[row][col].clear();
}

Board::Board(const UI board_size)
{
    this->board_size = board_size;
    board = new BoardField*[board_size+1];

    for(UI i = 0; i < board_size+1; i++)
        board[i] = new BoardField[board_size+1];
}

Board::~Board()
{
    for(UI i = 0; i < board_size+1; i++)
        delete[] board[i];
    delete[] board;
}

bool Board::is_coord_inside_board(UI coord)
{
    return (coord >= 0 && coord < board_size);
}

UI Board::get_board_size()
{
    return this->board_size;
}

UI Board::get_bombs_amount()
{
    return this->bombs_amount;
}

void Board::set_bombs_amount(UI bombs_amount)
{
    this->bombs_amount = bombs_amount;
}

void Board::update_board()
{
    for(UI row = 0; row < board_size; row++)
        for(UI col = 0; col < board_size; col++)
            board[row][col].update();
}

UI Board::count_bombs_around_field(int row, int col)
{
    int bombs = 0;
    int rows[3] = {row-1, row, row+1};
    int cols[3] = {col-1, col, col+1};
    for(int current_row: rows)
    {
        for(int current_col: cols)
            if(is_coord_inside_board(current_row) && is_coord_inside_board(current_col) && board[current_row][current_col].is_bomb())
                bombs++;
    }
    return bombs;
}

UI Board::get_hidden_fields_amount()
{
    UI hidden_fields_amount = 0;

    for(UI row = 0; row < board_size; row++)
        for(UI col = 0; col < board_size; col++)
            if(board[row][col].is_hidden())
                hidden_fields_amount++;

    return hidden_fields_amount;
}

UI Board::get_flags_amount()
{
    UI flags = 0;

    for(UI row = 0; row < board_size; row++)
        for(UI col = 0; col < board_size; col++)
            if(board[row][col].is_marked())
                flags++;

    return bombs_amount-flags;
}

void Board::set_fields_values()
{
    for(UI row = 0; row < board_size; row++)
        for(UI col = 0; col < board_size; col++)
            board[row][col].set_value(show_zeros, count_bombs_around_field(row, col));
}

void Board::set_bombs_on_board()
{
    UI bombs_set = 0;
    while(bombs_set != bombs_amount)
    {
        UI row = rand()% board_size;
        UI col = rand()% board_size;
        if(board[row][col].is_bomb() == false)
        {
            board[row][col].set_bomb();
            bombs_set++;
        }
    }
}

bool Board::stepped_on_bomb()
{
    for(UI row = 0; row < board_size; row++)
        for(UI col = 0; col < board_size; col++)
            if(board[row][col].is_bomb() && board[row][col].is_revealed())
                return true;
    return false;
}

void Board::set_show_zeros_value(const bool show_zeros)
{
    this->show_zeros = show_zeros;
}
