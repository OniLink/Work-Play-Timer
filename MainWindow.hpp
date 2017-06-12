#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <random>
#include <QMainWindow>
#include <QSoundEffect>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
	Q_OBJECT

	public:
		explicit MainWindow( QWidget* parent = nullptr );
		~MainWindow();

	public slots:
		void todoListItemAdd();
		void timerChangeLength( int length );
		void timerStartStop();
		void timerTaskComplete();
		void timerTaskSkip();

	protected:
		void timerEvent( QTimerEvent* event );

	private:
		void pickTask();
		void updateTimerDisplay();

		Ui::MainWindow *ui;

		bool timer_counting;
		int current_task;
		int current_timer;
		int countdown_time;

		QSoundEffect alert;

		std::random_device rand_device;
		std::mt19937 rand_generator;
		std::uniform_int_distribution<> rand_distribution;

		const int task_none = -2;
		const int task_free = -1;
};

#endif // MAINWINDOW_HPP
