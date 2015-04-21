#include <iostream>
#include <time.h>
#include <windows.h>
#include <fstream>
#include <cstdlib>
#include <stdio.h>
#include <conio.h>
#include <string>
#include "translations.h"
#include "main.h"

inline char Sapper::int_to_char(int x)
{
    return (x+48);
}

inline UI Board::get_board_size()
{
    return this->board_size;
}

Board::Board(const UI board_size_arg)
{
    this->board_size = board_size_arg;

    board_int = new UI*[board_size+1];
    shown = new UI*[board_size+1];
    board = new char*[board_size+1];

    for(UI i = 0; i < board_size+1; i++)
    {
        board_int[i] = new UI[board_size+1];
        shown[i] = new UI[board_size+1];
        board[i] = new char[board_size+1];
    }
}

Board::~Board()
{
    for(UI i = 0; i < board_size+1; i++)
    {
        delete[] board_int[i];
        delete[] shown[i];
        delete[] board[i];
    }

    delete[] board_int;
    delete[] shown;
    delete[] board;
}

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

const UI Board::FIELD_CLEAR;
const UI Board::FIELD_BOMB;
const UI Board::FIELD_MARKED;
const UI Board::FIELD_SHOWN;
const UI Board::FIELD_HIDDEN;

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

void Board::clear_field(UI row, UI col)
{
    board[row][col] = 'X';
    board_int[row][col] = 0;
    shown[row][col] = FIELD_HIDDEN;
}

void Board::clear_boards()
{
    // set all fields to its default value
    for(UI row = 0; row < board_size; row++)
        for(UI col = 0; col < board_size; col++)
            clear_field(row, col);
}

void Sapper::set_bombs_on_board()
{
    // sets bombs on the board
    UI dec_bombs = 0;
    while(dec_bombs != BOMBS_AMOUNT)
    {
        UI row = rand()%board_ -> get_board_size();
        UI col = rand()%board_ -> get_board_size();
        if(board_ -> board_int[row][col] != Board::FIELD_BOMB)
        {
            board_ -> board_int[row][col] = Board::FIELD_BOMB;
            dec_bombs++;
        }
    }
}

void Sapper::set_field_value(UI row, UI col)
{
    if(board_ -> board_int[row][col] != Board::FIELD_BOMB)
    {
        board_ -> board_int[row][col] = count_bombs_around_field(row, col);
        // if SHOW_ZEROES is true - show zero-valued fields
        if(SHOW_ZEROES && board_ -> board_int[row][col] == 0)
        {
            board_ -> shown[row][col] = Board::FIELD_SHOWN;
            --hidden_fields_amount;
        }
    }
}

void Sapper::set_fields_values()
{
    // set value to each non-bomb field
    for(UI row = 0; row < board_->get_board_size(); row++)
        for(UI col = 0; col < board_->get_board_size(); col++)
            set_field_value(row, col);
}

void Sapper::init_game()
{
    srand(time(NULL));
    // set each field to its default value
    board_ -> clear_boards();
    set_bombs_on_board();
    hidden_fields_amount = board_->get_board_size() * board_->get_board_size();
    // set values of all non-bomb fields
    set_fields_values();
    lose = false;
    flags = 0;
    hidden_fields_amount = board_->get_board_size() * board_->get_board_size();
    update_board();
}

bool Sapper::continue_saved_game_from_file()
{
    try
    {
        UI BOARD_SIZE;
        fstream saved_progress;
        saved_progress.open("saved_progress.txt", ios::in);
        saved_progress >> BOARD_SIZE;
        board_ = new Board(BOARD_SIZE);
        saved_progress >> BOMBS_AMOUNT >> SHOW_ZEROES >> hidden_fields_amount >> flags >> lose;
        for(UI i = 0; i < BOARD_SIZE; i++)
            for(UI j = 0; j < BOARD_SIZE; j++)
                saved_progress >> board_->board[i][j] >> board_->board_int[i][j] >> board_->shown[i][j];
        saved_progress.close();
    }
    catch(...)
    {
        return false;
    }
    return true;
}

void Sapper::save_progress_to_file()
{
    fstream saved_progress("saved_progress.txt", ios::out);
    saved_progress << board_->get_board_size() << "\n" << BOMBS_AMOUNT << "\n" << SHOW_ZEROES << "\n";
    saved_progress << hidden_fields_amount << "\n" << flags << "\n" << lose << "\n";
    for(UI i = 0; i < board_->get_board_size(); i++)
        for(UI j = 0; j < board_->get_board_size(); j++)
            saved_progress << board_->board[i][j] << " " << board_->board_int[i][j] << " " << board_->shown[i][j] << "\n";
    saved_progress.close();
}

inline bool Board::is_field_a_bomb(UI row, UI col)
{
    return board_int[row][col] == Board::FIELD_BOMB;
}

UI Sapper::count_bombs_around_field(int row, int col)
{
    // counts bombs around XY field
    int bombs = 0;
    int rows[3] = {row-1, row, row+1};
    int cols[3] = {col-1, col, col+1};
    for(int current_row: rows)
    {
        if(board_->is_coord_inside_board(current_row))
            for(int current_col: cols)
                if(board_->is_coord_inside_board(current_col) && board_ -> is_field_a_bomb(current_row, current_col))
                    bombs++;
    }
    return bombs;
}

inline bool Board::is_coord_inside_board(UI coord)
{
    return (coord >= 0 && coord < board_size);
}

