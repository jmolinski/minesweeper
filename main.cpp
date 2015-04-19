#include <iostream>
#include <time.h>
#include <windows.h>
#include <cstdlib>
#include <stdio.h>
#include <conio.h>
#include <string>
#include "translations.h"
#include "main.h"
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

Sapper::Sapper()
{
    user_interactor.setLanguage();
    start_new_game_or_continue_saved_proggress();
}

void Sapper::run()
{
    update_board();
    user_interactor.print_board();
    main_game();
}

void Sapper::clear_field(UI row, UI col)
{
    board[row][col] = 'X';
    board_int[row][col] = 0;
    shown[row][col] = FIELD_HIDDEN;
}

void Sapper::clear_boards()
{
    // set all fields to its default value
    for(UI row = 0; row < BOARD_SIZE; row++)
        for(UI col = 0; col < BOARD_SIZE; col++)
            clear_field(row, col);
}

void Sapper::set_bombs_on_board()
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

void Sapper::set_field_value(UI row, UI col)
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

void Sapper::set_fields_values()
{
    // set value to each non-bomb field
    for(UI row = 0; row < BOARD_SIZE; row++)
        for(UI col = 0; col < BOARD_SIZE; col++)
            set_field_value(row, col);
}

void Sapper::init_game()
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

inline bool Sapper::is_field_a_bomb(UI row, UI col)
{
    return board_int[row][col] == FIELD_BOMB;
}

UI Sapper::count_bombs_around_field(int row, int col)
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

inline bool Sapper::is_coord_inside_board(UI coord)
{
    return (coord >= 0 && coord < BOARD_SIZE);
}

void Sapper::update_field(UI row, UI col)
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

void Sapper::update_board()
{
    // update all fields of formal board
    for(UI row = 0; row < BOARD_SIZE; row++)
        for(UI col = 0; col < BOARD_SIZE; col++)
            update_field(row, col);
}

void Sapper::mark_field(UI row, UI col)
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

void Sapper::do_action(string action, UI row, UI col)
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
        user_interactor.gameSavedMessage();
        getch();
        exit(0);
    }
}

void Sapper::continue_or_end()
{
    user_interactor.playOnceAgainQuestionMessage();
    string answer = user_interactor.continue_or_end_gameAnswer();
    if(answer == "no")
    {
        user_interactor.endGameMessage();
        getch();
        exit(0);
    }
    user_interactor.continueGameMessage();
    init_game();
    getch();
    user_interactor.print_board();
}

void Sapper::specify_settings()
{
    BOARD_SIZE = user_interactor.specify_board_size();
    user_interactor.BOARD_SIZE = BOARD_SIZE;
    BOMBS_AMOUNT = user_interactor.specify_bombs_amount();
    SHOW_ZEROES = user_interactor.specify_zeroes_shown();
}

void Sapper::start_new_game_or_continue_saved_proggress()
{
    string game_mode = user_interactor.selectGameModeQuestion();
    if(game_mode == "new_game")
    {
        specify_settings();
        init_game();
    }
    if(game_mode == "saved_progress")
        if(continue_saved_game_from_file() == false)
        {
            user_interactor.noSavedProgressErrorMessage();
            getch();
            exit(0);
        }
}

