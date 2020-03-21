#include "stdafx.h"
#include <QtNetwork/QSslSocket>
#include <QtNetwork/QSslCipher>
#include <QMessageBox>
#include "ui_sslerrors.h"
#include "certificateinfo.h"
#include "securesocketclient.h"

securesocketclient::securesocketclient(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	ui.hostNameEdit->setSelection(0, ui.hostNameEdit->text().size());
	ui.sessionOutput->setHtml(tr("&lt;not connected&gt;"));

	connect(ui.hostNameEdit, &QLineEdit::textChanged, this, &securesocketclient::updateEnabledState);
	connect(ui.connectButton, &QPushButton::clicked, this, &securesocketclient::secureConnect);
	connect(ui.sendButton, &QPushButton::clicked, this, &securesocketclient::sendData);

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
	ui.sendButton->setEnabled(connected);
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
	QString input = ui.sessionInput->text();
	appendString(input + '\n');
	socket->write(input.toUtf8() + "\r\n");
	ui.sessionInput->clear();
}

void securesocketclient::socketError(QAbstractSocket::SocketError error)
{
	if (handlingSocketError)
		return;

	handlingSocketError = true;
	//QMessageBox::critical(this, tr("Connection error"), socket->errorString());
	handlingSocketError = false;
}

void securesocketclient::sslErrors(const QList<QSslError> &errors)
{
	QDialog errorDialog(this);
	Ui::sslerrors ui;
	ui.setupUi(&errorDialog);
	connect(ui.certificateChainButton, &QPushButton::clicked, this, &securesocketclient::displayCertificateInfo);

	for (const auto &error : errors)
	{
		ui.sslErrorList->addItem(error.errorString());

		executingDialog = true;
		if (errorDialog.exec() == QDialog::Accepted)
			socket->ignoreSslErrors();
		executingDialog = false;

		if (socket->state() != QAbstractSocket::ConnectedState)
			socketStateChanged(socket->state());
	}
}

void securesocketclient::displayCertificateInfo()
{
	certificateinfo info;
	info.setCertificateChain(socket->peerCertificateChain());
	info.exec();
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
