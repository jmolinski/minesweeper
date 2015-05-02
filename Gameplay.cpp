#include "Gameplay.h"
#include <ctime>
#include <cstdlib>
#include <fstream>

using namespace std;

Gameplay::~Gameplay()
{

}

Gameplay::Gameplay(UserInteractor* user_interactor, Board* board)
{
    this->user_interface = user_interactor;
    this->board = board;
}

void Gameplay::run()
{
    user_interface->print_board();
    main_game();
}

void Gameplay::main_game()
{
    string action, row, col;
    while(true)
    {
        validated_input input = user_interface->take_command(board->get_bombs_amount(), board->get_flags_amount());
        do_action(input.action, input.row, input.col);
        user_interface->print_board();
        if((board->get_flags_amount() == 0 && board->get_hidden_fields_amount() == 0) || board->stepped_on_bomb())
        {
            user_interface->game_finished_message(board->stepped_on_bomb());
            break;
        }
    }
}

void Gameplay::do_action(string action, UI row, UI col)
{
    if(action == "mark")
        board->board[row][col].mark(board->get_flags_amount());
    else if(action == "show")
        board->board[row][col].reveal();
    else if(action == "save_game")
    {
        GameplaySaver* GS = new GameplaySaver;
        GS->save(board);
        delete GS;
        user_interface->game_saved_message();
    }
    board->board[row][col].update();
}
