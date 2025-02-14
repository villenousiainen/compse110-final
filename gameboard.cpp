/*
#############################################################################
 * COMP.CS.110 Ohjelmointi 2: Rakenteet / Programming 2: Structures
 * Project4: Peli nimeltä 2048 / Game called 2048
 * File: numbers_gui
 *
 * This is the implementation file for the class GameBoard

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
#include "gameboard.hh"
#include <iostream>

GameBoard::GameBoard()
{
    set_grid_size(4);
}

GameBoard::~GameBoard()
{

    for(unsigned int y = 0; y < board_.size(); ++y)
    {
        for(unsigned int x = 0; x < board_.at(y).size(); ++x)
        {
            delete board_.at(x).at(y);
            board_.at(x).at(y) = nullptr;
        }
    }
}

void GameBoard::init_empty()
{
    std::vector<NumberTile*> row;
    for( int i = 0; i < grid_size_; ++i)
    {
        row.push_back(nullptr);
    }
    for( int i = 0; i < grid_size_; ++i)
    {
        board_.push_back(row);
    }
}

void GameBoard::fill(int seed)
{
    randomEng_.seed(seed);
    distribution_ = std::uniform_int_distribution<int>(0, grid_size_ - 1);

    // Wiping out the first random number (which is almost almost 0)
    distribution_(randomEng_);

    for( auto y = 0; y < grid_size_; ++y )
    {
        for( auto x = 0; x < grid_size_; ++x )
        {
            board_.at(y).at(x) = new NumberTile(0, std::make_pair(y, x), this);
        }
    }

    for( int i = 0 ; i < grid_size_ ; ++i )
    {
        new_value();
    }
}

void GameBoard::reset(int seed, bool change_grid_size)
{
    randomEng_.seed(seed);
    distribution_ = std::uniform_int_distribution<int>(0, grid_size_ - 1);
    distribution_(randomEng_);

    if (not change_grid_size)
    {
        // set all values to 0.
        for( auto y = 0; y < grid_size_; ++y )
        {
            for( auto x = 0; x < grid_size_; ++x )
            {
             board_.at(y).at(x)->set_value(0);
            }
        }
        // Initialising starting numbers.
        for( int i = 0 ; i < grid_size_ ; ++i )
        {
            new_value();
        }     
    } else // If grid size is changed
    {
        int current_board_size = int(board_.size());
        // New grid size is smaller than old one
        if (grid_size_ < current_board_size )
        {
            int amount_to_remove = current_board_size  - grid_size_;
            // Removes enough y-coordinates
            for(auto y = 0; y < amount_to_remove; ++y)
            {
                int index = current_board_size - 1 - y;
                for(NumberTile* tile : board_.at(index))
                {
                    delete tile;
                }
            }
            for(auto x = 0; x < amount_to_remove; ++x)
            {
                board_.pop_back();
            }
            // Removes enough x-coordinates
            for (auto x = 0; x < grid_size_; ++x)
            {
                for(auto y = 0; y < amount_to_remove; ++y)
                {
                    delete board_.at(x).at(current_board_size - 1 - y);
                    board_.at(x).pop_back();
                }
            }
        } else // New grid size is bigger than old one
        {
            int amount_to_increase = grid_size_ - current_board_size;
            // Increases enough x-coordinates
            for(int x = 0; x < amount_to_increase; ++x)
            {
                for(int y = 0; y < current_board_size; ++y)
                {
                    int x_coord = current_board_size + x;
                    board_.at(y).push_back(new NumberTile(0, std::make_pair(y, x_coord), this));
                }
            }
            // Increases enough y-coordinates
            int y_coord;
            for (int i = 0  ; i < amount_to_increase;++i)
            {
                y_coord = current_board_size + i ;
                std::vector<NumberTile*> row = {};
                for( int x = 0; x < grid_size_; ++x)
                {
                    row.push_back(new NumberTile(0, std::make_pair(y_coord, x), this));
                }
                    board_.push_back(row);
                }
            // Initializes newly added numbertiles with starting values.
            reset(seed);
        }
    }
}

void GameBoard::new_value(bool check_if_empty)
{
    if( check_if_empty and is_full() ){
        // So that we will not be stuck in a forever loop
        return;
    }
    int random_x = 0;
    int random_y = 0;
    do
    {
        random_x = distribution_(randomEng_);
        random_y = distribution_(randomEng_);
    } while( not board_.at(random_y).at(random_x)->new_value(NEW_VALUE) );
}

void GameBoard::print() const
{
    for( auto y : board_ )
    {
        std::cout << std::string(PRINT_WIDTH * grid_size_ + 1, '-') << std::endl;
        for( auto x : y )
        {
            x->print(PRINT_WIDTH);
        }
        std::cout << "|" << std::endl;
    }
    std::cout << std::string(PRINT_WIDTH * grid_size_ + 1, '-') << std::endl;
}

void GameBoard::set_grid_size(int new_size)
{
    grid_size_ = new_size;
}

int GameBoard::get_grid_size()
{
    return grid_size_;
}

void GameBoard::remove_random_tile()
{
    // Temporary vector to hold all possible removable values
    std::vector<int> temp_vector = {};

    for(unsigned int y = 0; y < board_.size(); ++y)
    {
        for(unsigned int x = 0; x < board_.at(y).size(); ++x)
        {
            NumberTile* temp_tile = get_item({y, x});
            int value = temp_tile->get_value();
            if(value != 0)
            {
                temp_vector.push_back(value);
            }
        }
    }

    srand(time(NULL));
    int max_index = int(temp_vector.size()) - 1;
    // Random index in temp_vector
    int i = rand()%(max_index+1);
    int value_to_remove = temp_vector.at(i);

    // Searching all the coordinates with the desired value to remove
    std::vector<Coords> temp_coords = {};
    for(unsigned int y = 0; y < board_.size(); ++y)
        {
            for(unsigned int x = 0; x < board_.at(y).size(); ++x)
            {
                NumberTile* temp_tile = get_item({y, x});
                int value = temp_tile->get_value();
                if(value == value_to_remove)
                {
                    temp_coords.push_back(temp_tile->get_coords());
                }
            }
        }
    int max_index2 = int(temp_coords.size() - 1);
    // random index in temp_coords
    int i2 = rand()%(max_index2+1);
    Coords coords_to_remove = temp_coords.at(i2);

    // set randomly selected tiles value to 0.
    board_.at(coords_to_remove.first).at(coords_to_remove.second)->set_value(0);
}


bool GameBoard::move(Coords dir, int goal)
{
    bool has_won = false;
    for( unsigned y = 0; y < board_.size(); ++y )
    {
        for( unsigned x = 0; x < board_.front().size(); ++x )
        {
            int directed_y = dir.first > 0 ? board_.size() - y - 1 : y;
            int directed_x = dir.second > 0 ? board_.back().size() - x - 1 : x;
            if( board_.at(directed_y).at(directed_x)->move(dir, goal) )
            {
                has_won = true;
            }
        }
    }
    for( auto &row : board_ )
    {
        for( auto &tile : row )
        {
            tile->reset_turn();
        }
    }
    return has_won;
}

NumberTile* GameBoard::get_item(Coords coords)
{
    return board_.at(coords.first).at(coords.second);
}


bool GameBoard::is_full() const
{
    for( auto y : board_ )
    {
        for( auto x : y )
        {
            if( x->is_empty() )
            {
                return false;
            }
        }
    }
    return true;
}
