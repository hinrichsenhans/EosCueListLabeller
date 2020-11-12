#include "MainWindow.h"

#include <qtablewidget.h>
#include <qdebug.h>
#include <quuid.h>
#include <qheaderview.h>


MainWindow::MainWindow()
	: QMainWindow()
{
	setWindowTitle(QString("Eos Cue List Labeller " VERSION));
	setMinimumSize(200, 200);
	resize(450, 600);
	table_ = new QTableWidget(this);
	table_->setColumnCount(Rows::kCount);
	table_->setRowCount(50);
	table_->horizontalHeader()->setStretchLastSection(true);
	table_->setHorizontalHeaderLabels({ "Cue List #", "Label" });
	table_->verticalHeader()->hide();
	setCentralWidget(table_);
	connect(table_, &QTableWidget::itemChanged, this, &MainWindow::OnItemEdited);
	connect(&tick_, &QTimer::timeout, this, &MainWindow::Tick);
	tick_.setInterval(10);
	tick_.start();

	UpdateTable();
}

void MainWindow::Tick()
{
	if (state_ == State::kStop || !sync_.IsRunning())
	{
		EosTarget::TYPE_LIST subscribedTypes;
		subscribedTypes.push_back(EosTarget::EnumEosTargetType::EOS_TARGET_CUELIST);
		if (sync_.Initialize("127.0.0.1", EosSyncLib::DEFAULT_PORT, &subscribedTypes))
			state_ = State::kInitialized;
	}
	sync_.Tick();

	if (!sync_.IsSynchronized())
		UpdateTable();
	if (sync_.IsConnectedAndSynchronized())
	{
		if (sync_.GetCueList().GetStatus().GetDirty())
		{
			UpdateTable();
			sync_.ClearDirty();
		}
	}

	EosLog& log = sync_.GetLog();
	EosLog::LOG_Q q;
	log.Flush(q);
	log.Clear();
	for (const EosLog::sLogMsg& msg : q)
	{
		qDebug() << QString::fromStdString(msg.text);
	}
}

QString PrintEosDecimal(const EosTarget::sDecimalNumber& dec)
{
	if (dec.decimal == 0)
		return QString::number(dec.whole);
	return QString("%1.%2").arg(dec.whole).arg(dec.decimal);
}

QString GetLabelFromFirstPart(const EosTargetList::PARTS& p)
{
	if (p.empty())
		return QString();
	const EosTarget::PROP_GROUPS propGroups = p.begin()->second->GetPropGroups();
	const EosTarget::PROP_GROUPS::const_iterator iter = propGroups.find("");
	if (iter != propGroups.end() && iter->second.props.size() > MainWindow::OSCConsts::CUELISTLABEL)
	{
		return QString::fromStdString(iter->second.props.at(MainWindow::OSCConsts::CUELISTLABEL).value);
	}
	return QString();
}

QTableWidgetItem* MakeNonEditableItem(QString name)
{
	QTableWidgetItem* item = new QTableWidgetItem(name);
	item->setFlags(Qt::ItemIsEnabled | Qt::ItemFlag::ItemIsSelectable);
	return item;
}


QTableWidgetItem* MakeEditableItem(QString name, const EosTarget::sDecimalNumber & targetId)
{
	QTableWidgetItem* item = new QTableWidgetItem(name);
	item->setData(MainWindow::ItemRole::kTargetWhole, targetId.whole);
	item->setData(MainWindow::ItemRole::kTargetDecimal, targetId.decimal);
	item->setFlags(Qt::ItemIsEnabled | Qt::ItemFlag::ItemIsSelectable | Qt::ItemFlag::ItemIsEditable);
	return item;
}

void MainWindow::UpdateTable()
{
	table_->clearContents();
	bool block = table_->blockSignals(true);

	if (!sync_.IsSynchronized())
	{
		table_->setRowCount(1);
		table_->setItem(0, 0, MakeNonEditableItem("Syncing..."));
	}
	else
	{
		const EosTargetList& cuelists = sync_.GetCueList();
		const EosTargetList::TARGETS& cuelistData = cuelists.GetTargets();
		table_->setRowCount(static_cast<int>(cuelistData.size()));
		int row = 0;
		for (EosTargetList::TARGETS::const_iterator iter = cuelistData.cbegin(); iter != cuelistData.cend(); ++iter)
		{
			table_->setItem(row, Rows::kId, MakeNonEditableItem(PrintEosDecimal(iter->first)));
			table_->setItem(row, Rows::kLabel, MakeEditableItem(GetLabelFromFirstPart(iter->second.list), iter->first));
			++row;
		}
	}
	table_->blockSignals(block);
}

void MainWindow::OnItemEdited(QTableWidgetItem* item)
{
	EosTarget::sDecimalNumber dec;
	dec.whole = item->data(ItemRole::kTargetWhole).toInt();
	dec.decimal = item->data(ItemRole::kTargetDecimal).toInt();
	QString text = item->data(Qt::ItemDataRole::DisplayRole).toString();

	QString path = QString("/eos/set/cuelist/%1/label").arg(PrintEosDecimal(dec));

	OSCPacketWriter writer(path.toStdString());
	writer.AddString(text.toStdString());
	sync_.Send(writer, false);
}
