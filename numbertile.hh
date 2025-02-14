/*
#############################################################################
 * COMP.CS.110 Ohjelmointi 2: Rakenteet / Programming 2: Structures
 * Project4: Peli nimeltä 2048 / Game called 2048
 * File: numbers_gui
 *
 * This is the header file for the class NumberTile that implements the
 * functionalities of a single numbertile.
 *
 * Functionalities of public methods are explained along with their
 * definitions in this file.
 *
 *
 * Program author:
 *
 * Name: Ville Nousiainen
 * Student number: 150265575
 * UserID: kcvino
 * E-mail: ville.j.nousiainen@tuni.fi
 *
#############################################################################
*/

#ifndef NUMBERTILE_HH
#define NUMBERTILE_HH

#include <vector>

using Coords = std::pair<int, int>;

// Forward declaration, to avoid circular include's
class GameBoard;

class NumberTile
{
public:
    // Constructor
    NumberTile(int value, Coords coords, GameBoard* board);

    // Destructor
    ~NumberTile();

    // Prints the number tile.
    void print(int width);

    // Moves the number tile in the given direction and merges it, if possible.
    // Returns true, if the player won the game, i.e. if the goal value was
    // reached, otherwise returns false.
    bool move(Coords direction, int goal_val);

    // Sets a new value for an empty number tile.
    // Returns true, if a new value was set, otherwise returns false.
    bool new_value(int new_val);

    // Returns true, if the number tile is empty, i.e. if it has the value 0.
    bool is_empty();

    // Sets the value of is_merged_ as false.
    void reset_turn();

    // getter and setter for the value of the numbertile
    int get_value();
    void set_value(int new_value);

    // Getter for the coordinates of the number tile
    Coords get_coords();

private:
    // Value in the number tile
    int value_;

    // Coordinates of the number tile
    Coords coords_;

    // Pointer to the gameboard, where the number tile lies
    GameBoard* board_;

    // Tells if the number is merged or not,
    // can be true only temporarily during moves
    bool is_merged_;

    // Returns true, if the given coordinates are on the gameboard,
    // otherwise returns false.
    bool is_on_board(Coords coords);
};

#endif // NUMBERTILE_HH
