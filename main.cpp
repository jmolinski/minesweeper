#include <iostream>
#include <time.h>
#include <windows.h>
#include <cstdlib>
#include <stdio.h>
#include <conio.h>
#include <sstream>
#include <string>
// the header with variable and function declarations etc
#include "main.h"

void print_board_upper_body()
{
    printf("\n\n");
    printf("                    ");
    for(UI i = 0; i < BOARD_SIZE; i++)
    {
        string col = "%i  ";
        col += [](int x)->string {return ((x>8)?(""):(" "));}(i);
        printf(col.c_str(), i+1);
    }
    printf("\n                   ");
    for(UI i = 0; i < BOARD_SIZE*4-1; i++)
        printf("_");
    printf("\n");
}

string string_of_x_spaces(UI x)
{
    string spaces;
    for(UI i = 0; i < x; i++)
        spaces += ' ';
    return spaces;
}

void print_line_separating_rows()
{
    printf("|");
    for(UI j = 0; j < BOARD_SIZE-1; j++)
        printf("---+");
    printf("---|\n");
}

void print_row(UI row)
{
    printf(" | ");
    for(UI col = 0; col < BOARD_SIZE; col++)
        cout << board[row][col] << " | ";
    printf("%i\n", row+1);
}

void print_board_main_body()
{
    for(UI row = 0; row < BOARD_SIZE; row++)
    {
        string spaces = [](UI row_num)->string {return ((row_num>8)?(""):(" "));}(row);
        spaces += "               %i";
        printf(spaces.c_str(), row+1);
        print_row(row);
        spaces = string_of_x_spaces(spaces.length());
        spaces += [](UI row_num)->string {return ((row_num>8)?(" "):(""));}(row);
        printf(spaces.c_str());
        if(row < BOARD_SIZE-1)
            print_line_separating_rows();
    }
}

void print_board_lower_body()
{
    printf(" ");
    for(UI i = 0; i < BOARD_SIZE*4-1; i++)
        printf("-");
    printf("\n                    ");
    for(UI i = 0; i < BOARD_SIZE; i++)
    {
        string col = "%i  ";
        col += [](int x)->string {return ((x<=8)?(" "):(""));}(i);
        printf(col.c_str(), i+1);
    }
    printf("\n\n");
}

void print_board()
{
    system("CLS");
    print_board_upper_body();
    print_board_main_body();
    print_board_lower_body();
}

void clear_field(UI row, UI col)
{
    board[row][col] = 'X';
    board_int[row][col] = 0;
    shown[row][col] = FIELD_HIDDEN;
}

void clear_boards()
{
    // set all fields to its default value
    for(UI row = 0; row < BOARD_SIZE; row++)
        for(UI col = 0; col < BOARD_SIZE; col++)
            clear_field(row, col);
}

void set_bombs_on_board()
{
    // sets bombs on the board
    UI dec_bombs = 0;
    while(dec_bombs != BOMBS_AMOUNT)
    {
        UI row = rand()%BOARD_SIZE;
        UI col = rand()%BOARD_SIZE;
        if(board_int[row][col] != FIELD_BOMB)
        {
            board_int[row][col] = FIELD_BOMB;
            dec_bombs++;
        }
    }
}

void set_field_value(UI row, UI col)
{
    if(board_int[row][col] != FIELD_BOMB)
    {
        board_int[row][col] = count_bombs_around_field(row, col);
        // if SHOW_ZEROES is true - show zero-valued fields
        if(SHOW_ZEROES && board_int[row][col] == 0)
        {
            shown[row][col] = FIELD_SHOWN;
            --hidden_fields_amount;
        }
    }
}

void set_fields_values()
{
    // set value to each non-bomb field
    for(UI row = 0; row < BOARD_SIZE; row++)
        for(UI col = 0; col < BOARD_SIZE; col++)
            set_field_value(row, col);
}

void init_game()
{
    srand(time(NULL));
    // set each field to its default value
    clear_boards();
    set_bombs_on_board();
    hidden_fields_amount = BOARD_SIZE * BOARD_SIZE;
    // set values of all non-bomb fields
    set_fields_values();
    lose = false;
    flags = 0;
    update_board();
}

