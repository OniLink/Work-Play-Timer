#include "MainWindow.hpp"
#include "ui_MainWindow.h"

#include <iostream>

MainWindow::MainWindow( QWidget* parent ) :
    QMainWindow( parent ),
    ui( new Ui::MainWindow ),
    rand_generator( rand_device() ),
    rand_distribution( 0, 0 ) {
	ui->setupUi( this );
	timer_counting = false;
	countdown_time = 1800;
	updateTimerDisplay();
	current_task = task_none;
	ui->labelCurrentTask->setText( "N/A" );
	alert.setSource( QUrl::fromLocalFile( "Resources/Alert.wav" ) );
}

MainWindow::~MainWindow() {
	delete ui;
}

void MainWindow::timerEvent( QTimerEvent* /*event*/ ) {
	countdown_time -= 1;

	if( countdown_time < 0 ) {
		alert.play();
		if( alert.status() == QSoundEffect::Null ) {
			std::cout << "Not loaded\n";
		} else if( alert.status() == QSoundEffect::Loading ) {
			std::cout << "Loading\n";
		} else if( alert.status() == QSoundEffect::Error ) {
			std::cout << "Error\n";
		}

		if( current_task == task_free ) {
			pickTask();
			countdown_time = ui->sliderTimeDivision->value() * 60;
		} else {
			current_task = task_free;
			ui->labelCurrentTask->setText( "Free Time" );
			countdown_time = 3600 - ui->sliderTimeDivision->value() * 60;
		}
	}

	updateTimerDisplay();
}

void MainWindow::todoListItemAdd() {
	ui->listTodo->addItem( ui->itemEntryTodo->text() );
	ui->itemEntryTodo->clear();
	rand_distribution = std::uniform_int_distribution<>( 0, ui->listTodo->count() - 1 );
}

void MainWindow::timerChangeLength( int length ) {
	if( !timer_counting ) {
		countdown_time = length * 60;
		updateTimerDisplay();
	}

	QString time_string_buffer;
	time_string_buffer.setNum( length );
	ui->labelWorkTime->setText( time_string_buffer + QString( " min" ) );
	time_string_buffer.setNum( 60 - length );
	ui->labelPlayTime->setText( time_string_buffer + QString( " min" ) );
}

void MainWindow::timerStartStop() {
	if( timer_counting ) {
		killTimer( current_timer );
		ui->buttonStartStop->setText( "Start" );
		current_task = task_none;
		ui->labelCurrentTask->setText( "N/A" );
		countdown_time = ui->sliderTimeDivision->value() * 60;
		updateTimerDisplay();
	} else {
		pickTask();
		ui->buttonStartStop->setText( "Stop" );
		current_timer = startTimer( 1000, Qt::VeryCoarseTimer );
	}

	timer_counting = !timer_counting;
}

void MainWindow::timerTaskComplete() {
	if( current_task == task_free || !timer_counting ) {
		return;
	}

	if( current_task >= 0 ) {
		delete ui->listTodo->takeItem( current_task );
	}

	if( ui->listTodo->count() > 0 ) {
		rand_distribution = std::uniform_int_distribution<>( 0, ui->listTodo->count() - 1 );
	}

	pickTask();
}

void MainWindow::timerTaskSkip() {
	if( current_task == task_free || !timer_counting ) {
		return;
	}

	pickTask();
}

void MainWindow::pickTask() {
	if( ui->listTodo->count() != 0 ) {
		current_task = rand_distribution( rand_generator );
		ui->labelCurrentTask->setText( ui->listTodo->item( current_task )->text() );
	} else {
		current_task = task_none;
		ui->labelCurrentTask->setText( "N/A" );
	}
}

void MainWindow::updateTimerDisplay() {
	int residue = countdown_time;
	int timer_seconds_ones = residue % 10;
	ui->counterTimeLeftSecondsOnes->display( timer_seconds_ones );

	residue -= timer_seconds_ones;
	residue /= 10;
	int timer_seconds_tens = residue % 6;
	ui->counterTimeLeftSecondsTens->display( timer_seconds_tens );

	residue -= timer_seconds_tens;
	residue /= 6;
	int timer_minutes_ones = residue % 10;
	ui->counterTimeLeftMinutesOnes->display( timer_minutes_ones );

	residue -= timer_minutes_ones;
	residue /= 10;
	int timer_minutes_tens = residue;
	ui->counterTimeLeftMinutesTens->display( timer_minutes_tens );
}
