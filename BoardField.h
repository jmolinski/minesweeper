#ifndef BOARDFIELD_H
#define BOARDFIELD_H

#include <string>

typedef unsigned int UI;

class BoardField
{
private:
    static const UI FIELD_BOMB = 9;
    static const UI FIELD_MARKED = 10;
    static const UI FIELD_SHOWN = 11;
    static const UI FIELD_HIDDEN = 12;

    char int_to_char(int x);

    UI state;
    UI val_int;
    char value;

public:
    void mark(const UI flags);
    void reveal();
    void clear();
    void update();
    void set_bomb();
    bool is_bomb();
    bool is_hidden();
    bool is_marked();
    bool is_revealed();
    void set_value(bool show_zeros, UI bombs_around_field);
    std::string serialize();
    void deserialize(const UI val_int, const UI state, const char value);
    char get_value();
};

#endif // BOARDFIELD_H
