#include <cstdlib>
#include "Board.h"

void Board::clear_boards()
{
    for(UI row = 0; row < board_size; row++)
        for(UI col = 0; col < board_size; col++)
            board[row][col].clear();
}

Board::Board(const UI board_size, const UI bombs_amount, const bool show_zeros)
{
    this->board_size = board_size;
    this->bombs_amount = bombs_amount;
    this->show_zeros = show_zeros;

    board = new BoardField*[board_size];

    for(UI i = 0; i < board_size; i++)
        board[i] = new BoardField[board_size];
}

Board::~Board()
{
    for(UI i = 0; i < board_size; i++)
        delete[] board[i];
    delete[] board;
}

bool Board::is_coord_inside_board(const int x, const int y)
{
    return (x >= 0 && x < board_size) && (y >= 0 && y < board_size);
}

UI Board::get_board_size()
{
    return this->board_size;
}

UI Board::get_bombs_amount()
{
    return this->bombs_amount;
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
            if(is_coord_inside_board(current_row, current_col) && board[current_row][current_col].is_bomb())
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

void Board::mark(const UI row, const UI col)
{
    this->board[row][col].mark(get_flags_amount());
}

void Board::reveal(const UI row, const UI col)
{
    this->board[row][col].reveal();
}

void Board::update(const UI row, const UI col)
{
    this->board[row][col].update();
}

char Board::get_value(const UI row, const UI col)
{
    return this->board[row][col].get_value();
}

std::string Board::serialize_field(const UI row, const UI col)
{
    return this->board[row][col].serialize();
}

void Board::deserialize_field(const UI row, const UI col, const UI val_int, const UI shown, const char value)
{
    this->board[row][col].deserialize(val_int, shown, value);
}
