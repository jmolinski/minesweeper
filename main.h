#ifndef MAIN_H
#define MAIN_H

using namespace std;

typedef unsigned int UI;

struct validated_input
{
    string action;
    int row;
    int col;
};

struct unverified_input
{
    string action;
    string row;
    string col;
};

typedef validated_input val_input;

#include "Board.h"

#include "UserInteractor.h"

#include "Sapper.h"

bool to_int(int& val, string str_val);

#endif // MAIN
