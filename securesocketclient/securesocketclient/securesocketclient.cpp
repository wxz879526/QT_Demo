#include "stdafx.h"
#include <QtNetwork/QSslSocket>
#include <QtNetwork/QSslCipher>
#include "securesocketclient.h"

securesocketclient::securesocketclient(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	ui.hostNameEdit->setSelection(0, ui.hostNameEdit->text().size());
	ui.sessionOutput->setHtml(tr("&lt;not connected&gt;"));

	connect(ui.hostNameEdit, &QLineEdit::textChanged, this, &securesocketclient::updateEnabledState);
	connect(ui.connectButton, &QPushButton::clicked, this, &securesocketclient::secureConnect);
	connect(ui.sendButton, &QPushButton::click, this, &securesocketclient::sendData);

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
	const bool unconnected = socket->state() == QAbstractSocket::UnconnectedState;
	ui.hostNameEdit->setReadOnly(!unconnected);
	ui.hostNameEdit->setFocusPolicy(unconnected ? Qt::StrongFocus : Qt::NoFocus);
	ui.hostNameLabel->setEnabled(unconnected);
	ui.portBox->setEnabled(unconnected);
	ui.portLabel->setEnabled(unconnected);
	ui.connectButton->setEnabled(unconnected && !ui.hostNameEdit->text().isEmpty());

	const bool connected = socket->state() == QAbstractSocket::ConnectedState;
	ui.sessionInput->setEnabled(connected);
	ui.sessionOutput->setEnabled(connected);
	ui.sessionInputLabel->setEnabled(connected);
	ui.connectButton->setEnabled(connected);
}

void securesocketclient::secureConnect()
{
	socket->connectToHostEncrypted(ui.hostNameEdit->text(), ui.portBox->value());
	updateEnabledState();
}

void securesocketclient::socketStateChanged(QAbstractSocket::SocketState state)
{
	if (executingDialog)
		return;

	updateEnabledState();

	if (state == QAbstractSocket::UnconnectedState) {
		ui.sessionInput->clear();
		ui.hostNameEdit->setPalette(QPalette());
		ui.hostNameEdit->setFocus();
		ui.cipherLabel->setText("<none>");
		padLock->hide();
	}
}

void securesocketclient::socketEncrypted()
{
	ui.sessionOutput->clear();
	ui.sessionInput->setFocus();

	QPalette palette;
	palette.setColor(QPalette::Base, QColor(255, 255, 192));
	ui.hostNameEdit->setPalette(palette);

	const QSslCipher cipher = socket->sessionCipher();
	const QString cipherInfo = QString("%1, %2 (%3/%4)").arg(cipher.authenticationMethod()).arg(cipher.name()).arg(cipher.usedBits()).arg(cipher.supportedBits());

	ui.cipherLabel->setText(cipherInfo);
	padLock->show();
}

void securesocketclient::socketReadyRead()
{
	appendString(QString::fromUtf8(socket->readAll()));
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

void securesocketclient::appendString(const QString& line)
{
	QTextCursor cursor(ui.sessionOutput->textCursor());
	cursor.movePosition(QTextCursor::End);
	cursor.insertText(line);
	ui.sessionOutput->verticalScrollBar()->setValue(ui.sessionOutput->verticalScrollBar()->maximum());
}
