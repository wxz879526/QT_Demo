#include "stdafx.h"
#include "securesocketclient.h"
#include <QtWidgets/QApplication>
#include <QtNetwork/QSslSocket>

int main(int argc, char *argv[])
{
	Q_INIT_RESOURCE(securesocketclient);

	QApplication a(argc, argv);
	
	qDebug() << QSslSocket::sslLibraryBuildVersionString();

	auto bSupport = QSslSocket::supportsSsl();
	if (!bSupport)
	{
		QMessageBox::information(nullptr, "error", QString("Not support ssl"));
		return -1;
	}
	

	securesocketclient w;
	w.show();
	return a.exec();
}
