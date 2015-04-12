#include <fstream>

bool continue_saved_game_from_file()
{
    try
    {
        fstream saved_progress;
        saved_progress.open("saved_progress.txt", ios::in);
        saved_progress >> BOARD_SIZE >> BOMBS_AMOUNT >> SHOW_ZEROES >> hidden_fields_amount >> flags >> lose;
        for(UI i = 0; i < BOARD_SIZE; i++)
            for(UI j = 0; j < BOARD_SIZE; j++)
                saved_progress >> board[i][j] >> board_int[i][j] >> shown[i][j];
        saved_progress.close();
    }
    catch(...)
    {
        return false;
    }
    return true;
}

void save_progress_to_file()
{
    fstream saved_progress("saved_progress.txt", ios::out);
    saved_progress << BOARD_SIZE << "\n" << BOMBS_AMOUNT << "\n" << SHOW_ZEROES << "\n";
    saved_progress << hidden_fields_amount << "\n" << flags << "\n" << lose << "\n";
    for(UI i = 0; i < BOARD_SIZE; i++)
        for(UI j = 0; j < BOARD_SIZE; j++)
            saved_progress << board[i][j] << " " << board_int[i][j] << " " << shown[i][j] << "\n";
    saved_progress.close();
}
