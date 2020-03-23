#pragma once

#include <QPixmap>
#include <QWidget>

QT_BEGIN_NAMESPACE
class QCheckBox;
class QGridLayout;
class QGroupBox;
class QHBoxLayout;
class QPushButton;
class QLabel;
class QSpinBox;
QT_END_NAMESPACE

class Screenshot : public QWidget
{
public:
	Screenshot();
	~Screenshot();

protected:
	void resizeEvent(QResizeEvent *event) override;

private slots:
	void newScreenshot();
	void saveScreenshot();
	void shootScreen();
	void updateCheckBox();

private:
	void updateScreenshotLabel();

	QPixmap originPixmap;
	QLabel *screenshotLabel;
	QSpinBox *delaySpinBox;
	QCheckBox *hideThisWindowCheckBox;
	QPushButton *newScreenshotButton;
};
