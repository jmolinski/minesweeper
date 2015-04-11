using namespace std;

typedef unsigned int UI;

// settings
UI BOARD_SIZE = 9; // board is a grid of x * x fields
UI BOMBS_AMOUNT = 18; // number of bombs set on the board
bool SHOW_ZEROES = true; // show all zero-valued fields
// value will be set before geme starts
string language;
// special values of fields
const UI FIELD_CLEAR = 0;
const UI FIELD_BOMB = 9;
const UI FIELD_MARKED = 10;
const UI FIELD_SHOWN = 11;
const UI FIELD_HIDDEN = 12;

struct validated_input
{
    string action;
    int row;
    int col;
};

typedef validated_input val_input;

// boards
string board[100][100]; // board that will be displayed to user
UI board_int[100][100]; // board with actual fields' values
UI shown[100][100]; // board with values: marked, hidden, shown for each field

// some global variables... :(
UI flags = 0;
bool lose = false;
UI hidden_fields_amount = BOARD_SIZE * BOARD_SIZE;

// function index
void start_game();
void update_board();
void update_field(UI row, UI col);
UI count_bombs_around_field(int row, int col);
void print_board();
void print_board_main_body();
void print_row(UI row);
void print_line_separating_rows();
void print_board_upper_body();
void print_board_lower_body();
void init_game();
void clear_boards();
void clear_field(UI row, UI col);
void set_bombs_on_board();
void set_fields_values();
void set_field_value(UI row, UI col);
void do_action(string action, UI row, UI col);
void mark_field(UI row, UI col);
void finish_game(bool);
void continue_or_end();
bool is_coord_inside_board(UI coord);
val_input validate_input(string act, string row, string col);
string string_of_x_spaces(UI x);
void print_line_separating_rows();
string ask_about_prefered_language();
bool load_game_from_file();
void start_new_game_or_continue_saved_proggress();
void specify_settings();
void specify_board_size();
void specify_bombs_amount();
void specify_zeroes_shown();
void main_game();
