#include "Board.h"

void Board::clear_field(UI row, UI col)
{
    board[row][col].value = 'X';
    board[row][col].val_int = 0;
    board[row][col].shown = FIELD_HIDDEN;
}

void Board::clear_boards()
{
    // set all fields to its default value
    for(UI row = 0; row < board_size; row++)
        for(UI col = 0; col < board_size; col++)
            clear_field(row, col);
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

const UI Board::FIELD_BOMB;
const UI Board::FIELD_MARKED;
const UI Board::FIELD_SHOWN;
const UI Board::FIELD_HIDDEN;

bool Board::is_coord_inside_board(UI coord)
{
    return (coord >= 0 && coord < board_size);
}

bool Board::is_field_a_bomb(UI row, UI col)
{
    return board[row][col].val_int == Board::FIELD_BOMB;
}

UI Board::get_board_size()
{
    return this->board_size;
}

void Board::update_board(bool& lose)
{
    for(UI row = 0; row < board_size; row++)
        for(UI col = 0; col < board_size; col++)
            update_field(row, col, lose);
}

void Board::update_field(UI row, UI col, bool& lose)
{
    if(board[row][col].shown == Board::FIELD_HIDDEN)
        board[row][col].value = 'X';
    else if(board[row][col].shown == Board::FIELD_MARKED)
        board[row][col].value = 'M';
    else if(board[row][col].val_int == Board::FIELD_BOMB)
    {
        board[row][col].value = 'B';
        lose = true;
    }
    else
        board[row][col].value = int_to_char(board[row][col].val_int);
}

inline char Board::int_to_char(int x)
{
    return (x+48);
}

void Board::mark_field(UI row, UI col, UI& flags, const UI bombs_amount, bool& lose)
{
    if(board[row][col].shown == Board::FIELD_HIDDEN && flags < bombs_amount)
    {
        board[row][col].shown = Board::FIELD_MARKED;
        flags++;
    }
    else if(board[row][col].shown == Board::FIELD_MARKED)
    {
        board[row][col].shown = Board::FIELD_HIDDEN;
        flags--;
    }
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
                if(is_coord_inside_board(current_col) && is_field_a_bomb(current_row, current_col))
                    bombs++;
    }
    return bombs;
}
