#include <iostream>
#include <time.h>
#include <fstream>
#include <cstdlib>
#include <cstdio>

#include "Sapper.h"

using namespace std;

Sapper::Sapper()
{
    user_interactor.setLanguage();
    start_new_game_or_continue_saved_proggress();
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

void Sapper::specify_settings()
{
    board_ = new Board(user_interactor.specify_board_size());
    user_interactor.board_ = board_;
    BOMBS_AMOUNT = user_interactor.specify_bombs_amount();
    SHOW_ZEROES = user_interactor.specify_zeroes_shown();
}

void Sapper::init_game()
{
    srand(time(NULL));
    // set each field to its default value
    board_ -> clear_boards();
    set_bombs_on_board();
    // set values of all non-bomb fields
    hidden_fields_amount = board_->get_board_size() * board_->get_board_size();
    set_fields_values();
    lose = false;
    flags = 0;
    update_board();
}

void Sapper::run()
{
    update_board();
    user_interactor.print_board();
    main_game();
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

inline char Sapper::int_to_char(int x)
{
    return (x+48);
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
