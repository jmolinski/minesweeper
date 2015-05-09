#ifndef SAPPER_H
#define SAPPER_H

#include "UserInteractor.h"
#include "GameplaySaver.h"
#include "Board.h"
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
    GameplaySaver* gameplay_saver;
    void init_game();
    void specify_settings();
    void run_gameplay();
};

#endif // SAPPER_H
