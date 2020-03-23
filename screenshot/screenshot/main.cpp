#include "screenshot.h"
#include <QScreen>
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	
	Screenshot screenshot;
	screenshot.move(screenshot.screen()->availableGeometry().topLeft() + QPoint(20, 20));
	screenshot.show();

	return a.exec();
}
