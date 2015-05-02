#ifndef BOARDFIELD_H
#define BOARDFIELD_H

typedef unsigned int UI;

class BoardField
{
public:
    UI shown;
    UI val_int;
    char value;

    static const UI FIELD_BOMB = 9;
    static const UI FIELD_MARKED = 10;
    static const UI FIELD_SHOWN = 11;
    static const UI FIELD_HIDDEN = 12;

    void mark(const UI flags);
    void reveal();
    void clear();
    void update();
    char int_to_char(int x);
    bool is_bomb();
    bool is_hidden();
    bool is_marked();
    bool is_revealed();
    void set_value(bool show_zeros, UI bombs_around_field);
};

#endif // BOARDFIELD_H
