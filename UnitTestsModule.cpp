#include "Board.h"
#include <iostream>

#define BOOST_TEST_MODULE tests
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE( BOARD )

BOOST_AUTO_TEST_CASE( clearBoards )
{
    Board board(4, 8, true);
    bool clear = true;
    board.clear_boards();

    for(UI row = 0; row < board.get_board_size(); row++)
        for(UI col = 0; col < board.get_board_size(); col++)
            if(board.get_value(row, col) != 'X')
                clear = false;

    BOOST_CHECK( clear == true );
}

BOOST_AUTO_TEST_CASE( getHiddenFieldsAmount )
{
    Board board(4, 8, true);
    board.clear_boards();
    BOOST_CHECK( board.get_hidden_fields_amount() == 16 );
}

BOOST_AUTO_TEST_CASE ( isCoordInsideBoard )
{
    Board board(4, 8, true);
    BOOST_CHECK(board.is_coord_inside_board(-1, -1) == false);
    BOOST_CHECK(board.is_coord_inside_board(-1, 1) == false);
    BOOST_CHECK(board.is_coord_inside_board(1, -1) == false);
    BOOST_CHECK(board.is_coord_inside_board(1, 1) == true);
    BOOST_CHECK(board.is_coord_inside_board(3, 3) == true);
    BOOST_CHECK(board.is_coord_inside_board(3, 4) == false);
    BOOST_CHECK(board.is_coord_inside_board(4, 3) == false);
    BOOST_CHECK(board.is_coord_inside_board(4, 4) == false);
}

BOOST_AUTO_TEST_SUITE_END() // SUITE BOARD

BOOST_AUTO_TEST_SUITE( BOARD_FIELD )

BOOST_AUTO_TEST_CASE( clearField )
{
    BoardField bf;
    bf.clear();

    BOOST_CHECK(bf.is_hidden());
    BOOST_CHECK(bf.get_value() == 'X');
    BOOST_CHECK(bf.is_bomb() == false);
}

BOOST_AUTO_TEST_SUITE_END() // SUITE BOARD_FIELD
