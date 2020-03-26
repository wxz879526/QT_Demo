#include "http.h"
#include <QLabel>
#include <QPushButton>
#include <QNetworkReply>
#include <QLineEdit>
#include <QCheckBox>
#include <QFormLayout>
#include <QStandardPaths>
#include <QFileInfo>
#include <QAuthenticator>
#include <QDir>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QDialogButtonBox>

#ifndef QT_NO_SSL
const char defaultUrl[] = "https://www.qt.io/";
#else
const char defaultUrl[] = "http://www.qt.io/";
#endif
const char defaultFileName[] = "index.html";

ProgressDialg::ProgressDialg(const QUrl &url, QWidget *parent /* = nullptr */)
	: QProgressDialog(parent)
{
	setWindowTitle(tr("Download Progress"));
	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
	setLabelText(tr("Downloading %1").arg(url.toDisplayString()));
	setMinimum(0);
	setValue(0);
	setMinimumDuration(0);
	setMinimumSize(QSize(400, 75));
}

ProgressDialg::~ProgressDialg()
{

}

void ProgressDialg::networkReplyProgress(qint64 bytesRead, qint64 totalBytes)
{
	setMaximum(totalBytes);
	setValue(bytesRead);
}

http::http(QWidget *parent /* = Q_NULLPTR */)
	: QDialog(parent)
	, statusLabel(new QLabel(tr("Please Enter the url to download"), this))
	, urlLineEdit(new QLineEdit(defaultUrl))
	, downloadButton(new QPushButton(tr("download")))
	, launchCheckBox(new QCheckBox(tr("Lauch file")))
	, defaultFileLineEdit(new QLineEdit(defaultFileName))
	, downloadDirectoryLineEdit(new QLineEdit)
	, reply(nullptr)
	, file(nullptr)
	, httpRequestAborted(false)
{
	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
	setWindowTitle(tr("HTTP"));

	connect(&qnam, &QNetworkAccessManager::authenticationRequired,
		this, &http::slotAuthenticationRequired);

#ifndef QT_NO_SSL
	connect(&qnam, &QNetworkAccessManager::sslErrors, this,
		&http::sslErrors);
#endif

	QFormLayout *formLayout = new QFormLayout();

	urlLineEdit->setClearButtonEnabled(true);
	connect(urlLineEdit, &QLineEdit::textChanged,
		this, &http::enableDownloadButton);
	formLayout->addRow(tr("&URL:"), urlLineEdit);

	QString downloadDirectory = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
	if (downloadDirectory.isEmpty() || !QFileInfo(downloadDirectory).isDir())
		downloadDirectory = QDir::currentPath();

	downloadDirectoryLineEdit->setText(QDir::toNativeSeparators(downloadDirectory));
	formLayout->addRow(tr("&Download directory:"), downloadDirectoryLineEdit);
	formLayout->addRow(tr("Default &File:"), defaultFileLineEdit);
	launchCheckBox->setChecked(true);
	formLayout->addRow(launchCheckBox);

	QVBoxLayout *mainLayout = new QVBoxLayout(this);
	mainLayout->addLayout(formLayout);

	mainLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Ignored, QSizePolicy::MinimumExpanding));

	statusLabel->setWordWrap(true);
	mainLayout->addWidget(statusLabel);
	downloadButton->setDefault(true);
	connect(downloadButton, &QPushButton::clicked,
		this, &http::downloadFile);
	QPushButton *quitButton = new QPushButton(tr("Quit"));
	quitButton->setAutoDefault(false);
	connect(quitButton, &QPushButton::clicked,
		this, &QWidget::close);
	QDialogButtonBox *buttonBox = new QDialogButtonBox;
	buttonBox->addButton(downloadButton, QDialogButtonBox::ActionRole);
	buttonBox->addButton(quitButton, QDialogButtonBox::RejectRole);
	mainLayout->addWidget(buttonBox);

	setLayout(mainLayout);

	urlLineEdit->setFocus();
}

http::~http()
{

}

void http::startRequest(const QUrl &requestedUrl)
{

}

void http::downloadFile()
{

}

void http::cancelDownload()
{

}

void http::httpFinished()
{

}

void http::httpReadyRead()
{

}

void http::enableDownloadButton()
{

}

void http::slotAuthenticationRequired(QNetworkReply *, QAuthenticator *authenticator)
{

}

void http::sslErrors(QNetworkReply *, const QList<QSslError> &errs)
{

}

std::unique_ptr<QFile> http::openFileForWrite(const QString &fileName)
{
	return nullptr;
}
