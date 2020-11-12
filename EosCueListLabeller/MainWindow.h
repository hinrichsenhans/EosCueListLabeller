#pragma once
#include <qmainwindow.h>
#include <qtimer.h>

#include "EosSyncLib.h"

#define VERSION "1.0.0.1"

class QTableWidget;
class QTableWidgetItem;

class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	enum class State
	{
		kStop,
		kInitialized,
	};
	MainWindow();

	void Tick();

	enum OSCConsts
	{
		CUELISTLABEL = 2,
	};
	enum ItemRole
	{
		kTargetWhole = Qt::UserRole + 1,
		kTargetDecimal,
	};
private:
	enum Rows
	{
		kId,
		kLabel,
		kCount
	};
	void UpdateTable();
	void OnItemEdited(QTableWidgetItem* item);
	State state_ = State::kStop;
	QTableWidget * table_ = nullptr;
	QTimer tick_;
	EosSyncLib sync_;
};

