#ifndef SAPPER_H
#define SAPPER_H

#include "Board.h"
#include "UserInteractor.h"

class Sapper
{
public:
    void run();
    Sapper();
private:
    void start_new_game_or_continue_saved_proggress();
    void update_board();
    void update_field(UI row, UI col);
    UI count_bombs_around_field(int row, int col);
    void init_game();
    void set_bombs_on_board();
    void set_fields_values();
    void set_field_value(UI row, UI col);
    void do_action(std::string action, UI row, UI col);
    void mark_field(UI row, UI col);
    void finish_game(bool);
    void continue_or_end();
    void main_game();
    void specify_settings();
    bool continue_saved_game_from_file();
    void save_progress_to_file();
    char int_to_char(int x);

    Board* board_;
    UserInteractor user_interactor;

    //UI BOARD_SIZE; // board is a grid of x * x fields
    UI bombs_amount; // number of bombs set on the board
    bool show_zeros; // show all zero-valued fields
    UI flags;
    bool lose;
    UI hidden_fields_amount;
};

#endif // SAPPER_H
