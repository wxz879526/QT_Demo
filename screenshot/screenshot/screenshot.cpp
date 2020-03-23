#include "screenshot.h"
#include <QLabel>
#include <QScreen>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QGuiApplication>
#include <QApplication>
#include <QWindow>
#include <QPushButton>
#include <QGroupBox>
#include <QStandardPaths>
#include <QCheckBox>
#include <QDir>
#include <QImageWriter>
#include <QFileDialog>
#include <QTimer>
#include <QMessageBox>

Screenshot::Screenshot()
	: screenshotLabel(new QLabel(this))
{
	screenshotLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	screenshotLabel->setAlignment(Qt::AlignCenter);

	const QRect screenGeometry = screen()->geometry();
	screenshotLabel->setMinimumSize(screenGeometry.width() / 8, screenGeometry.height() / 8);
	
	QVBoxLayout *mainLayout = new QVBoxLayout(this);
	mainLayout->addWidget(screenshotLabel);

	QGroupBox *optionsGroupBox = new QGroupBox(tr("Options"), this);
	delaySpinBox = new QSpinBox(optionsGroupBox);
	delaySpinBox->setSuffix(tr(" s"));
	delaySpinBox->setMaximum(60);

	connect(delaySpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this,
		&Screenshot::updateCheckBox);

	hideThisWindowCheckBox = new QCheckBox(tr("Hide This Window"), optionsGroupBox);

	QGridLayout *optionsGroupBoxLayout = new QGridLayout(optionsGroupBox);
	optionsGroupBoxLayout->addWidget(new QLabel(tr("Screenshot Delay"), this), 0, 0);
	optionsGroupBoxLayout->addWidget(delaySpinBox, 0, 1);
	optionsGroupBoxLayout->addWidget(hideThisWindowCheckBox, 1, 0, 1, 2);
	mainLayout->addWidget(optionsGroupBox);

	QHBoxLayout *buttonsLayout = new QHBoxLayout;
	newScreenshotButton = new QPushButton(tr("New Screenshot"), this);
	connect(newScreenshotButton, &QPushButton::clicked, this, &Screenshot::newScreenshot);
	buttonsLayout->addWidget(newScreenshotButton);
	
	QPushButton *saveScreenButton = new QPushButton(tr("Save Screenshot"), this);
	connect(saveScreenButton, &QPushButton::clicked, this, &Screenshot::saveScreenshot);
	buttonsLayout->addWidget(saveScreenButton);

	QPushButton *quitScreenButton = new QPushButton(tr("Quit"), this);
	quitScreenButton->setShortcut(Qt::CTRL + Qt::Key_Q);
	connect(quitScreenButton, &QPushButton::clicked, this, &QWidget::close);
	buttonsLayout->addWidget(quitScreenButton);
	buttonsLayout->addStretch();
	mainLayout->addLayout(buttonsLayout);

	shootScreen();
	delaySpinBox->setValue(5);

	setWindowTitle(tr("Screenshot"));
	resize(300, 200);
}


Screenshot::~Screenshot()
{
}

void Screenshot::resizeEvent(QResizeEvent *event)
{
	QSize scaledSize = originPixmap.size();
	scaledSize.scale(screenshotLabel->size(), Qt::KeepAspectRatio);
	if (!screenshotLabel->pixmap() || scaledSize != screenshotLabel->pixmap()->size())
		updateScreenshotLabel();
}

void Screenshot::newScreenshot()
{
	if (hideThisWindowCheckBox->isChecked())
		hide();

	newScreenshotButton->setDisabled(true);

	QTimer::singleShot(delaySpinBox->value() * 1000, this, &Screenshot::shootScreen);
}

void Screenshot::saveScreenshot()
{
	const QString format = "png";
	QString initialPath = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation);
	if (initialPath.isEmpty())
		initialPath = QDir::currentPath();

	initialPath += tr("untitiled.") + format;

	QFileDialog fileDialog(this, tr("Save As"), initialPath);
	fileDialog.setAcceptMode(QFileDialog::AcceptSave);
	fileDialog.setFileMode(QFileDialog::AnyFile);
	fileDialog.setDirectory(initialPath);
	QStringList mimeTypes;
	const QList<QByteArray> baMimeTypes = QImageWriter::supportedMimeTypes();
	for (const QByteArray &bf : baMimeTypes)
		mimeTypes.append(QLatin1String(bf));
	fileDialog.setMimeTypeFilters(mimeTypes);
	fileDialog.selectMimeTypeFilter("image/" + format);
	fileDialog.setDefaultSuffix(format);
	if (fileDialog.exec() != QDialog::Accepted)
		return;

	const QString fileName = fileDialog.selectedFiles().first();
	if (!originPixmap.save(fileName)) {
		QMessageBox::warning(this, tr("Save Error"), 
			tr("The Image Could not be saved to \"%1\".").arg(QDir::toNativeSeparators(fileName)));
	}
}

void Screenshot::shootScreen()
{
	QScreen *screen = QGuiApplication::primaryScreen();
	if (const QWindow *window = windowHandle())
		screen = window->screen();

	if (!screen)
		return;

	if (delaySpinBox->value() != 0)
		QApplication::beep();

	originPixmap = screen->grabWindow(0);
	updateScreenshotLabel();

	newScreenshotButton->setDisabled(false);
	if (hideThisWindowCheckBox->isChecked())
		show();
}

void Screenshot::updateCheckBox()
{
	if (delaySpinBox->value() == 0) {
		hideThisWindowCheckBox->setDisabled(true);
		hideThisWindowCheckBox->setChecked(false);
	}
	else {
		hideThisWindowCheckBox->setDisabled(false);
	}
}

void Screenshot::updateScreenshotLabel()
{
	screenshotLabel->setPixmap(originPixmap.scaled(screenshotLabel->size(),
		Qt::KeepAspectRatio, Qt::SmoothTransformation));
}