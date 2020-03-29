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
#include <QMessageBox>
#include <QDesktopServices>
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
	url = requestedUrl;
	httpRequestAborted = false;

	reply = qnam.get(QNetworkRequest(url));
	connect(reply, &QNetworkReply::finished, this, &http::httpFinished);
	connect(reply, &QIODevice::readyRead, this, &http::httpReadyRead);

	ProgressDialg *progressDialog = new ProgressDialg(url, this);
	progressDialog->setAttribute(Qt::WA_DeleteOnClose);
	connect(progressDialog, &ProgressDialg::canceled, this, &http::cancelDownload);
	connect(reply, &QNetworkReply::downloadProgress, progressDialog, &ProgressDialg::networkReplyProgress);
	connect(reply, &QNetworkReply::finished, progressDialog, &ProgressDialg::hide);
	progressDialog->show();

	statusLabel->setText(tr("Downloading %1...").arg(url.toString()));
}

void http::downloadFile()
{
	const QString urlSpec = urlLineEdit->text().trimmed();
	if (urlSpec.isEmpty())
		return;

	const QUrl &newUrl = QUrl::fromUserInput(urlSpec);
	if (!newUrl.isValid())
	{
		QMessageBox::information(this, tr("Error"),
			tr("Invalid URL: %1: %2").arg(urlSpec, newUrl.errorString()));
		return;
	}

	QString fileName = newUrl.fileName();
	if (fileName.isEmpty())
		fileName = defaultFileLineEdit->text().trimmed();
	if (fileName.isEmpty())
		fileName = defaultFileName;

	QString downloadDirectory = QDir::cleanPath(downloadDirectoryLineEdit->text().trimmed());
	bool useDirectory = !downloadDirectory.isEmpty() && QFileInfo(downloadDirectory).isDir();
	if (useDirectory)
		fileName.prepend(downloadDirectory + QDir::separator());

	if (QFile::exists(fileName)) {
		if (QMessageBox::question(this, tr("Overwrite Existing File"),
			tr("There already exists a file called %1%2."
				" Overwrite?")
			.arg(fileName,
				useDirectory
				? QString()
				: QStringLiteral(" in the current directory")),
			QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::No) {
			return;
		}

		QFile::remove(fileName);
	}

	file = openFileForWrite(fileName);
	if (!file)
		return;

	downloadButton->setEnabled(false);

	startRequest(newUrl);
}

void http::cancelDownload()
{
	statusLabel->setText(tr("Download canceled."));
	httpRequestAborted = true;
	reply->abort();
	downloadButton->setEnabled(true);
}

void http::httpFinished()
{
	QFileInfo fi;
	if (file) {
		fi.setFile(file->fileName());
		file->close();
		file->reset();
	}

	if (httpRequestAborted) {
		reply->deleteLater();
		reply = nullptr;
		return;
	}

	if (reply->error()) {
		QFile::remove(fi.absoluteFilePath());
		statusLabel->setText(tr("Downloading failed:\n%1.").arg(reply->errorString()));
		downloadButton->setEnabled(true);
		reply->deleteLater();
		reply = nullptr;
		return;
	}

	const QVariant redirectionTarget = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
	reply->deleteLater();
	reply = nullptr;

	if (!redirectionTarget.isNull()) {
		const QUrl redirectedUrl = url.resolved(redirectionTarget.toUrl());
		if (QMessageBox::question(this, tr("Redirect"),
			tr("Redirect to %1 ?").arg(redirectionTarget.toString()),
			QMessageBox::Yes | QMessageBox::No) == QMessageBox::No) {
			QFile::remove(fi.absoluteFilePath());
			downloadButton->setEnabled(true);
			statusLabel->setText(tr("Download failed:\nRedirect rejected."));
			return;
		}

		file = openFileForWrite(fi.absoluteFilePath());
		if (!file) {
			downloadButton->setEnabled(true);
			return;
		}
		startRequest(redirectedUrl);
		return;
	}

	statusLabel->setText(tr("Downloaded %1 bytes to %2\nin\n%3")
		.arg(fi.size()).arg(fi.fileName(), QDir::toNativeSeparators(fi.absolutePath())));
	if (launchCheckBox->isChecked())
		QDesktopServices::openUrl(QUrl::fromLocalFile(fi.absoluteFilePath()));
	downloadButton->setEnabled(true);
}

void http::httpReadyRead()
{
	if (file)
		file->write(reply->readAll());
}

void http::enableDownloadButton()
{
	downloadButton->setEnabled(!urlLineEdit->text().isEmpty());
}

void http::slotAuthenticationRequired(QNetworkReply *, QAuthenticator *authenticator)
{

}

void http::sslErrors(QNetworkReply *, const QList<QSslError> &errs)
{
	QString errorString;
	for (const QSslError &error : errs) {
		if (!errorString.isEmpty())
			errorString += '\n';
		errorString += error.errorString();
	}

	if (QMessageBox::warning(this, tr("SSL Errors"),
		tr("One or more SSL errors has occurred:\n%1").arg(errorString),
		QMessageBox::Ignore | QMessageBox::Abort) == QMessageBox::Ignore) {
		reply->ignoreSslErrors();
	}
}

std::unique_ptr<QFile> http::openFileForWrite(const QString &fileName)
{
	std::unique_ptr<QFile> file(new QFile(fileName));
	if (!file->open(QIODevice::WriteOnly)) {
		QMessageBox::information(this, tr("Error"),
			tr("Unable to save the file: %1: %2").arg(fileName, file->errorString()));
		return nullptr;
	}

	return file;
}
