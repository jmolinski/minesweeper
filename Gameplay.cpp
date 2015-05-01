#include "Gameplay.h"
#include <ctime>
#include <cstdlib>
#include <fstream>

using namespace std;

Gameplay::~Gameplay()
{
    delete sapper;
    delete user_interface;
}

void Gameplay::run()
{
    setup();
    user_interface->print_board();
    main_game();
}

void Gameplay::setup()
{
    this->sapper = new Sapper;
    this->user_interface = new UserInteractor;

    this->user_interface->specify_UI_language();
    start_new_game_or_continue_saved_proggress();
    this->user_interface->board = board;
    this->sapper->set_board(board);
}

void Gameplay::start_new_game_or_continue_saved_proggress()
{
    string game_mode = user_interface->select_game_mode_question();
    if(game_mode == "new_game")
    {
        specify_settings();
        init_game();
    }
    if(game_mode == "saved_progress")
    {
        GameplaySaver* GS = new GameplaySaver;
        if(GS->load(board, show_zeros, flags, lose, hidden_fields_amount) == false)
        {
            user_interface->no_saved_progress_error_message();
            specify_settings();
            init_game();
        }
        delete GS;
    }
}

void Gameplay::main_game()
{
    string action, row, col;
    while(true)
    {
        validated_input input = user_interface->take_command(board->bombs_amount, flags);
        do_action(input.action, input.row, input.col);
        user_interface->print_board();
        if((flags == board->bombs_amount && hidden_fields_amount == board->bombs_amount) || lose)
        {
            user_interface->game_finished_message(lose);
            if(continue_or_end() == false)
                break;
        }
    }
}

bool Gameplay::continue_or_end()
{
    user_interface->play_once_again_message();
    string answer = user_interface->continue_or_end_game_question();
    if(answer == "no")
    {
        user_interface->end_game_message();
        return false;
    }
    user_interface->continue_game_message();
    delete board;
    specify_settings();
    init_game();
    user_interface->print_board();
    return true;
}

void Gameplay::specify_settings()
{
    board = new Board(user_interface->specify_board_size());
    user_interface->board = board;
    sapper->set_board(board);
    board->bombs_amount = user_interface->specify_bombs_amount();
    show_zeros = user_interface->specify_zeros_shown();
}

void Gameplay::init_game()
{
    srand(time(NULL));
    // set each field to its default value
    board -> clear_boards();
    sapper -> set_bombs_on_board(board->bombs_amount);
    // set values of all non-bomb fields
    hidden_fields_amount = board->get_board_size() * board->get_board_size();
    sapper->set_fields_values(show_zeros, hidden_fields_amount);
    lose = false;
    flags = 0;
    board->update_board(lose);
}

void Gameplay::do_action(string action, UI row, UI col)
{
    if(action == "mark")
        board->mark_field(row, col, flags, board->bombs_amount, lose);
    else if(action == "show" && board->board[row][col].shown == Board::FIELD_HIDDEN)
    {
        board->board[row][col].shown = Board::FIELD_SHOWN;
        --hidden_fields_amount;
    }
    else if(action == "save_game")
    {
        GameplaySaver* GS = new GameplaySaver;
        GS->save(board, show_zeros, flags, lose, hidden_fields_amount);
        delete GS;
        user_interface->game_saved_message();
    }
    board->update_field(row, col, lose);
}
