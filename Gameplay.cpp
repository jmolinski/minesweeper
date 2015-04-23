#include "Gameplay.h"
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <iostream>

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

    this->user_interface->set_language();
    start_new_game_or_continue_saved_proggress();
    this->user_interface->board_ = board;
    sapper->set_board(board);
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
        if(continue_saved_game_from_file() == false)
        {
            user_interface->no_saved_progress_error_message();
            specify_settings();
            init_game();
        }
}

void Gameplay::main_game()
{
    string action, row, col;
    while(true)
    {
        val_input input = user_interface->take_command(bombs_amount, flags);
        do_action(input.action, input.row, input.col);
        user_interface->print_board();
        if((flags == bombs_amount && hidden_fields_amount == bombs_amount) || lose)
        {
            user_interface->game_finished_messages(lose);
            if(continue_or_end() == false)
                break;
        }
    }
}

bool Gameplay::continue_or_end()
{
    user_interface->play_once_again_question_message();
    string answer = user_interface->continue_or_end_game_answer();
    if(answer == "no")
    {
        user_interface->end_game_message();
        return false;
    }
    user_interface->continue_game_message();
    delete board;
    specify_settings();
    init_game();
    sapper->set_board(board);
    user_interface->board_ = board;
    user_interface->print_board();
    return true;
}

void Gameplay::specify_settings()
{
    board = new Board(user_interface->specify_board_size());
    bombs_amount = user_interface->specify_bombs_amount();
    show_zeros = user_interface->specify_zeroes_shown();
}

void Gameplay::init_game()
{
    srand(time(NULL));
    // set each field to its default value
    board -> clear_boards();
    sapper -> set_bombs_on_board(bombs_amount);
    // set values of all non-bomb fields
    hidden_fields_amount = board->get_board_size() * board->get_board_size();
    sapper->set_fields_values(show_zeros, hidden_fields_amount);
    lose = false;
    flags = 0;
    sapper->update_board(lose);
}

void Gameplay::do_action(string action, UI row, UI col)
{
    if(action == "mark")
        sapper->mark_field(row, col, flags, bombs_amount, lose);
    else if(action == "show" && board->shown[row][col] == Board::FIELD_HIDDEN)
    {
        board->shown[row][col] = Board::FIELD_SHOWN;
        --hidden_fields_amount;
    }
    else if(action == "save_game")
    {
        save_progress_to_file();
        user_interface->game_saved_message();
    }
    sapper->update_field(row, col, lose);
}

bool Gameplay::continue_saved_game_from_file()
{
    UI board_size;
    fstream saved_progress;
    saved_progress.open("saved_progress.txt", ios::in);
    if(saved_progress.good() == false)
        return false;
    saved_progress >> board_size;
    board = new Board(board_size);
    saved_progress >> bombs_amount >> show_zeros >> hidden_fields_amount >> flags >> lose;
    for(UI i = 0; i < board_size; i++)
        for(UI j = 0; j < board_size; j++)
            saved_progress >> board->board[i][j] >> board->board_int[i][j] >> board->shown[i][j];
    saved_progress.close();
    return true;
}

void Gameplay::save_progress_to_file()
{
    fstream saved_progress("saved_progress.txt", ios::out);
    saved_progress << board->get_board_size() << "\n" << bombs_amount << "\n" << show_zeros << "\n";
    saved_progress << hidden_fields_amount << "\n" << flags << "\n" << lose << "\n";
    for(UI i = 0; i < board->get_board_size(); i++)
        for(UI j = 0; j < board->get_board_size(); j++)
            saved_progress << board->board[i][j] << " " << board->board_int[i][j] << " " << board->shown[i][j] << "\n";
    saved_progress.close();
}
