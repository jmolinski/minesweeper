#include <ctime>
#include "Sapper.h"

using namespace std;

Sapper::Sapper()
{
    this->gameplay_saver = new GameplaySaver;
    this->user_interactor = new UserInteractor;
}

Sapper::~Sapper()
{
    delete gameplay_saver;
    delete user_interactor;
}

void Sapper::run()
{
    while(true)
    {
        user_interactor->print_menu();
        UI user_selection = user_interactor->menu_selection_question();

        switch(user_selection)
        {
        case 1:
            specify_settings();
            init_game();
            run_gameplay();
            break;
        case 2:
            load_game();
            run_gameplay();
            break;
        case 3:
            user_interactor->specify_UI_language();
            break;
        case 4:
            user_interactor->about_game();
            break;
        case 5:
            return;
        }
    }
}

void Sapper::load_game()
{
    if(gameplay_saver->load(board) == false)
    {
        user_interactor->no_saved_progress_error_message();
        specify_settings();
        init_game();
    }
}

void Sapper::run_gameplay()
{
    Gameplay game(user_interactor, board);
    game.run();
    delete board;
}

void Sapper::specify_settings()
{
    UI board_size = user_interactor->specify_board_size();
    UI bombs_amount = user_interactor->specify_bombs_amount(board_size);
    bool show_zeros = user_interactor->specify_zeros_shown();

    board = new Board(board_size, bombs_amount, show_zeros);
}

void Sapper::init_game()
{
    srand(time(NULL));
    board->clear_boards();
    board->set_bombs_on_board();
    board->set_fields_values();
    board->update_board();
}
