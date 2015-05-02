#ifndef SAPPER_H
#define SAPPER_H

#include "Board.h"
#include "UserInteractor.h"
#include "Gameplay.h"

class Sapper
{
public:
    Sapper();
    ~Sapper();
    void run();
private:
    UserInteractor* user_interactor;
    Board* board;
    void init_game();
    void specify_settings();
    bool continue_or_end();
    void start_new_game_or_continue_saved_proggress();
};

#endif // SAPPER_H
