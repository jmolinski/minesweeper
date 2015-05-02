#include "BoardField.h"

const UI BoardField::FIELD_BOMB;
const UI BoardField::FIELD_MARKED;
const UI BoardField::FIELD_SHOWN;
const UI BoardField::FIELD_HIDDEN;

void BoardField::mark(const UI flags)
{
    if(this->shown == FIELD_HIDDEN && flags)
        this->shown = FIELD_MARKED;
    else if(this->shown == FIELD_MARKED)
        this->shown = FIELD_HIDDEN;
}

void BoardField::reveal()
{
    if(this->shown == FIELD_HIDDEN)
        this->shown = FIELD_SHOWN;
}

void BoardField::set_bomb()
{
    this->val_int = FIELD_BOMB;
}

void BoardField::set_value(bool show_zeros, UI bombs_around_field)
{
    if(this->val_int != FIELD_BOMB)
    {
        this->val_int = bombs_around_field;
        if(show_zeros && this->val_int == 0)
            this->shown = FIELD_SHOWN;
    }
}

void BoardField::clear()
{
    this->value = 'X';
    this->val_int = 0;
    this->shown = FIELD_HIDDEN;
}

void BoardField::update()
{
    if(this->shown == FIELD_HIDDEN)
        this->value = 'X';
    else if(this->shown == FIELD_MARKED)
        this->value = 'M';
    else if(this->val_int == FIELD_BOMB)
        this->value = 'B';
    else
        this->value = int_to_char(this->val_int);
}

inline char BoardField::int_to_char(int x)
{
    return (x+48);
}

bool BoardField::is_bomb()
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

bool BoardField::is_revealed()
{
    return this->shown == FIELD_SHOWN;
}
