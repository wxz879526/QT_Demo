#include "MemoryWidget.h"
#include "SysInfo.h"
#include <QtCharts/QAreaSeries>

const int CHART_X_RANGE_COUNT = 50;
const int CHART_X_RANGE_MAX = CHART_X_RANGE_COUNT - 1;

MemoryWidget::MemoryWidget(QWidget *parent)
    : SysInfoWidget(parent)
    , mSeries(new QtCharts::QLineSeries(this))
    , mPositionX(0)
{
    QtCharts::QAreaSeries *areaSeries = new QtCharts::QAreaSeries(mSeries);

    QtCharts::QChart *chart = chartView().chart();
    chart->addSeries(areaSeries);
    chart->setTitle("Memory used");
    chart->createDefaultAxes();
    chart->axisX()->setVisible(false);
    chart->axisX()->setRange(0, CHART_X_RANGE_MAX);
    chart->axisY()->setRange(0, 100);
}

void MemoryWidget::updateSeries()
{
    double memoryUsed = SysInfo::instance().memoryUsed();

    mSeries->append(mPositionX++, memoryUsed);
    if (mSeries->count() > CHART_X_RANGE_COUNT)
    {
        QtCharts::QChart *chart = chartView().chart();
        chart->scroll(chart->plotArea().width()/CHART_X_RANGE_MAX, 0);
        mSeries->remove(0);
    }
}
