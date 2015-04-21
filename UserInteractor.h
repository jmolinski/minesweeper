#ifndef USERINTERACTOR_H
#define USERINTERACTOR_H

#include "Translator.h"
#include "Board.h"

struct validated_input
{
    std::string action;
    int row;
    int col;
};

struct unverified_input
{
    std::string action;
    std::string row;
    std::string col;
};

typedef validated_input val_input;
typedef unsigned int UI;

class UserInteractor
{
private:
    Translator translator;
    std::string string_of_x_spaces(UI x);
    void print_board_main_body();
    void print_row(UI row);
    void print_line_separating_rows();
    void print_board_upper_body();
    void print_board_lower_body();
public:
    Board* board_;
    UI specify_board_size();
    UI specify_bombs_amount();
    bool specify_zeroes_shown();
    std::string ask_about_prefered_language();
    void print_board();
    val_input take_command(UI, UI);
    val_input validate_input(unverified_input unverified);
    void set_language();
    void game_saved_message();
    std::string continue_or_end_game_answer();
    void end_game_message();
    void continue_game_message();
    void game_finished_messages(bool lose);
    void play_once_again_question_message();
    void no_saved_progress_error_message();
    std::string select_game_mode_question();
    bool to_int(int& val, std::string str_val);
};

#endif // USERINTERACTOR_H
