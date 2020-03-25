#include "loopback.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	loopback w;
	w.show();
	return a.exec();
}
