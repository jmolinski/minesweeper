#include <fstream>
#include "GameplaySaver.h"

bool GameplaySaver::load(Board* board)
{
    UI board_size, bombs_amount, val_int, shown;
    char value;
    bool show_zeros;
    std::fstream saved_progress;
    saved_progress.open("saved_progress.txt", std::ios::in);
    if(saved_progress.good() == false)
        return false;
    saved_progress >> board_size >> bombs_amount >> show_zeros;
    board = new Board(board_size, bombs_amount, show_zeros);
    for(UI i = 0; i < board_size; i++)
        for(UI j = 0; j < board_size; j++)
        {
            saved_progress >> value >> val_int >> shown;
            board->deserialize_field(i, j, val_int, shown, value);
        }
    saved_progress.close();
    return true;
}

void GameplaySaver::save(Board* board)
{
    std::fstream saved_progress("saved_progress.txt", std::ios::out);
    saved_progress << board->get_board_size() << "\n" << board->get_bombs_amount() << "\n";
    for(UI i = 0; i < board->get_board_size(); i++)
        for(UI j = 0; j < board->get_board_size(); j++)
            saved_progress << board->serialize_field(i, j);
    saved_progress.close();
}
