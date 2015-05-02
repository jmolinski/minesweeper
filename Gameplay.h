#ifndef GAMEPLAY_H
#define GAMEPLAY_H

typedef unsigned int UI;

#include "Sapper.h"
#include "GameplaySaver.h"

class Gameplay
{
public:
    void run();
    Gameplay(UserInteractor* user_interactor, Board* board);
    ~Gameplay();
private:
    void setup();
    void main_game();
    void do_action(std::string action, UI row, UI col);

    Board* board;
    UserInteractor* user_interface;
};

#endif // GAMEPLAY_H
