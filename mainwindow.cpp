/*
#############################################################################
 * COMP.CS.110 Ohjelmointi 2: Rakenteet / Programming 2: Structures
 * Project4: Peli nimeltä 2048 / Game called 2048
 * File: numbers_gui
 *
 * This is the implementation file for the class MainWindow
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


#include "mainwindow.hh"
#include "ui_mainwindow.h"
#include <QKeyEvent>

// Struct to link each value to a Qcolor object
struct ColorOfValue {
    int value;
    QColor color_name;
};

// Vector to hold structs which link each value to a Qcolor object
const std::vector<ColorOfValue> COLORS = {
    {0, Qt::gray},
    {2, QColor(72,209,204)}, // Medium turquoise
    {4, QColor(154,205,0)}, // Yellow Green
    {8, QColor(255,165,0)}, // Orange
    {16, QColor(102,102,153)}, // Dark blue-gray
    {32, QColor(0,128,128)}, // Teal
    {64, QColor(255,215,0)}, // Gold
    {128, QColor(176,48,96)}, // Maroon
    {256, QColor(0,0,205)}, // Medium blue
    {512, QColor(75,0,130)}, //Indigo
    {1024, QColor(250,128,114)}, //Salmon
    {2048, QColor(210,105,30)} // Chocolate
    // Rest will be yellow (255,255,0)
};

// Coords = std::pair<int, int>;
const Coords DEFAULT_DIR = {0 , 0};
const Coords LEFT = {0, -1};
const Coords UP = {-1, 0};
const Coords RIGHT = {0, 1};
const Coords DOWN = {1, 0};


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    // Initialize internal gameboard with default
    //seed_value_ of 0 and grid size of 4x4
    board.init_empty();
    board.fill(seed_value_);

    ui->setupUi(this);

    // Agraphics scene in which to draw the squares
    scene_ = new QGraphicsScene(this);

    // Calculates the size of scene depending on the grid size
    width_of_scene_ = board.get_grid_size()*NUMBERTILE_SIDE_LENGTH;
    height_of_scene_ = board.get_grid_size() * NUMBERTILE_SIDE_LENGTH;

    // Set up for the timer functionality
    timer = new QTimer(this);
    // Background and figure color for each timer component
    // (Minute, second, hundredths)
    ui->lcdNumberMin->setStyleSheet("background-color:white;");
    ui->lcdNumberMin->setPalette(Qt::black);
    ui->lcdNumberSec->setStyleSheet("background-color:white;");
    ui->lcdNumberSec->setPalette(Qt::black);
    ui->lcdNumberHundreths->setStyleSheet("background-color:white;");
    ui->lcdNumberHundreths->setPalette(Qt::black);

    // The width of the graphicsView is BORDER_RIGHT added by 2,
    // since the borders take one pixel on each side
    // (1 on the left, and 1 on the right).
    // Similarly, the height of the graphicsView is BORDER_DOWN added by 2.
    ui->graphicsView->setGeometry(LEFT_MARGIN, TOP_MARGIN,
                                   width_of_scene_ + 2, height_of_scene_ + 2);
    ui->graphicsView->setScene(scene_);


    // The width of the scene_ is BORDER_RIGHT decreased by 1 and
    // the height of it is BORDER_DOWN decreased by 1,
    // because the circle is considered to be inside the sceneRect,
    // if its upper left corner is inside the sceneRect.
    scene_->setSceneRect(0, 0, width_of_scene_ - 1, height_of_scene_ - 1);


    // Connecting buttons, spinboxes, and timer (their signals) to necessary slots
    connect(ui->resetPushButton, &QPushButton::clicked,
            this, &MainWindow::on_resetPushButton_clicked);
    connect(ui->startPushButton, &QPushButton::clicked,
            this, &MainWindow::on_startPushButton_clicked);
    connect(ui->goalValueSpinBox, &QSpinBox::valueChanged,
            this, &MainWindow::on_goalValueSpinBox_valueChanged);
    connect(ui->seedSpinBox, &QSpinBox::valueChanged,
            this, &MainWindow::on_seedSpinBox_valueChanged);
    connect(ui->gridSizeSpinBox, &QSpinBox::valueChanged,
            this, &MainWindow::on_gridSizeSpinBox_valueChanged);
    connect(timer,&QTimer::timeout, this, &MainWindow::timer_function);


    // Default goal value
    ui->goalValueSpinBox->setValue(2048);
    // Default grid size
    board.set_grid_size(4);
    // Continue button only visible when pause is clicked
    ui->continuePushButton->hide();
    // Populates the ui gameboard data structure with QLabel objects
    populate_ui_game_board(board.get_grid_size());

}

MainWindow::~MainWindow()
{

    delete ui;
}


void MainWindow::on_seedSpinBox_valueChanged(int arg1)
{
    ui->textBrowser->clear();
    seed_value_ = arg1;

    reset_timer_and_board();

    update_gameboard();
}

void MainWindow::on_startPushButton_clicked()
{
    // Spamming the push button does nothing
    if(timer->isActive())
    {
        return;
    }

    ui->textBrowser->clear();

    // Clears focus from spinboxes, so the game
    // responds to key strokes immediately.
    ui->seedSpinBox->clearFocus();
    ui->goalValueSpinBox->clearFocus();
    ui->gridSizeSpinBox->clearFocus();
    ui->tileRemoveSpinBox->clearFocus();

    update_gameboard();

    timer->start(10);
}

bool MainWindow::is_power_of_two(int integer)
{
    return (ceil(log2(integer)) == floor(log2(integer)));
}

void MainWindow::on_goalValueSpinBox_valueChanged(int arg1)
{
    ui->textBrowser->clear();

    // Checks if the selected goal is a power of two
    if (not is_power_of_two(arg1))
    {
        ui->textBrowser->setText("Goal not a power of two, goal is set as " + QString::number(goal_value_));
        return;
    } else {
        // Board doesn't reset when new and old goal values are the same.
        if(goal_value_ == arg1)
        {
            return;
        }

        goal_value_ = arg1;
        reset_timer_and_board();
        update_gameboard();
    }
}

void MainWindow::timer_function()
{
    timer_hundreths_ +=1;
    if(timer_hundreths_ == 100)
    {
        timer_seconds_ += 1;
        timer_hundreths_ = 0;
    }
    if(timer_seconds_ == 60)
    {
        timer_minutes_ += 1;
        timer_seconds_ = 0;
    }

    ui->lcdNumberMin->display(timer_minutes_);
    ui->lcdNumberSec->display(timer_seconds_);
    ui->lcdNumberHundreths->display(timer_hundreths_);
}

void MainWindow::update_gameboard()
{
        // 2 for-loops for nested data structure.
        for(unsigned int y = 0; y < tiles_label_.size(); ++y)
        {
            for(unsigned int x = 0; x < tiles_label_.at(y).size(); ++x)
            {
                // to figure out the value of each label
                NumberTile* temp = board.get_item({y, x});
                int value = temp->get_value();

                // To figure out the color of each label
                QPalette palette = tiles_label_.at(y).at(x)->palette();
                palette.setColor(tiles_label_.at(y).at(x)->backgroundRole(), value_to_color(value));
                tiles_label_.at(y).at(x)->setPalette(palette);

                // Only values over are explicitly shown
                if (value == 0)
                {
                    tiles_label_.at(y).at(x)->clear();
                }else
                {
                    tiles_label_.at(y).at(x)->setText(QString::number(value));
                }
            }
        }
}

void MainWindow::keyPressEvent(QKeyEvent* event) {
    Coords dir = DEFAULT_DIR;
    ui->textBrowser->clear();
    // Doesn't respond to keys if pause is on
    if(ui->pausePushButton->isDown() or on_pause_)
    {
        return;
    }
    // WASD keys.
    if(event->key() == Qt::Key_W)
    {
        dir = UP;
    }
    if(event->key() == Qt::Key_S)
    {
        dir = DOWN;
    }
    if(event->key() == Qt::Key_A)
    {
        dir = LEFT;
    }
    if(event->key() == Qt::Key_D)
    {
        dir = RIGHT;
    }

    if( dir != DEFAULT_DIR )
    {
        // If the player starts playing by pressing keys
        // instead of pressing 'start', the timer is started here
        if(not timer->isActive())
        {
            timer->start(10);
        }

        // Random tile removal implementation
        ++moves_after_last_removal_;
        if(moves_after_last_removal_ >= random_tile_removal_interval_)
        {
            board.remove_random_tile();
            ui->textBrowser->setText("A random tile removed!");
            moves_after_last_removal_ = 0;
        }

        // Win scenario
        if( board.move(dir, goal_value_) )
        {
            update_gameboard();

            ui->textBrowser->setText("You reached the goal value of " + QString::number(goal_value_) + "! Press reset to play again.");
            this->setStyleSheet("QMainWindow {background: 'seagreen';}");
            timer->stop();

            ui->pausePushButton->setDown(true);

            disable_all_but_reset();
            on_pause_ = true;
            return;
        }
        // Lose scenario
        else if( board.is_full() )
        {
            ui->textBrowser->setText("Can't add new tile, you lost! Press reset to play again.");
            update_gameboard();
            this->setStyleSheet("QMainWindow {background: 'crimson';}");
            timer->stop();

            ui->pausePushButton->setDown(true);

            disable_all_but_reset();
            on_pause_ = true;
            return;
        }
        // Neither a win or lose scenario
        board.new_value(false);
        update_gameboard();
    return;
}
}

void MainWindow::on_resetPushButton_clicked()
{
    enable_all_but_reset();
    on_pause_ = false;

    ui->pausePushButton->setDown(false);
    ui->textBrowser->clear();
    ui->continuePushButton->hide();
    reset_timer_and_board();

    update_gameboard();

    // After a win/loss, resets the background color
    this->setStyleSheet("");
}

void MainWindow::on_gridSizeSpinBox_valueChanged(int arg1)
{
    // Grid size from 2x2 up to 7x7 can't be exceeded.
    if(arg1>7)
    {
        ui->textBrowser->setText("Grid size can't be bigger than 7x7");
        ui->gridSizeSpinBox->setValue(7);
        return;
    } else if(arg1 < 2)
    {
        ui->textBrowser->setText("Grid size can't be smaller than 2x2");
        ui->gridSizeSpinBox->setValue(2);
        return;
    }

    // Reset the timer (game resets when grid changed while playing)
    if(timer->isActive())
    {
        timer->stop();
    }
    timer_hundreths_ = 0;
    timer_seconds_ = 0;
    timer_minutes_ = 0;
    ui->lcdNumberMin->display(timer_minutes_);
    ui->lcdNumberSec->display(timer_seconds_);
    ui->lcdNumberHundreths->display(timer_hundreths_);

    moves_after_last_removal_ = 0;

    // Delete current UI data structure of Qlabel objects.
    for(unsigned int y = 0; y < tiles_label_.size(); ++y)
    {
        for(unsigned int x = 0; x < tiles_label_.at(y).size(); ++x)
        {
            delete tiles_label_.at(y).at(x);
            tiles_label_.at(y).at(x) = nullptr;
        }
    }

    // Initialize as empty
    tiles_label_ = {};
    // New grid_size
    board.set_grid_size(arg1);
    // Populate UI data structure with new grid size determined
    // amount of pointers to QLabel objects.
    populate_ui_game_board(arg1);

    // Set new scene size to accomodate different size grid.
    width_of_scene_ = board.get_grid_size()*NUMBERTILE_SIDE_LENGTH;
    height_of_scene_ = board.get_grid_size() * NUMBERTILE_SIDE_LENGTH;
    ui->graphicsView->setGeometry(LEFT_MARGIN, TOP_MARGIN,
                                   width_of_scene_ + 2, height_of_scene_ + 2);
    ui->graphicsView->setScene(scene_);
    scene_->setSceneRect(0, 0, width_of_scene_ - 1, height_of_scene_ - 1);

    // Reset internal data structure with pointers to NumberTile objects, and
    // initialize with values.
    board.reset(seed_value_, true);

    update_gameboard();
}

void MainWindow::populate_ui_game_board(int grid_size)
{
    int yflag = 0;
    while (yflag < grid_size)
    {
        int xflag = 0;
        std::vector<QLabel*> temp = {};

        while(xflag < board.get_grid_size())
        {
            QLabel* new_label = new QLabel();
            scene_->addWidget(new_label);
            new_label->setFixedSize(NUMBERTILE_SIDE_LENGTH, NUMBERTILE_SIDE_LENGTH);
            new_label->move(NUMBERTILE_SIDE_LENGTH*xflag, yflag*NUMBERTILE_SIDE_LENGTH);
            new_label->setStyleSheet("QLabel{font-size: 14pt;}");
            new_label->setAlignment(Qt::AlignCenter);
            new_label->setFont(QFont("Comic Sans MS"));
            new_label->setStyleSheet("border: 1px solid black");

            temp.push_back(new_label);
            ++xflag;
        }
        tiles_label_.push_back(temp);
        ++yflag;
    }
}

void MainWindow::reset_timer_and_board()
{
    // If seed value is changed while playing the game, timer resets
    if(timer->isActive())
    {
        timer->stop();
    }
    timer_hundreths_ = 0;
    timer_seconds_ = 0;
    timer_minutes_ = 0;
    ui->lcdNumberMin->display(timer_minutes_);
    ui->lcdNumberSec->display(timer_seconds_);
    ui->lcdNumberHundreths->display(timer_hundreths_);


    moves_after_last_removal_ = 0;

    // If seed value is changed while playing the game, board resets
    board.reset(seed_value_);


}

void MainWindow::disable_all_but_reset()
{
    ui->seedSpinBox->setDisabled(true);
    ui->goalValueSpinBox->setDisabled(true);
    ui->gridSizeSpinBox->setDisabled(true);
    ui->startPushButton->setDisabled(true);
    ui->pausePushButton->setDisabled(true);
    ui->tileRemoveSpinBox->setDisabled(true);
}

void MainWindow::enable_all_but_reset()
{
    ui->seedSpinBox->setEnabled(true);
    ui->goalValueSpinBox->setEnabled(true);
    ui->gridSizeSpinBox->setEnabled(true);
    ui->startPushButton->setEnabled(true);
    ui->pausePushButton->setEnabled(true);
    ui->tileRemoveSpinBox->setEnabled(true);
}

QColor MainWindow::value_to_color(int value)
{
    for(auto c : COLORS)
    if(value == c.value)
    {
        return c.color_name;
    }
    return QColor(255,255,0); //Yellow
}

void MainWindow::on_pausePushButton_clicked()
{
        // Pause button only works when game is on.
        if(not timer->isActive())
        {
            return;
        }
        timer->stop();
        ui->textBrowser->setText("The game is on pause, press 'Continue' to continue or 'Reset' to reset");

        ui->pausePushButton->setDown(true);
        on_pause_ = true;

        disable_all_but_reset();

        // Continue button only visible on pause
        ui->continuePushButton->setVisible(true);
}

void MainWindow::on_continuePushButton_clicked()
{
    timer->start(10);
    ui->textBrowser->clear();
    ui->pausePushButton->setDown(false);
    on_pause_ = false;

    enable_all_but_reset();

    // Continue button only visible on pause
    ui->continuePushButton->setVisible(false);

    // Needed so that spinboxes don't take priority
    // over the key event right after pressing continue button
    ui->seedSpinBox->clearFocus();
    ui->goalValueSpinBox->clearFocus();
    ui->gridSizeSpinBox->clearFocus();
    ui->tileRemoveSpinBox->clearFocus();


}


void MainWindow::on_tileRemoveSpinBox_valueChanged(int arg1)
{
    random_tile_removal_interval_ = arg1;
}

