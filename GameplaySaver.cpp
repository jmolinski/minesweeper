#include <fstream>
#include "GameplaySaver.h"

bool GameplaySaver::load(Board* board, bool& show_zeros, UI& flags, bool& lose, UI& hidden_fields_amount)
{
    UI board_size;
    std::fstream saved_progress;
    saved_progress.open("saved_progress.txt", std::ios::in);
    if(saved_progress.good() == false)
        return false;
    saved_progress >> board_size;
    board = new Board(board_size);
    saved_progress >> board->bombs_amount >> show_zeros >> hidden_fields_amount >> flags >> lose;
    for(UI i = 0; i < board_size; i++)
        for(UI j = 0; j < board_size; j++)
            saved_progress >> board->board[i][j].value >> board->board[i][j].val_int >> board->board[i][j].shown;
    saved_progress.close();
    return true;
}

void GameplaySaver::save(Board* board, bool show_zeros, UI flags, bool lose, UI hidden_fields_amount)
{
    std::fstream saved_progress("saved_progress.txt", std::ios::out);
    saved_progress << board->get_board_size() << "\n" << board->bombs_amount << "\n" << show_zeros << "\n";
    saved_progress << hidden_fields_amount << "\n" << flags << "\n" << lose << "\n";
    for(UI i = 0; i < board->get_board_size(); i++)
        for(UI j = 0; j < board->get_board_size(); j++)
            saved_progress << board->board[i][j].value << " " << board->board[i][j].val_int << " " << board->board[i][j].shown << "\n";
    saved_progress.close();
}
