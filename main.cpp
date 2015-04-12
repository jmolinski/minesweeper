#include <iostream>
#include <time.h>
#include <windows.h>
#include <cstdlib>
#include <stdio.h>
#include <conio.h>
#include <string>
#include "main.h"
#include "translations.h"
#include "save.h"

bool to_int(int& val, string str_val)
{
    try
    {
        val = stoi(str_val);
    }
    catch(...)
    {
        val = 0;
        return false;
    }
    return true;
}

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
    else if(action == "save_game")
    {
        save_progress_to_file();
        cout << loc("\n\nSaved. [Press any button]", language);
        getch();
        exit(0);
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
    validated.action = ((act == "show" || act == "mark" || act == "save_game")?(act):("invalid"));
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
        answer = to_en(answer);
    }
    if(answer == "no")
    {
        cout << loc("Ok, goodbye! :) [press any button]", language);
        getch();
        exit(0);
    }
    cout<< loc("Here we go! [press any button]", language);
    init_game();
    getch();
    print_board();
}

void finish_game(bool lose)
{
    if(lose)
        cout << loc("\n\nWhat a pity! You stepped on a bomb!\n", language);
    else
        cout << loc("\n\nCongratulations! You marked all bombs correctly and saved many human beings!\n", language);
    cout<< loc("Would you like to play once again? [yes/no]\n", language);
    continue_or_end();
}

string ask_about_prefered_language()
{
    string selected;
    while(selected != "en" && selected != "EN" && selected != "PL" && selected != "pl" &&
            selected != "1" && selected != "2")
    {
        cout<<"Select language:\n1.EN [English]\n2.PL [Polski]\n>";
        cin>>selected;
    }
    if(selected == "1" || selected == "EN" || selected == "en")
        return "EN";
    if(selected == "2" || selected == "PL" || selected == "pl")
        return "PL";
    return "EN";
}

void specify_settings()
{
    specify_board_size();
    specify_bombs_amount();
    specify_zeroes_shown();
}

void specify_board_size()
{
    int input_int = 0;
    while(input_int > 99 || input_int < 1)
    {
        cout << loc("\nSpecify the size of the board (1 to 99)\n>", language);
        string input;
        cin>>input;
        if(to_int(input_int, input) == false || (input_int > 99 || input_int < 1))
            cout << loc("\nInvalid value!", language);
    }
    BOARD_SIZE = input_int;
}

void specify_bombs_amount()
{
    int input_int = 0;
    while(input_int > BOARD_SIZE*BOARD_SIZE || input_int < 1)
    {
        cout << loc("\nSpecify how many bombs will be set on the board (1 to the square od board size)\n>", language);
        string input;
        cin>>input;
        if(to_int(input_int, input) == false || (input_int > 99 || input_int < 1))
            cout << loc("\nInvalid value!", language);
    }
    BOMBS_AMOUNT = input_int;
}

void specify_zeroes_shown()
{
    string input;
    while(input != "true" && input != "false")
    {
        cout << loc("\nSpecify if the zeroes will be shown since the game begin: [true/false]\n>", language);
        cin>>input;
        input = to_en(input);
        if(input != "true" && input != "false")
            cout << loc("\nInvalid value!", language);
    }
    SHOW_ZEROES = ( (input == "true") ? (true) : (false) );
}

void start_new_game_or_continue_saved_proggress()
{
    string game_mode = "";
    while(game_mode != "1" && game_mode != "2")
    {
        cout << loc("\nSelect game mode:\n1.Start new game\n2.Continue saved game\n>", language);
        cin>>game_mode;
    }
    if(game_mode == "1")
    {
        specify_settings();
        init_game();
    }
    if(game_mode == "2")
        if(continue_saved_game_from_file() == false)
        {
            cout << loc("\nError! No saved progress available. [Press any botton to close game]", language);
            getch();
            exit(0);
        }
}

void start_game()
{
    language = ask_about_prefered_language();
    start_new_game_or_continue_saved_proggress();
    update_board();
    print_board();
    main_game();
}

void main_game()
{
    string action, row, col;
    while(true)
    {
        cout << loc("Flags left: ", language) << BOMBS_AMOUNT - flags << "\n";
        cout << loc("Tell me what to do! (action[show/mark/save_game], row[1-9] , col[1-9]):\n>", language);
        cin>>action>>row>>col;
        val_input vi = validate_input(to_en(action), row, col);
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
