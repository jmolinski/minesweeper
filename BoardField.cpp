#include "BoardField.h"

const UI BoardField::FIELD_BOMB;
const UI BoardField::FIELD_MARKED;
const UI BoardField::FIELD_SHOWN;
const UI BoardField::FIELD_HIDDEN;

void BoardField::mark(UI& flags, const UI bombs_amount, bool& lose)
{
    if(this->shown == FIELD_HIDDEN && flags < bombs_amount)
    {
        this->shown = FIELD_MARKED;
        flags++;
    }
    else if(this->shown == FIELD_MARKED)
    {
        this->shown = FIELD_HIDDEN;
        flags--;
    }
}

void BoardField::clear()
{
    this->value = 'X';
    this->val_int = 0;
    this->shown = FIELD_HIDDEN;
}

void BoardField::update(bool& lose)
{
    if(this->shown == FIELD_HIDDEN)
        this->value = 'X';
    else if(this->shown == FIELD_MARKED)
        this->value = 'M';
    else if(this->val_int == FIELD_BOMB)
    {
        this->value = 'B';
        lose = true;
    }
    else
        this->value = int_to_char(this->val_int);
}

inline char BoardField::int_to_char(int x)
{
    return (x+48);
}

bool BoardField::is_a_bomb()
{
    return this->val_int == FIELD_BOMB;
}

bool BoardField::is_hidden()
{
    return this->shown == FIELD_HIDDEN;
}

bool BoardField::is_marked()
{
    return this->shown == FIELD_MARKED;
}

void BoardField::reveal()
{
    if(this->shown == FIELD_HIDDEN)
        this->shown = FIELD_SHOWN;
}
