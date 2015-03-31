using namespace std;

typedef unsigned int UI;

const UI BOARD_SIZE = 9; // board is a grid of x * x fields
const UI BOMBS = 18; // number of bombs
const bool SHOW_ZEROES = true; // show all zero-valued fields
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
string board[BOARD_SIZE][BOARD_SIZE]; // board that will be displayed to user
UI board_int[BOARD_SIZE][BOARD_SIZE]; // board with actual fields' values
UI shown[BOARD_SIZE][BOARD_SIZE]; // board with values: marked, hidden, shown for each field

// some global variables... :(
UI flags = 0;
bool lose = false;
UI hidden_fields = BOARD_SIZE * BOARD_SIZE;

// function index
void start_game();
void update_board();
void update_field(UI row, UI col);
UI count_bombs_around_field(int row, int col);
void print_board();
void print_board_main_body();
void print_board_upper_body();
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
string to_string(int);
int to_string(string);

// convert int to string
template <class T>
inline std::string to_string(const T& t)
{
    std::stringstream ss;
    ss << t;
    return ss.str();
}

template <class T>
inline int to_int(const T& str)
{
    int i;
    istringstream iss(str);
    iss >> i;
    return i;
}
