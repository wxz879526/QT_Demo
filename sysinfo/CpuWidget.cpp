#include "CpuWidget.h"
#include "SysInfo.h"

CpuWidget::CpuWidget(QWidget *parent)
    : SysInfoWidget(parent)
    , mSeries(new QtCharts::QPieSeries(this))
{
    mSeries->setHoleSize(0.35);
    mSeries->append("CPU Load", 30.0);
    mSeries->append("CPU Free", 70.0);

    QtCharts::QChart *chart = chartView().chart();
    chart->addSeries(mSeries);
    chart->setTitle("CPU average load");
}

void CpuWidget::updateSeries()
{
    double cpuLoadAverage = SysInfo::instance().cpuLoadAverage();
    mSeries->clear();
    mSeries->append("Load", cpuLoadAverage);
    mSeries->append("Free", 100.0 - cpuLoadAverage);
}