void Sapper::main_game()
{
    string action, row, col;
    while(true)
    {
        val_input input = user_interactor.takeCommand();
        do_action(input.action, input.row, input.col);
        user_interactor.print_board();
        if((flags == BOMBS_AMOUNT && hidden_fields_amount == BOMBS_AMOUNT) || lose)
        {
            user_interactor.gameFinishedMessages(lose);
            continue_or_end();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

string UserInteractor::continue_or_end_gameAnswer()
{
    string answer;
    while(answer != "yes" && answer != "no")
    {
        cout<<">";
        cin>>answer;
        answer = translator.to_en(answer);
    }
    return answer;
}

void UserInteractor::gameFinishedMessages(bool lose)
{
    if(lose)
        cout << translator.loc("\n\nWhat a pity! You stepped on a bomb!\n");
    else
        cout << translator.loc("\n\nCongratulations! You marked all bombs correctly and saved many human beings!\n");
}

string UserInteractor::ask_about_prefered_language()
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

UI UserInteractor::specify_board_size()
{
    int input_int = 0;
    while(input_int > 99 || input_int < 1)
    {
        cout << translator.loc("\nSpecify the size of the board (1 to 99)\n>");
        string input;
        cin>>input;
        if(to_int(input_int, input) == false || (input_int > 99 || input_int < 1))
            cout << translator.loc("\nInvalid value!");
    }
    return static_cast<UI>(input_int);
}

UI UserInteractor::specify_bombs_amount()
{
    int input_int = 0;
    int max_b = BOARD_SIZE*BOARD_SIZE;
    while(input_int > max_b || input_int < 1)
    {
        cout << translator.loc("\nSpecify how many bombs will be set on the board (1 to the square od board size)\n>");
        string input;
        cin>>input;
        if(to_int(input_int, input) == false || (input_int > 99 || input_int < 1))
            cout << translator.loc("\nInvalid value!");
    }
    return static_cast<UI>(input_int);
}

bool UserInteractor::specify_zeroes_shown()
{
    string input;
    while(input != "true" && input != "false")
    {
        cout << translator.loc("\nSpecify if the zeroes will be shown since the game begin: [true/false]\n>");
        cin>>input;
        input = translator.to_en(input);
        if(input != "true" && input != "false")
            cout << translator.loc("\nInvalid value!");
    }
    return ( (input == "true") ? (true) : (false) );
}

string UserInteractor::selectGameModeQuestion()
{
    string game_mode = "";
    while(game_mode != "1" && game_mode != "2")
    {
        cout << translator.loc("\nSelect game mode:\n1.Start new game\n2.Continue saved game\n>");
        cin>>game_mode;
    }
    return (game_mode == "1" ? "new_game" : "saved_progress");
}

void UserInteractor::print_board_upper_body()
{
    printf("\n\n");
    printf("                    ");
    for(UI i = 0; i < this->BOARD_SIZE; i++)
    {
        string col = "%i  ";
        col += [](int x)->string {return ((x>8)?(""):(" "));}(i);
        printf(col.c_str(), i+1);
    }
    printf("\n                   ");
    for(UI i = 0; i < this->BOARD_SIZE*4-1; i++)
        printf("_");
    printf("\n");
}

string UserInteractor::string_of_x_spaces(UI x)
{
    string spaces;
    for(UI i = 0; i < x; i++)
        spaces += ' ';
    return spaces;
}

void UserInteractor::print_line_separating_rows()
{
    printf("|");
    for(UI j = 0; j < this->BOARD_SIZE-1; j++)
        printf("---+");
    printf("---|\n");
}

void UserInteractor::print_row(UI row)
{
    printf(" | ");
    for(UI col = 0; col < this->BOARD_SIZE; col++)
        cout << board[row][col] << " | ";
    printf("%i\n", row+1);
}

void UserInteractor::print_board_main_body()
{
    for(UI row = 0; row < this->BOARD_SIZE; row++)
    {
        string spaces = [](UI row_num)->string {return ((row_num>8)?(""):(" "));}(row);
        spaces += "               %i";
        printf(spaces.c_str(), row+1);
        print_row(row);
        spaces = string_of_x_spaces(spaces.length());
        spaces += [](UI row_num)->string {return ((row_num>8)?(" "):(""));}(row);
        printf(spaces.c_str());
        if(row < this->BOARD_SIZE-1)
            print_line_separating_rows();
    }
}

void UserInteractor::print_board_lower_body()
{
    printf(" ");
    for(UI i = 0; i < this->BOARD_SIZE*4-1; i++)
        printf("-");
    printf("\n                    ");
    for(UI i = 0; i < this->BOARD_SIZE; i++)
    {
        string col = "%i  ";
        col += [](int x)->string {return ((x<=8)?(" "):(""));}(i);
        printf(col.c_str(), i+1);
    }
    printf("\n\n");
}

void UserInteractor::print_board()
{
    system("CLS");
    print_board_upper_body();
    print_board_main_body();
    print_board_lower_body();
}

val_input UserInteractor::takeCommand()
{
    unverified_input ui;
    cout << translator.loc("Flags left: ") << BOMBS_AMOUNT - flags << "\n";
    cout << translator.loc("Tell me what to do! (action[show/mark/save_game], row[1-9] , col[1-9]):\n>");
    cin>>ui.action>>ui.row>>ui.col;
    return validate_input(ui);
}

val_input UserInteractor::validate_input(unverified_input unverified)
{
    val_input validated;
    int rown = BOARD_SIZE + 1, coln = BOARD_SIZE +1;
    unverified.action = translator.to_en(unverified.action);
    try
    {
        rown = stoi(unverified.row) - 1, coln = stoi(unverified.col) - 1;
    }
    catch(...)
    {
        // error when converting invalid value from
        // string to int with stoi(), ignore
    }
    validated.action = ((unverified.action == "show" || unverified.action == "mark" || unverified.action == "save_game")?(unverified.action):("invalid"));
    validated.row = ((is_coord_inside_board(rown))?(rown):(this->BOARD_SIZE+1));
    validated.col = ((is_coord_inside_board(coln))?(coln):(this->BOARD_SIZE+1));
    return validated;
}

void UserInteractor::noSavedProgressErrorMessage()
{
    cout << translator.loc("\nError! No saved progress available. [Press any botton to close game]");
}

void UserInteractor::endGameMessage()
{
    cout << translator.loc("Ok, goodbye! :) [press any button]");
}

void UserInteractor::continueGameMessage()
{
    cout << translator.loc("Here we go! [press any button]");
}

void UserInteractor::playOnceAgainQuestionMessage()
{
    cout<< translator.loc("Would you like to play once again? [yes/no]\n");
}

void UserInteractor::setLanguage()
{
    translator.set_language(ask_about_prefered_language());
}

inline bool UserInteractor::is_coord_inside_board(UI coord)
{
    return (coord >= 0 && coord < BOARD_SIZE);
}

void UserInteractor::gameSavedMessage()
{
    cout << translator.loc("\n\nSaved. [Press any button]");
}

int main()
{
    Sapper saper;
    saper.run();
    return 0;
}
