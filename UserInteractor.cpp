#include <iostream>
#include "UserInteractor.h"

using namespace std;

void UserInteractor::no_saved_progress_error_message()
{
    cout << "\n" << translator.loc("\nError! No saved progress available. [Press any botton to continue]") << "\n\n";
    cin.sync();
    cin.get();
}

void UserInteractor::end_game_message()
{
    cout << translator.loc("Ok, goodbye! :) [press any button]");
    cin.sync();
    cin.get();
}

void UserInteractor::continue_game_message()
{
    cout << translator.loc("Here we go! [press any button]") << "\n";
    cin.sync();
    cin.get();
}

void UserInteractor::play_once_again_message()
{
    cout<< translator.loc("Would you like to play once again? [yes/no]\n");
}

void UserInteractor::game_saved_message()
{
    cout << translator.loc("\n\nSaved. [Press any button]");
    cin.sync();
    cin.get();
}

bool UserInteractor::continue_or_end_game_question()
{
    string answer;
    while(answer != "yes" && answer != "no")
    {
        cout<<">";
        cin>>answer;
        answer = translator.to_en(answer);
    }
    return answer == "yes";
}

void UserInteractor::game_finished_message(bool lose)
{
    if(lose)
        cout << translator.loc("\n\nWhat a pity! You stepped on a bomb!\n");
    else
        cout << translator.loc("\n\nCongratulations! You marked all bombs correctly and saved many human beings!\n");
}

void UserInteractor::specify_UI_language()
{
    string selected, language;
    while(selected != "1" && selected != "2")
    {
        cout<<"Select language:\n1.EN [English]\n2.PL [Polski]\n>";
        cin>>selected;
    }
    if(selected == "1")
        language = "EN";
    else if(selected == "2")
        language = "PL";
    this->translator.set_language(language);
}

UI UserInteractor::specify_board_size()
{
    int input_int = 0;
    while(true)
    {
        cout << translator.loc("\nSpecify the size of the board (1 to 99)\n>");
        string input;
        cin>>input;
        if(to_int(input_int, input) && input_int <= 99 && input_int >= 1)
            break;
        cout << translator.loc("\nInvalid value!");
    }
    return static_cast<UI>(input_int);
}

UI UserInteractor::specify_bombs_amount(const UI board_size)
{
    int input_int = 0;
    int max_bombs_limit = board_size*board_size;
    while(true)
    {
        cout << translator.loc("\nSpecify how many bombs will be set on the board (1 to the square od board size)\n>");
        string input;
        cin >> input;
        if(to_int(input_int, input) && input_int <= max_bombs_limit && input_int >= 1)
            break;
        cout << translator.loc("\nInvalid value!");
    }
    return static_cast<UI>(input_int);
}

bool UserInteractor::specify_zeros_shown()
{
    string input;
    while(input != "true" && input != "false")
    {
        cout << translator.loc("\nSpecify if the zeros will be shown since the game begin: [true/false]\n>");
        cin >> input;
        input = translator.to_en(input);
        if(input != "true" && input != "false")
            cout << translator.loc("\nInvalid value!");
    }
    return input == "true";
}

string UserInteractor::select_game_mode_question()
{
    string game_mode = "";
    while(game_mode != "1" && game_mode != "2")
    {
        cout << translator.loc("\nSelect game mode:\n1.Start new game\n2.Continue saved game\n>");
        cin >> game_mode;
    }
    return (game_mode == "1" ? "new_game" : "saved_progress");
}

void UserInteractor::print_board()
{
    system("CLS");
    this->board_printer.print(this->board);
}

validated_input UserInteractor::take_command(UI bombs_amount, UI flags)
{
    unverified_input ui;
    cout << translator.loc("Flags left: ") << flags << "\n";
    cout << translator.loc("Tell me what to do! (action[show/mark/save_game], row, col):\n>");
    cin>>ui.action>>ui.row>>ui.col;
    return validate_input(ui);
}

validated_input UserInteractor::validate_input(unverified_input unverified)
{
    validated_input validated;
    int rown = board->get_board_size() + 1;
    int coln = board->get_board_size() +1;
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
    validated.row = ((board->is_coord_inside_board(rown))?(rown):(board->get_board_size()+1));
    validated.col = ((board->is_coord_inside_board(coln))?(coln):(board->get_board_size()+1));
    return validated;
}

bool UserInteractor::to_int(int& val, string str_val)
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

void UserInteractor::set_board(Board* board)
{
    this->board = board;
}
