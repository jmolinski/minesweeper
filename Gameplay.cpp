#include "Gameplay.h"

using namespace std;

Gameplay::Gameplay(UserInteractor* user_interactor, Board* board)
{
    this->user_interactor = user_interactor;
    this->board = board;
}

void Gameplay::run()
{
    user_interactor->print_board(board);
    string action, row, col;
    while(true)
    {
        validated_input input = user_interactor->take_command(board->get_bombs_amount(), board->get_flags_amount());
        input = validate_input(input);
        do_action(input.action, input.row, input.col);
        user_interactor->print_board(board);
        if((board->get_flags_amount() == 0 && board->get_hidden_fields_amount() == 0) || board->stepped_on_bomb())
        {
            user_interactor->game_finished_message(board->stepped_on_bomb());
            user_interactor->press_any_button_message();
            break;
        }
    }
}

void Gameplay::do_action(string action, UI row, UI col)
{
    if(action == "mark")
        board->mark(row, col);
    else if(action == "show")
        board->reveal(row, col);
    else if(action == "save_game")
    {
        GameplaySaver* GS = new GameplaySaver;
        GS->save(board);
        delete GS;
        user_interactor->game_saved_message();
    }
    board->update(row, col);
}

validated_input Gameplay::validate_input(validated_input input)
{
    if(!board->is_coord_inside_board(input.row, input.col) ||
            (input.action != "show" && input.action != "mark" && input.action != "save_game"))
        return validated_input(string("invalid"), 0, 0);
    return input;
}
