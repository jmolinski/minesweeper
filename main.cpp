#include <iostream>
#include <time.h>
#include <windows.h>
#include <cstdlib>
#include <stdio.h>
#include <conio.h>
#include <sstream>
#include <string>

using namespace std;

typedef unsigned int UI;

const UI BOARD_SIZE = 9; // board is a grid of x * x fields
const UI BOMBS = 18; // number of bombs
const bool SHOW_ZEROES = true; // show all zero-valued fields
// special values of fields
const UI FIELD_CLEAR = 0;
const UI FIELD_BOMB = 9;
const UI FIELD_MARKED = 10;
const UI FIELD_SHOWN = 11;
const UI FIELD_HIDDEN = 12;

struct validated_input
{
    string action;
    int row;
    int col;
};

typedef validated_input val_input;

// boards
string board[BOARD_SIZE][BOARD_SIZE]; // board that will be displayed to user
UI board_int[BOARD_SIZE][BOARD_SIZE]; // board with actual fields' values
UI shown[BOARD_SIZE][BOARD_SIZE]; // board with values: marked, hidden, shown for each field

// some global variables... :(
UI flags = 0;
bool lose = false;
UI hidden_fields = BOARD_SIZE * BOARD_SIZE;

// function index
void start_game();
void update_board();
void update_field(UI, UI);
UI count_bombs_around_field(int, int);
void print_board();
void print_board_main_body();
void print_board_upper_body();
void init_game();
void clear_boards();
void clear_field(UI, UI);
void set_bombs_on_board();
void set_fields_values();
void set_field_value(UI, UI);
void do_action(string, UI, UI);
void mark_field(UI, UI);
void finish_game(bool);
void continue_or_end();
val_input validate_input(string, string, string);
string to_string(int);
int to_string(string);

// convert int to string
template <class T>
inline std::string to_string(const T& t)
{
    std::stringstream ss;
    ss << t;
    return ss.str();
}

template <class T>
inline int to_int(const T& str)
{
    int i;
    istringstream iss(str);
    iss >> i;
    return i;
}

void print_board_upper_body()
{
    printf("                    ");
    for(UI i = 0; i < BOARD_SIZE; i++)
        printf("%i   ", i+1);
    printf("\n                   ");
    for(UI i = 0; i < BOARD_SIZE*4-1; i++)
        printf("_");
    printf("\n");
}

void print_board_main_body()
{
    for(UI i = 0; i < BOARD_SIZE; i++)
    {
        printf("                %i | ", i+1);
        for(UI j = 0; j < BOARD_SIZE; j++)
            cout << board[i][j] << " | ";
        printf("%i\n                  ", i+1);
        if(i < BOARD_SIZE-1)
        {
            printf("|");
            for(UI j = 0; j < BOARD_SIZE-1; j++)
                printf("---+");
            printf("---|\n");
        }
    }
}

void print_board()
{
    system("CLS");
    // print upper body of the board
    print_board_upper_body();
    // print main body of the board
    print_board_main_body();
    printf(" ");
    for(UI i = 0; i < BOARD_SIZE*4-1; i++)
        printf("-");
    printf("\n");
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
    while(dec_bombs != BOMBS)
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
            --hidden_fields;
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
    hidden_fields = BOARD_SIZE * BOARD_SIZE;
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
    for(int row_being_checked: rows)
    {
        if(row_being_checked < 0 || row_being_checked > (int)(BOARD_SIZE-1))
            continue;
        for(int col_being_checked: cols)
        {
            if(col_being_checked < 0 || col_being_checked > (int)(BOARD_SIZE-1))
                continue;
            if(is_field_a_bomb(row_being_checked, col_being_checked))
                bombs++;
        }
    }
    return bombs;
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
    if(shown[row][col] == FIELD_HIDDEN && flags < BOMBS)
    {
        shown[row][col] = FIELD_MARKED;
        flags++;
    }
    else if(shown[row][col] == FIELD_MARKED)
    {
        shown[row][col] = FIELD_HIDDEN;
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
        --hidden_fields;
    }
}

val_input validate_input(string act, string rowp, string colp)
{
    val_input validated;
    int rown = to_int(rowp) - 1, coln = to_int(colp) - 1;

    if(act == "show" || act == "mark")
        validated.action = act;
    else
        validated.action = "invalid";

    if(rown > (BOARD_SIZE-1) || rown < 0)
        validated.row = 0;
    else
        validated.row = rown;

    if(coln > (BOARD_SIZE-1) || coln < 0)
        validated.col = 0;
    else
        validated.col = coln;

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
        cout << "Flags left: " << BOMBS - flags << "\n";
        cout << "Tell me what to do! (action[show/mark], row[1-9] , col[1-9]):\n>";
        cin>>action>>row>>col;
        val_input vi = validate_input(action, row, col);
        do_action(vi.action, vi.row, vi.col);
        update_board();
        print_board();
        if((flags == BOMBS && hidden_fields == BOMBS) || lose)
            finish_game(lose);
    }
}

int main()
{
    start_game();
    return 0;
}