#pragma once

#include <QtWidgets/QWidget>
#include <QtNetwork/QAbstractSocket>
#include <QtNetwork/QSslError>
#include <QtNetwork/QSslSocket>
#include "ui_securesocketclient.h"

class securesocketclient : public QWidget
{
	Q_OBJECT

public:
	securesocketclient(QWidget *parent = Q_NULLPTR);
	~securesocketclient() = default;

private slots:
	void updateEnabledState();
	void secureConnect();
	void socketStateChanged(QAbstractSocket::SocketState state);
	void socketEncrypted();
	void socketReadyRead();
	void sendData();
	void socketError(QAbstractSocket::SocketError error);
	void sslErrors(const QList<QSslError> &errors);
	void displayCertificateInfo();

private:
	void setupSecureSocket();
	void appendString(const QString& line);

private:
	Ui::securesocketclientClass ui;

	QSslSocket *socket = nullptr;
	QToolButton *padLock = nullptr;
	bool handlingSocketError = false;
	bool executingDialog = false;
};
