#include <ctime>
#include "Sapper.h"

using namespace std;

Sapper::Sapper()
{
    this->user_interactor = new UserInteractor;
    this->user_interactor->specify_UI_language();
}

Sapper::~Sapper()
{
    delete user_interactor;
}

void Sapper::run()
{
    start_new_game_or_continue_saved_proggress();
    while(true)
    {
        Gameplay game(user_interactor, board);
        game.run();
        delete board;
        if(continue_or_end() == false)
            break;
        else
        {
            specify_settings();
            init_game();
        }
    }
}

void Sapper::start_new_game_or_continue_saved_proggress()
{
    string game_mode = user_interactor->select_game_mode_question();
    if(game_mode == "new_game")
    {
        specify_settings();
        init_game();
    }
    if(game_mode == "saved_progress")
    {
        GameplaySaver* GS = new GameplaySaver;
        if(GS->load(board) == false)
        {
            user_interactor->no_saved_progress_error_message();
            specify_settings();
            init_game();
        }
        delete GS;
    }
}

void Sapper::specify_settings()
{
    UI board_size = user_interactor->specify_board_size();
    UI bombs_amount = user_interactor->specify_bombs_amount(board_size);
    bool show_zeros = user_interactor->specify_zeros_shown();

    board = new Board(board_size, bombs_amount, show_zeros);
    user_interactor->set_board(board);
}

void Sapper::init_game()
{
    srand(time(NULL));
    board->clear_boards();
    board->set_bombs_on_board();
    board->set_fields_values();
    board->update_board();
}

bool Sapper::continue_or_end()
{
    user_interactor->play_once_again_message();
    bool answer = user_interactor->continue_or_end_game_question();
    if(!answer)
    {
        user_interactor->end_game_message();
        return false;
    }
    user_interactor->continue_game_message();
    return true;
}
