#include <fstream>
#include "GameplaySaver.h"

bool GameplaySaver::continue_saved_game_from_file(Board* board, UI& bombs_amount, bool& show_zeros, UI& flags, bool& lose, UI& hidden_fields_amount)
{
    UI board_size;
    std::fstream saved_progress;
    saved_progress.open("saved_progress.txt", std::ios::in);
    if(saved_progress.good() == false)
        return false;
    saved_progress >> board_size;
    board = new Board(board_size);
    saved_progress >> bombs_amount >> show_zeros >> hidden_fields_amount >> flags >> lose;
    for(UI i = 0; i < board_size; i++)
        for(UI j = 0; j < board_size; j++)
            saved_progress >> board->board[i][j] >> board->board_int[i][j] >> board->shown[i][j];
    saved_progress.close();
    return true;
}

void GameplaySaver::save_progress_to_file(Board* board, UI bombs_amount, bool show_zeros, UI flags, bool lose, UI hidden_fields_amount)
{
    std::fstream saved_progress("saved_progress.txt", std::ios::out);
    saved_progress << board->get_board_size() << "\n" << bombs_amount << "\n" << show_zeros << "\n";
    saved_progress << hidden_fields_amount << "\n" << flags << "\n" << lose << "\n";
    for(UI i = 0; i < board->get_board_size(); i++)
        for(UI j = 0; j < board->get_board_size(); j++)
            saved_progress << board->board[i][j] << " " << board->board_int[i][j] << " " << board->shown[i][j] << "\n";
    saved_progress.close();
}
