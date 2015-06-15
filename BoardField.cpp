#include <sstream>
#include "BoardField.h"

const UI BoardField::FIELD_BOMB;
const UI BoardField::FIELD_MARKED;
const UI BoardField::FIELD_SHOWN;
const UI BoardField::FIELD_HIDDEN;

void BoardField::mark(const UI flags)
{
    if(this->state == FIELD_HIDDEN && flags)
        this->state = FIELD_MARKED;
    else if(this->state == FIELD_MARKED)
        this->state = FIELD_HIDDEN;
}

void BoardField::reveal()
{
    if(this->state == FIELD_HIDDEN)
        this->state = FIELD_SHOWN;
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
            this->state = FIELD_SHOWN;
    }
}

void BoardField::clear()
{
    this->value = 'X';
    this->val_int = 0;
    this->state = FIELD_HIDDEN;
}

void BoardField::update()
{
    if(this->state == FIELD_HIDDEN)
        this->value = 'X';
    else if(this->state == FIELD_MARKED)
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
    return this->state == FIELD_HIDDEN;
}

bool BoardField::is_marked()
{
    return this->state == FIELD_MARKED;
}

bool BoardField::is_revealed()
{
    return this->state == FIELD_SHOWN;
}

char BoardField::get_value()
{
    return this->value;
}

std::string BoardField::serialize()
{
    std::stringstream str;
    str << this->value << " " << this->val_int << " " << this->state << "\n";
    return str.str();
}

void BoardField::deserialize(const UI val_int, const UI state, const char value)
{
    this->value = value;
    this->state = state;
    this->val_int = val_int;
}
