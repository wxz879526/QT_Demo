#ifndef CPUWIDGET_H
#define CPUWIDGET_H

#include "SysInfoWidget.h"
#include <QtCharts/QPieSeries>


class CpuWidget : public SysInfoWidget
{
    Q_OBJECT
public:
    CpuWidget(QWidget *parent = nullptr);

    // SysInfoWidget interface
protected slots:
    void updateSeries() override;

private:
    QtCharts::QPieSeries* mSeries;
};

#endif // CPUWIDGET_H
