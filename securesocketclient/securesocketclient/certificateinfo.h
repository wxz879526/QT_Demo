#pragma once

#include <QDialog>
#include "ui_certificateinfo.h"
#include <QSslCertificate>

class certificateinfo : public QDialog
{
	Q_OBJECT

public:
	certificateinfo(QWidget *parent = Q_NULLPTR);
	~certificateinfo();

	void setCertificateChain(const QList<QSslCertificate> &chain);

private slots:
	void updateCertificateInfo(int index);

private:
	Ui::certificateinfo ui;
	QList<QSslCertificate> certificateChain;
};
