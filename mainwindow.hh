/*
#############################################################################
 * COMP.CS.110 Ohjelmointi 2: Rakenteet / Programming 2: Structures
 * Project4: Peli nimeltä 2048 / Game called 2048
 * File: numbers_gui
 * Description:
 *  This program implements a user interface for the game 2048.
 *
 *  The program reads several forms of input from the user from spinboxes,
 *  including seed for random initialisation, goal value, grid size and
 *  random tile removal interval.
 *
 *  User can start the game by pressing the start button, and reset it to
 *  the beginning with the reset button. The program also implements a timer
 *  with accuracy to the hundredth of a second. The timer is paused in one of
 *  the following circumstances 1) player wins 2) player loses 3) player
 *  presses the pause button.
 *
 *  User can pause the game with the pause button. At this point, the game
 *  does not respond to key strokes, timer has been paused, spinboxes and
 *  start button has been disabled. Also a continue button appears during
 *  pauses. Player can continue the game with 'continue' or reset the game
 *  with 'reset'.
 *
 *  If the player changes the seed or the size of the grid while a game
 *  is on, the game is reset. If the player changes the goal value to another
 *  value that is a power of 2, the game is reset.
 *
 *  Player can exit the program by pressing the x in the top right corner.
 *
 * This file in question (mainwindow.hh) is the header for the class
 * MainWindow, which manages the user interface of the game and keeps
 * the grid updated in accordance with the internal data structure.
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


#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <QMainWindow>
#include <gameboard.hh>
#include <QGraphicsScene>
#include <numbertile.hh>
#include <bits/stdc++.h>
#include <vector>
#include <QLabel>
#include <QGraphicsRectItem>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

// Constant size of single numbertile 50x50px
const int NUMBERTILE_SIDE_LENGTH = 50;

// Constants coordinates to hold the game grid's top left corner
// in place, even when game grid changes in size.
const int LEFT_MARGIN = 420;
const int TOP_MARGIN = 100;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    // Game is played with keys wasd. Calculates if the game has been
    // won or lost on each key press.
    void keyPressEvent(QKeyEvent* event) override;

private slots:

    // When seed is changed while playing, also timer and board is reset,
    // and gameboard updated. Seed values between 0-999.
    void on_seedSpinBox_valueChanged(int arg1);

    // Checks if the selected new goal is a power of two.
    // If not, does not change goal value, but tells the user that
    // goal value remains as the old one.
    //
    // If goal value is changed while playing, resets timer and board,
    // and updates the gameboard, except when new goal value is the same
    // as the old one, eg. when changing 2048->2047->2048 while playing.
    void on_goalValueSpinBox_valueChanged(int arg1);

    // Changes the size of the grid not only on the UI, but
    // calls required methods to change the internal data structure.
    // Grid size is possible from 2x2 up to 7x7.
    // Resets the time if changed while playing.
    void on_gridSizeSpinBox_valueChanged(int arg1);

    // Slot to update the timer of following format:
    // Minutes : seconds : hundredths
    // Called every 10th millisecond when playing.
    void timer_function();

    // Enables buttons and spinboxes, resets timer to 0,
    // board to starting values and after win/loss, the background
    // color to default.
    void on_resetPushButton_clicked();

    // Starts timer, updates UI gameboard and clears focus from spinboxes.
    // Spamming the push button while playing does nothing.
    void on_startPushButton_clicked();

    // Puts game on pause. Timer paused, game doesn't respond to key strokes,
    // continue button becomes visible. Only continue and reset button possible
    // to click during a pause. Only works if game is on.
    void on_pausePushButton_clicked();

    // Slot to operate the continue button.
    // Button only visible when pause has been pressed
    void on_continuePushButton_clicked();

    // Changes the attribute random_tile_removal_interval_ to arg1.
    void on_tileRemoveSpinBox_valueChanged(int arg1);

private:
    Ui::MainWindow *ui;
    QGraphicsScene* scene_;
    GameBoard board;

    // Function to populate the UI game grid (tiles_label_) with
    // QLabels of size 50x50. Font: 14pt, Comic Sans MS, border
    // 1px solid black, numbers aligned in center of the label.
    // Takes the desired grid size as a parameter.
    void populate_ui_game_board(int grid_size);

    // Function that stops and resets the timer, resets
    // the board without changing the grid size, resets
    // moves_after_last_removal_ to 0.
    // Called by slots: on_seedSpinBox_valueChanged
    //                  on_goalValueSpinBox_valueChanged
    //                  on_resetPushButton_clicked
    void reset_timer_and_board();

    QTimer* timer;
    // Attributes to hold the timer figures
    // to be shown on the screen
    int timer_seconds_ = 0;
    int timer_minutes_ = 0;
    int timer_hundreths_ = 0;

    // After this amount of moves, the game removes
    // a random tile from the grid. Default is 20, but the user
    // can change this.
    int random_tile_removal_interval_ = 20;

    int moves_after_last_removal_ = 0;

    // Attribute to determine if the game is on pause.
    // Pause happens when player 1) wins 2) loses 3) presses pause button
    // Pause indicates that game does not respond to keyboard inputs.
    bool on_pause_ = false;

    // Following 2 functions control following widgets:
    // seedSpinBox, goalValueSpinBox, gridSizeSpinBox,
    // tileRemoveSpinBox, startPushButton, pausePushButton
    void disable_all_but_reset();
    void enable_all_but_reset();

    // Data structure to hold pointers to label objects, whiches' values
    // are to be shown on the UI
    std::vector<std::vector<QLabel*>> tiles_label_ = {};

    // Function that returns the correct Qcolor object based
    // on the value given in the parameter. Combs through COLORS data
    // structure located in mainwindow.cpp
    QColor value_to_color(int value);

    // Updates the UI, meaning the QLabels' text (figure) and color based
    // on the internal data structure. Called at least after each move made.
    void update_gameboard();

    // Attributes to hold seed and goal value
    // 0 and 2048 by default respectevely.
    int seed_value_ = 0;
    int goal_value_ = 2048;

    // Attributes to hold the size of the scene
    // when game grid changes in size.
    int width_of_scene_;
    int height_of_scene_;

    // Function to check whether the goal value
    // is power of two. Takes in the integer to be checked and
    // returns boolean value.
    bool is_power_of_two(int integer);

};
#endif // MAINWINDOW_HH
