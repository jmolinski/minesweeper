#include "Board.h"

void Board::clear_boards()
{
    // set all fields to its default value
    for(UI row = 0; row < board_size; row++)
        for(UI col = 0; col < board_size; col++)
            board[row][col].clear();
}

Board::Board(const UI board_size_arg)
{
    this->board_size = board_size_arg;

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

UI Board::set_bombs_amount(UI bombs_amount_arg)
{
    this->bombs_amount = bombs_amount_arg;
}

void Board::update_board(bool& lose)
{
    for(UI row = 0; row < board_size; row++)
        for(UI col = 0; col < board_size; col++)
            board[row][col].update(lose);
}

UI Board::count_bombs_around_field(int row, int col)
{
    // counts bombs around XY field
    int bombs = 0;
    int rows[3] = {row-1, row, row+1};
    int cols[3] = {col-1, col, col+1};
    for(int current_row: rows)
    {
        if(is_coord_inside_board(current_row))
            for(int current_col: cols)
                if(is_coord_inside_board(current_col) && board[current_row][current_col].is_a_bomb())
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
