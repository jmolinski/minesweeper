#ifndef USERINTERACTOR_H
#define USERINTERACTOR_H

#include "Translator.h"
#include "Board.h"
#include "BoardPrinter.h"

struct validated_input
{
    validated_input(std::string action, int row, int col);
    std::string action;
    int row;
    int col;
};

typedef unsigned int UI;

class UserInteractor
{
private:
    Board* board;
    Translator translator;
    BoardPrinter board_printer;
    bool to_int(int& val, std::string str_val);
    validated_input validate_input(std::string action_u, std::string row_u, std::string col_u);
public:
    UI specify_board_size();
    UI specify_bombs_amount(const UI board_size);
    bool specify_zeros_shown();
    void specify_UI_language();
    void print_board();
    validated_input take_command(UI, UI);
    void game_saved_message();
    bool continue_or_end_game_question();
    void end_game_message();
    void continue_game_message();
    void game_finished_message(bool lose);
    void play_once_again_message();
    void no_saved_progress_error_message();
    std::string select_game_mode_question();
    void set_board(Board* board);
};

#endif // USERINTERACTOR_H
