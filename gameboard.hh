/*
#############################################################################
 * COMP.CS.110 Ohjelmointi 2: Rakenteet / Programming 2: Structures
 * Project4: Peli nimeltä 2048 / Game called 2048
 * File: numbers_gui
 *
 * This is the header file for the class GameBoard that implements the internal
 * structure of the grid. Holds pointers to NumberTiles in a nested vector
 * and a random generator for initializing the starting figures.
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

#ifndef GAMEBOARD_HH
#define GAMEBOARD_HH

#include "numbertile.hh"
#include <vector>
#include <random>
#include <set>


const int PRINT_WIDTH = 5;
const int NEW_VALUE = 2;
const int DEFAULT_GOAL = 2048;

class GameBoard
{
public:
    // Constructor
    GameBoard();

    // Destructor
    ~GameBoard();

    // Initializes the gameboard with nullptrs.
    void init_empty();

    // Initializes the random number generator and fills the gameboard
    // with random numbers.
    void fill(int seed);

    // Resets the already-existing NumberTiles to default figures
    // with the random generator and parameter 'seed'.
    // Parameter change_grid_size determines if the grid changes
    // in size during reset, and adds/deletes required NumberTiles
    // to/from data structure.
    void reset(int seed, bool change_grid_size=false);

    // Draws a new location (coordinates) from the random number generator and
    // puts the NEW_VALUE on that location, unless check_if_empty is true and
    // the gameboard is full.
    void new_value(bool check_if_empty = true);

    // Returns true, if all the tiles in the game board are occupied,
    // otherwise returns false.
    bool is_full() const;

    // Prints the game board.
    void print() const;

    // setter and getter for grid side size entered by user.
    void set_grid_size(int new_size);
    int get_grid_size();

    // Sets the value of a random tile in board_ to 0.
    void remove_random_tile();

    // Moves the number tiles in the gameboard, if possible (by calling
    // move method for each number tile).
    // Finally, resets turn of all number tiles.
    bool move(Coords dir, int goal);

    // Returns the element (number tile) in the given coordinates.
    NumberTile* get_item(Coords coords);

    // setter for new seed value entered by user.
    void set_seed_value(int seed);

private:
    // Internal structure of the game board
    std::vector<std::vector<NumberTile*>> board_;

    // Size of the grid is 4x4 by default.
    int grid_size_ = 4;

    // Random number generator and distribution,
    // they work better, if they are attributes of a class.
    std::default_random_engine randomEng_;
    std::uniform_int_distribution<int> distribution_;
};

#endif // GAMEBOARD_HH
