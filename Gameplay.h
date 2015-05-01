#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include "Sapper.h"
#include "GameplaySaver.h"

class Gameplay
{
public:
    void run();
    ~Gameplay();
private:
    void setup();
    void start_new_game_or_continue_saved_proggress();
    void main_game();
    void specify_settings();
    void init_game();
    bool continue_or_end();
    void do_action(std::string action, UI row, UI col);

    Sapper* sapper;
    Board* board;
    UserInteractor* user_interface;

    bool show_zeros; // show all zero-valued fields
    UI flags;
    bool lose;
    UI hidden_fields_amount;
};

#endif // GAMEPLAY_H
