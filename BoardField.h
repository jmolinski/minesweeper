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

    void mark(UI& flags, const UI bombs_amount, bool& lose);
    void reveal();
    void clear();
    void update(bool& lose);
    char int_to_char(int x);
    bool is_a_bomb();
    bool is_hidden();
    bool is_marked();
};

#endif // BOARDFIELD_H
