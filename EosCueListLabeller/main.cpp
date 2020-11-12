#include <QApplication.h>
#include <qdialog.h>
#include <qstylefactory.h>
#include "MainWindow.h"

//Most of this code is boilerplate

//Define colors for a dark mode
#define DEEP_DARK_BLUE QColor("#0B101C")
#define DARK_BLUE DEEP_DARK_BLUE.lighter(175)
#define MID_BLUE QColor("#30346A")
#define MID_BLUE_DARKER MID_BLUE.darker()
#define BRIGHT_BLUE QColor("#466ec2")
#define TEXT_WHITE QColor("#B3C3DF").lighter(125)
#define TEXT_WHITE_DARKER TEXT_WHITE.darker()
#define TEXT_WHITE_BRIGHT TEXT_WHITE.lighter()
#define SILLY_RED QColor("#FF0022")

#define WARNING QColor("#CF5C36").darker()

#define ALERT QColor(253, 96, 87)

#define OUTLINE QColor(50,50,90)


//App entry point
int main(int argc, char** argv)
{
	QApplication app(argc, argv);
	app.setApplicationName("Eos Cue List Labeller");
	app.setOrganizationName("ETC Labs");
	app.setStyle(QStyleFactory::create("Fusion"));

	QPalette p = app.palette();
	p.setColor(QPalette::Window, DARK_BLUE);
	p.setColor(QPalette::WindowText, TEXT_WHITE);
	p.setColor(QPalette::Base, DEEP_DARK_BLUE);
	p.setColor(QPalette::AlternateBase, MID_BLUE_DARKER);
	p.setColor(QPalette::ToolTipBase, MID_BLUE_DARKER);
	p.setColor(QPalette::ToolTipText, TEXT_WHITE);
	p.setColor(QPalette::Text, TEXT_WHITE);
	p.setColor(QPalette::Disabled, QPalette::Text, BRIGHT_BLUE);
	p.setColor(QPalette::Button, MID_BLUE_DARKER);
	p.setColor(QPalette::ButtonText, TEXT_WHITE);
	p.setColor(QPalette::Disabled, QPalette::ButtonText, BRIGHT_BLUE);
	p.setColor(QPalette::BrightText, TEXT_WHITE_BRIGHT);
	p.setColor(QPalette::Link, BRIGHT_BLUE);

	p.setColor(QPalette::Highlight, BRIGHT_BLUE.darker());
	p.setColor(QPalette::HighlightedText, TEXT_WHITE_BRIGHT);
	p.setColor(QPalette::Disabled, QPalette::HighlightedText, BRIGHT_BLUE);

	p.setColor(QPalette::PlaceholderText, TEXT_WHITE_DARKER);

	QFont f = app.font();
	//TODO: Qt deprecated this function in at least 5.15, this will need to be changed
#pragma warning(disable:4996)
	f.setRawName("Calibri");
	app.setFont(f);

	app.setPalette(p);
	
	//Init and go!
	MainWindow m;
	m.show();
	return app.exec();
}