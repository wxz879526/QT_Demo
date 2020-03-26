#include "http.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	http w;
	w.show();
	return a.exec();
}
