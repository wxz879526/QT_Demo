#pragma once

#include <QtWidgets/QWidget>
#include <QDialog>
#include <QTcpServer>
#include <QTcpSocket>

QT_BEGIN_NAMESPACE
class QDialogButtonBox;
class QLabel;
class QProgressBar;
class QPushButton;
QT_END_NAMESPACE 

class loopback : public QDialog
{
	Q_OBJECT

public:
	loopback(QWidget *parent = Q_NULLPTR);


private slots:
	void start();
	void acceptConnection();
	void startTransfer();
	void updateServerProgress();
	void updateClientProgress(qint64 numBytes);
	void displayError(QAbstractSocket::SocketError err);

private:
	QProgressBar *clientProgressBar = nullptr;
	QProgressBar *serverProgressBar = nullptr;
	QLabel *clientStatusLabel = nullptr;
	QLabel *serverStatusLabel = nullptr;

	QPushButton *startButton = nullptr;
	QPushButton *quitButton = nullptr;
	QDialogButtonBox *buttonBox = nullptr;

	QTcpServer tcpServer;
	QTcpSocket tcpClient;
	QTcpSocket *tcpServerConnection = nullptr;

	int bytesToWrite = 0;
	int bytesWritten = 0;
	int bytesReceived = 0;
};
