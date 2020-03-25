#include "loopback.h"
#include <QLabel>
#include <QPushButton>
#include <QProgressBar>
#include <QDialogButtonBox>
#include <QGuiApplication>
#include <QVBoxLayout>
#include <QMessageBox>

static const int TotalBytes = 50 * 1024 * 1024; //50M
static const int PayloadSize = 64 * 1024; //64KB

loopback::loopback(QWidget *parent)
	: QDialog(parent)
{
	QVBoxLayout *mainLayout = new QVBoxLayout();

	clientProgressBar = new QProgressBar;
	mainLayout->addWidget(clientProgressBar);

	clientStatusLabel = new QLabel(tr("Client ready"));
	mainLayout->addWidget(clientStatusLabel);

	serverProgressBar = new QProgressBar;
	mainLayout->addWidget(serverProgressBar);

	serverStatusLabel = new QLabel(tr("Server ready"));
	mainLayout->addWidget(serverStatusLabel);

	startButton = new QPushButton(tr("&start"));
	quitButton = new QPushButton(tr("&Quit"));
	buttonBox = new QDialogButtonBox;
	buttonBox->addButton(startButton, QDialogButtonBox::ActionRole);
	buttonBox->addButton(quitButton, QDialogButtonBox::RejectRole);

	connect(startButton, &QPushButton::clicked, this, &loopback::start);
	connect(quitButton, &QPushButton::clicked, this, &QWidget::close);

	connect(&tcpServer, &QTcpServer::newConnection, this, &loopback::acceptConnection);
	connect(&tcpClient, &QTcpSocket::connected, this, &loopback::startTransfer);
	connect(&tcpClient, &QIODevice::bytesWritten, this, &loopback::updateClientProgress);
	connect(&tcpClient, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),
		this, &loopback::displayError);
	mainLayout->addStretch(1);
	mainLayout->addSpacing(10);
	mainLayout->addWidget(buttonBox);

	setLayout(mainLayout);
}

void loopback::start()
{
	startButton->setEnabled(false);

#ifndef QT_NO_CUSOR
	QGuiApplication::setOverrideCursor(Qt::WaitCursor);
#endif

	bytesWritten = 0;
	bytesReceived = 0;

	while (!tcpServer.isListening() && !tcpServer.listen())
	{
		QMessageBox::StandardButton ret = QMessageBox::critical(this,
			tr("Loopback"),
			tr("Unable to start the test: %l.").arg(tcpServer.errorString()),
			QMessageBox::Retry | QMessageBox::Cancel);

		if (QMessageBox::Cancel == ret)
			return;
	}

	serverStatusLabel->setText(tr("Listening"));
	clientStatusLabel->setText(tr("Connecting"));
	tcpClient.connectToHost(QHostAddress::LocalHost, tcpServer.serverPort());
}

void loopback::acceptConnection()
{
	tcpServerConnection = tcpServer.nextPendingConnection();
	if (!tcpServerConnection)
	{
		serverStatusLabel->setText(tr("Error: got invalid pending connection!"));
		return;
	}

	connect(tcpServerConnection, &QIODevice::readyRead, this, &loopback::updateServerProgress);
	connect(tcpServerConnection, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),
		this, &loopback::displayError);
	connect(tcpServerConnection, &QTcpSocket::disconnected, tcpServerConnection, &QTcpSocket::deleteLater);

	serverStatusLabel->setText(tr("Accepted connected"));
	tcpServer.close();
}

void loopback::startTransfer()
{
	bytesToWrite = TotalBytes - int(tcpClient.write(QByteArray(PayloadSize, '@')));
	clientStatusLabel->setText(tr("Connected"));
}

void loopback::updateServerProgress()
{
	bytesReceived += int(tcpServerConnection->bytesAvailable());
	tcpServerConnection->readAll();

	serverProgressBar->setMaximum(TotalBytes);
	serverProgressBar->setValue(bytesReceived);
	serverStatusLabel->setText(tr("Received: %1MB").arg(bytesReceived / (1024 * 1024)));

	if (bytesReceived == TotalBytes) {
		tcpServerConnection->close();
		startButton->setEnabled(true);
#ifndef QT_NO_CURSOR
		QGuiApplication::restoreOverrideCursor();
#endif
	}
}

void loopback::updateClientProgress(qint64 numBytes)
{
	bytesWritten += (int)(numBytes);

	if (bytesToWrite > 0 && tcpClient.bytesToWrite() <= 4 * PayloadSize)
		bytesToWrite -= tcpClient.write(QByteArray(qMin(bytesToWrite, PayloadSize), '@'));

	clientProgressBar->setMaximum(TotalBytes);
	clientProgressBar->setValue(bytesWritten);
	clientStatusLabel->setText(tr("Sent: %1MB").arg(bytesWritten/(1024*1024)));
}

void loopback::displayError(QAbstractSocket::SocketError err)
{
	if (QTcpSocket::RemoteHostClosedError == err)
		return;

	QMessageBox::information(this, tr("Network error"),
		tr("The following error occurred: %1.").arg(tcpClient.errorString()));

	tcpClient.close();
	tcpServer.close();
	clientProgressBar->reset();
	serverProgressBar->reset();
	clientStatusLabel->setText(tr("Client ready"));
	serverStatusLabel->setText(tr("Client ready"));

#ifndef QT_NO_CURSOR
	QGuiApplication::restoreOverrideCursor();
#endif
}
