#pragma once

#include <QProgressDialog>
#include <QNetworkAccessManager>
#include <QUrl>
#include <memory>

QT_BEGIN_NAMESPACE
class QFile;
class QLabel;
class QLineEdit;
class QPushButton;
class QSslError;
class QAuthenticator;
class QNetworkReply;
class QCheckBox;
QT_END_NAMESPACE

class ProgressDialg : QProgressDialog {
	Q_OBJECT

public:
	explicit ProgressDialg(const QUrl &url, QWidget *parent = nullptr);
	~ProgressDialg();

public slots:
	void networkReplyProgress(qint64 bytesRead, qint64 totalBytes);
};

class http : public QDialog
{
	Q_OBJECT

public:
	explicit http(QWidget *parent = Q_NULLPTR);
	~http();

	void startRequest(const QUrl &requestedUrl);

private slots:
	void downloadFile();
	void cancelDownload();
	void httpFinished();
	void httpReadyRead();
	void enableDownloadButton();
	void slotAuthenticationRequired(QNetworkReply *, QAuthenticator *authenticator);

#ifndef QT_NO_SSL
	void sslErrors(QNetworkReply *, const QList<QSslError> &errs);
#endif

private:
	std::unique_ptr<QFile> openFileForWrite(const QString &fileName);

	QLabel *statusLabel;
	QLineEdit *urlLineEdit;
	QPushButton *downloadButton;
	QCheckBox *launchCheckBox;
	QLineEdit *defaultFileLineEdit;
	QLineEdit *downloadDirectoryLineEdit;

	QUrl url;
	QNetworkAccessManager qnam;
	QNetworkReply *reply;
	std::unique_ptr<QFile> file;
	bool httpRequestAborted;
};
