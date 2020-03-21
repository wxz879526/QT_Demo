#include "stdafx.h"
#include "certificateinfo.h"

certificateinfo::certificateinfo(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	connect(ui.certificationPathView, QOverload<int>::of(&QComboBox::currentIndexChanged),
		this, &certificateinfo::updateCertificateInfo);
}

certificateinfo::~certificateinfo()
{
	
}

void certificateinfo::setCertificateChain(const QList<QSslCertificate> &chain)
{
	certificateChain = chain;

	ui.certificationPathView->clear();
	for (int i = 0; i < certificateChain.size(); ++i)
	{
		const QSslCertificate &cert = certificateChain.at(i);
		ui.certificationPathView->addItem(tr("%1%2 (%3)")
			.arg(!i ? QString() : tr("Issued by:"))
			.arg(cert.subjectInfo(QSslCertificate::Organization).join(QLatin1Char(' ')))
			.arg(cert.subjectInfo(QSslCertificate::CommonName).join(QLatin1Char(' '))));
	}
	ui.certificationPathView->setCurrentIndex(0);
}

void certificateinfo::updateCertificateInfo(int index)
{
	ui.certificateInfoView->clear();
	if (index >= 0 && index < certificateChain.size())
	{
		const QSslCertificate &cert = certificateChain.at(index);
		QStringList lines;
		lines << tr("Organization: %1").arg(cert.subjectInfo(QSslCertificate::Organization).join(QLatin1Char(' ')))
			<< tr("Subunit: %1").arg(cert.subjectInfo(QSslCertificate::OrganizationalUnitName).join(QLatin1Char(' ')))
			<< tr("Country: %1").arg(cert.subjectInfo(QSslCertificate::CountryName).join(QLatin1Char(' ')))
			<< tr("Locality: %1").arg(cert.subjectInfo(QSslCertificate::LocalityName).join(QLatin1Char(' ')))
			<< tr("State/Province: %1").arg(cert.subjectInfo(QSslCertificate::StateOrProvinceName).join(QLatin1Char(' ')))
			<< tr("Common Name: %1").arg(cert.subjectInfo(QSslCertificate::CommonName).join(QLatin1Char(' ')))
			<< QString()
			<< tr("Issuer Organization: %1").arg(cert.issuerInfo(QSslCertificate::Organization).join(QLatin1Char(' ')))
			<< tr("Issuer Unit Name: %1").arg(cert.issuerInfo(QSslCertificate::OrganizationalUnitName).join(QLatin1Char(' ')))
			<< tr("Issuer Country: %1").arg(cert.issuerInfo(QSslCertificate::CountryName).join(QLatin1Char(' ')))
			<< tr("Issuer Locality: %1").arg(cert.issuerInfo(QSslCertificate::LocalityName).join(QLatin1Char(' ')))
			<< tr("Issuer State/Province: %1").arg(cert.issuerInfo(QSslCertificate::StateOrProvinceName).join(QLatin1Char(' ')))
			<< tr("Issuer Common Name: %1").arg(cert.issuerInfo(QSslCertificate::CommonName).join(QLatin1Char(' ')));

		for (const auto &line : lines)
		{
			ui.certificateInfoView->addItem(line);
		}
	}
}