#ifndef MEMORYWIDGET_H
#define MEMORYWIDGET_H

#include "SysInfoWidget.h"
#include <QtCharts/QLineSeries>


class MemoryWidget : public SysInfoWidget
{
    Q_OBJECT
public:
    MemoryWidget(QWidget *parent);

    // SysInfoWidget interface
protected slots:
    void updateSeries();

private:
    QtCharts::QLineSeries *mSeries;
    qint64 mPositionX;
};

#endif // MEMORYWIDGET_H
