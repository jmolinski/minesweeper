#ifndef SAPPER_H
#define SAPPER_H

#include "Board.h"
#include "UserInteractor.h"
#include "Gameplay.h"

class Sapper
{
    UserInteractor* user_interactor;
    Board* board;
    void init_game();
public:
    Sapper();
    ~Sapper();
    void run();
    void start_new_game_or_continue_saved_proggress();
    void specify_settings();
    bool continue_or_end();
};

#endif // SAPPER_H
