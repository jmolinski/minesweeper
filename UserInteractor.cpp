#include <iostream>
#include "UserInteractor.h"

using namespace std;

void UserInteractor::no_saved_progress_error_message()
{
    cout << "\n" << translator.loc("\nError! No saved progress available. [Press any botton to continue]") << "\n\n";
    cin.sync();
    cin.get();
}

void UserInteractor::game_saved_message()
{
    cout << translator.loc("\n\nSaved. [Press any button]");
    cin.sync();
    cin.get();
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
        cout<<"\n"<<"Select language:\n1.EN [English]\n2.PL [Polski]\n>";
        cin>>selected;
    }
    if(selected == "1")
        language = "EN";
    else if(selected == "2")
        language = "PL";
    this->translator.set_language(language);
}

UserInteractor::UserInteractor()
{
    this->translator.set_language("EN");
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

void UserInteractor::print_board(Board* board)
{
    system("CLS");
    this->board_printer.print(board);
}

validated_input UserInteractor::take_command(UI bombs_amount, UI flags)
{
    string action, row, col;
    cout << translator.loc("Flags left: ") << flags << "\n";
    cout << translator.loc("Tell me what to do! (action[show/mark/save_game], row, col):\n>");
    cin>>action>>row>>col;
    return validated_input(translator.to_en(action), row, col);
}

validated_input::validated_input(std::string action, std::string row, std::string col)
{
    this->action = action;
    try
    {
        this->row = stoi(row) - 1;
        this->col = stoi(col) - 1;
    }
    catch(...)
    {
        this->action = "invalid";
        this->row = 0;
        this->col = 0;
    }
}

validated_input::validated_input(std::string action, int row, int col)
{
    this->action = action;
    this->row = row;
    this->col = col;
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

void UserInteractor::print_menu()
{
    system("cls");
    cout << "                Sapper\n\n";
    cout << "1. " << translator.loc("Start new game") << "\n";
    cout << "2. " << translator.loc("Continue game") << "\n";
    cout << "3. " << translator.loc("Change language") << "\n";
    cout << "4. " << translator.loc("About game") << "\n";
    cout << "5. " << translator.loc("Quit game") << "\n";
}

UI UserInteractor::menu_selection_question()
{
    char chose = ' ';
    while(chose > '5' || chose < '1')
    {
        cout << "\n" << translator.loc("Chose one option:\n>");
        cin>>chose;
    }
    // char to int
    return chose-48;
}

void UserInteractor::about_game()
{
    cout << "about game\n";
    cin.sync();
    cin.get();
}

void UserInteractor::press_any_button_message()
{
    cout << translator.loc("[Press any button]");
    cin.sync();
    cin.get();
}