void Sapper::update_field(UI row, UI col)
{
    if(board_->shown[row][col] == Board::FIELD_HIDDEN)
        board_->board[row][col] = 'X';
    else if(board_->shown[row][col] == Board::FIELD_MARKED)
        board_->board[row][col] = 'M';
    else if(board_->board_int[row][col] == Board::FIELD_BOMB)
    {
        board_->board[row][col] = 'B';
        lose = true;
    }
    else
        board_->board[row][col] = int_to_char(board_->board_int[row][col]);
}

void Sapper::update_board()
{
    // update all fields of formal board
    for(UI row = 0; row < board_->get_board_size(); row++)
        for(UI col = 0; col < board_->get_board_size(); col++)
            update_field(row, col);
}

void Sapper::mark_field(UI row, UI col)
{
    if(board_->shown[row][col] == Board::FIELD_HIDDEN && flags < BOMBS_AMOUNT)
    {
        board_->shown[row][col] = Board::FIELD_MARKED;
        update_field(row, col);
        flags++;
    }
    else if(board_->shown[row][col] == Board::FIELD_MARKED)
    {
        board_->shown[row][col] = Board::FIELD_HIDDEN;
        update_field(row, col);
        flags--;
    }
}

void Sapper::do_action(string action, UI row, UI col)
{
    if(action == "mark")
        mark_field(row, col);
    else if(action == "show" && board_->shown[row][col] == Board::FIELD_HIDDEN)
    {
        board_->shown[row][col] = Board::FIELD_SHOWN;
        update_field(row, col);
        --hidden_fields_amount;
    }
    else if(action == "save_game")
    {
        save_progress_to_file();
        user_interactor.gameSavedMessage();
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
        exit(0);
    }
    user_interactor.continueGameMessage();
    init_game();
    user_interactor.print_board();
}

void Sapper::specify_settings()
{
    board_ = new Board(user_interactor.specify_board_size());
    user_interactor.board_ = board_;
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
            exit(0);
        }
}

void Sapper::main_game()
{
    string action, row, col;
    while(true)
    {
        val_input input = user_interactor.takeCommand(BOMBS_AMOUNT, flags);
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
    int max_b = board_->get_board_size()*board_->get_board_size();
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
    for(UI i = 0; i < board_->get_board_size(); i++)
    {
        string col = "%i  ";
        col += [](int x)->string {return ((x>8)?(""):(" "));}(i);
        printf(col.c_str(), i+1);
    }
    printf("\n                   ");
    for(UI i = 0; i < board_->get_board_size()*4-1; i++)
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
    for(UI j = 0; j < board_->get_board_size()-1; j++)
        printf("---+");
    printf("---|\n");
}

void UserInteractor::print_row(UI row)
{
    printf(" | ");
    for(UI col = 0; col < board_->get_board_size(); col++)
        cout << board_->board[row][col] << " | ";
    printf("%i\n", row+1);
}

void UserInteractor::print_board_main_body()
{
    for(UI row = 0; row < board_->get_board_size(); row++)
    {
        string spaces = [](UI row_num)->string {return ((row_num>8)?(""):(" "));}(row);
        spaces += "               %i";
        printf(spaces.c_str(), row+1);
        print_row(row);
        spaces = string_of_x_spaces(spaces.length());
        spaces += [](UI row_num)->string {return ((row_num>8)?(" "):(""));}(row);
        printf(spaces.c_str());
        if(row < board_->get_board_size()-1)
            print_line_separating_rows();
    }
}

void UserInteractor::print_board_lower_body()
{
    printf(" ");
    for(UI i = 0; i < board_->get_board_size()*4-1; i++)
        printf("-");
    printf("\n                    ");
    for(UI i = 0; i < board_->get_board_size(); i++)
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

val_input UserInteractor::takeCommand(UI bombs_amount, UI flags)
{
    unverified_input ui;
    cout << translator.loc("Flags left: ") << bombs_amount - flags << "\n";
    cout << translator.loc("Tell me what to do! (action[show/mark/save_game], row[1-9] , col[1-9]):\n>");
    cin>>ui.action>>ui.row>>ui.col;
    return validate_input(ui);
}

val_input UserInteractor::validate_input(unverified_input unverified)
{
    val_input validated;
    int rown = board_->get_board_size() + 1, coln = board_->get_board_size() +1;
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
    validated.row = ((board_->is_coord_inside_board(rown))?(rown):(board_->get_board_size()+1));
    validated.col = ((board_->is_coord_inside_board(coln))?(coln):(board_->get_board_size()+1));
    return validated;
}

void UserInteractor::noSavedProgressErrorMessage()
{
    cout << translator.loc("\nError! No saved progress available. [Press any botton to close game]");
    getch();
}

void UserInteractor::endGameMessage()
{
    cout << translator.loc("Ok, goodbye! :) [press any button]");
    getch();
}

void UserInteractor::continueGameMessage()
{
    cout << translator.loc("Here we go! [press any button]");
    getch();
}

void UserInteractor::playOnceAgainQuestionMessage()
{
    cout<< translator.loc("Would you like to play once again? [yes/no]\n");
}

void UserInteractor::setLanguage()
{
    translator.set_language(ask_about_prefered_language());
}

void UserInteractor::gameSavedMessage()
{
    cout << translator.loc("\n\nSaved. [Press any button]");
    getch();
}

int main()
{
    Sapper saper;
    saper.run();
    return 0;
}
