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
private:
    void do_action(std::string action, UI row, UI col);
    validated_input validate_input(validated_input);

    Board* board;
    UserInteractor* user_interactor;
};

#endif // GAMEPLAY_H
