#ifndef USERINTERACTOR_H
#define USERINTERACTOR_H

class UserInteractor
{
private:
    Translator translator;
    std::string string_of_x_spaces(UI x);
    void print_board_main_body();
    void print_row(UI row);
    void print_line_separating_rows();
    void print_board_upper_body();
    void print_board_lower_body();
public:
    Board* board_;
    UI specify_board_size();
    UI specify_bombs_amount();
    bool specify_zeroes_shown();
    std::string ask_about_prefered_language();
    void print_board();
    val_input takeCommand(UI, UI);
    val_input validate_input(unverified_input unverified);
    void setLanguage();
    void gameSavedMessage();
    std::string continue_or_end_gameAnswer();
    void endGameMessage();
    void continueGameMessage();
    void gameFinishedMessages(bool lose);
    void playOnceAgainQuestionMessage();
    void noSavedProgressErrorMessage();
    std::string selectGameModeQuestion();
};

#endif // USERINTERACTOR_H