inline bool is_field_a_bomb(UI row, UI col)
{
    return board_int[row][col] == FIELD_BOMB;
}

UI count_bombs_around_field(int row, int col)
{
    // counts bombs around XY field
    int bombs = 0;
    int rows[3] = {row-1, row, row+1};
    int cols[3] = {col-1, col, col+1};
    for(int current_row: rows)
    {
        if(is_coord_inside_board(current_row))
            for(int current_col: cols)
                if(is_coord_inside_board(current_col) && is_field_a_bomb(current_row, current_col))
                    bombs++;
    }
    return bombs;
}

inline bool is_coord_inside_board(UI coord)
{
    return (coord >= 0 && coord < BOARD_SIZE);
}

void update_field(UI row, UI col)
{
    if(shown[row][col] == FIELD_HIDDEN)
        board[row][col] = 'X';
    else if(shown[row][col] == FIELD_MARKED)
        board[row][col] = 'M';
    else if(board_int[row][col] == FIELD_BOMB)
    {
        board[row][col] = 'B';
        lose = true;
    }
    else
        board[row][col] = to_string(board_int[row][col]);
}

void update_board()
{
    // update all fields of formal board
    for(UI row = 0; row < BOARD_SIZE; row++)
        for(UI col = 0; col < BOARD_SIZE; col++)
            update_field(row, col);
}

void mark_field(UI row, UI col)
{
    if(shown[row][col] == FIELD_HIDDEN && flags < BOMBS_AMOUNT)
    {
        shown[row][col] = FIELD_MARKED;
        update_field(row, col);
        flags++;
    }
    else if(shown[row][col] == FIELD_MARKED)
    {
        shown[row][col] = FIELD_HIDDEN;
        update_field(row, col);
        flags--;
    }
}

void do_action(string action, UI row, UI col)
{
    if(action == "mark")
        mark_field(row, col);
    else if(action == "show" && shown[row][col] == FIELD_HIDDEN)
    {
        shown[row][col] = FIELD_SHOWN;
        update_field(row, col);
        --hidden_fields_amount;
    }
}

val_input validate_input(string act, string rowp, string colp)
{
    val_input validated;
    int rown = BOARD_SIZE + 1, coln = BOARD_SIZE +1;
    try
    {
        rown = stoi(rowp) - 1, coln = stoi(colp) - 1;
    }
    catch(...)
    {
        // error when converting invalid value from
        // string to int with stoi(), ignore
    }
    validated.action = ((act == "show" || act == "mark")?(act):("invalid"));
    validated.row = ((is_coord_inside_board(rown))?(rown):(0));
    validated.col = ((is_coord_inside_board(coln))?(coln):(0));
    return validated;
}

void continue_or_end()
{
    string answer;
    while(answer != "yes" && answer != "no")
    {
        cout<<">";
        cin>>answer;
    }
    if(answer == "no")
    {
        cout << "Ok, goodbye! :) [press any button]";
        getch();
        exit(0);
    }
    cout<<"Here we go! [press any button]";
    init_game();
    getch();
    print_board();
}

void finish_game(bool lose)
{
    if(lose)
        cout << "\n\nWhat a pity! You stepped on a bomb!\n";
    else
        cout << "\n\nCongratulations! You marked all bombs correctly and saved many human beings!\n";
    cout<<"Would you like to play once again? [yes/no]\n";
    continue_or_end();
}

void start_game()
{
    init_game();
    print_board();
    string action, row, col;
    while(true)
    {
        cout << "Flags left: " << BOMBS_AMOUNT - flags << "\n";
        cout << "Tell me what to do! (action[show/mark], row[1-9] , col[1-9]):\n>";
        cin>>action>>row>>col;
        val_input vi = validate_input(action, row, col);
        do_action(vi.action, vi.row, vi.col);
        print_board();
        if((flags == BOMBS_AMOUNT && hidden_fields_amount == BOMBS_AMOUNT) || lose)
            finish_game(lose);
    }
}

int main()
{
    start_game();
    return 0;
}
