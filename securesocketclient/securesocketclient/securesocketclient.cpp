#include "stdafx.h"
#include <QtNetwork/QSslSocket>
#include "securesocketclient.h"

securesocketclient::securesocketclient(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	ui.hostNameEdit->setSelection(0, ui.hostNameEdit->text().size());
	ui.sessionOutput->setHtml(tr("&lt;not connected&gt;"));

	connect(ui.hostNameEdit, &QLineEdit::textChanged, this, &securesocketclient::updateEnabledState);
	connect(ui.connectButton, &QPushButton::clicked, this, &securesocketclient::secureConnect);
	connect(ui.pushButton_2, &QPushButton::click, this, &securesocketclient::sendData);

	padLock = new QToolButton;
	padLock->setIcon(QIcon(":/encrypted.png"));
	connect(padLock, &QToolButton::clicked, this, &securesocketclient::displayCertificateInfo);

	padLock->setCursor(Qt::ArrowCursor);
	padLock->setToolTip(tr("Display encryption details"));

	const int extent = ui.hostNameEdit->height() - 2;
	padLock->resize(extent, extent);
	padLock->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Ignored);

	QHBoxLayout *layout = new QHBoxLayout(ui.hostNameEdit);
	const int margin = ui.hostNameEdit->style()->pixelMetric(QStyle::PM_DefaultFrameWidth);
	layout->setContentsMargins(margin, margin, margin, margin);
	layout->setSpacing(0);
	layout->addStretch();
	layout->addWidget(padLock);

	ui.hostNameEdit->setLayout(layout);
	padLock->hide();


	setupSecureSocket();
}

void securesocketclient::updateEnabledState()
{

}

void securesocketclient::secureConnect()
{

}

void securesocketclient::socketStateChanged(QAbstractSocket::SocketState state)
{

}

void securesocketclient::socketEncrypted()
{

}

void securesocketclient::socketReadyRead()
{

}

void securesocketclient::sendData()
{

}

void securesocketclient::socketError(QAbstractSocket::SocketError error)
{

}

void securesocketclient::sslErrors(const QList<QSslError> &errors)
{

}

void securesocketclient::displayCertificateInfo()
{

}

void securesocketclient::setupSecureSocket()
{
	if (socket) return;

	socket = new QSslSocket(this);
	connect(socket, &QSslSocket::stateChanged, this, &securesocketclient::socketStateChanged);
	connect(socket, &QSslSocket::encrypted, this, &securesocketclient::socketEncrypted);
	connect(socket, qOverload<QAbstractSocket::SocketError>(&QSslSocket::error), this, &securesocketclient::socketError);
	connect(socket, qOverload<const QList<QSslError>&>(&QSslSocket::sslErrors), this, &securesocketclient::sslErrors);
	connect(socket, &QSslSocket::readyRead, this, &securesocketclient::socketReadyRead);
}
