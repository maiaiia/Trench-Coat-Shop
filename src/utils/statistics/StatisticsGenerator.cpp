#include "StatisticsGenerator.h"

#include <QtWidgets/QApplication>
#include <qaction.h>

#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QPieSeries>

StatisticGenerator::StatisticGenerator(std::vector<TrenchCoat> &data) {
    for (const auto& coat: data) {
        this->itemCount[coat.getSize()]++;
        this->totalCount[coat.getSize()] += coat.getQuantity();
        coatsByModel[coat.getName()] += coat.getQuantity();
    }
}

QtCharts::QChart *StatisticGenerator::generateBarChart(std::map<char, int> &data, std::string chartName) {
    //initialise data
    auto barSet = new QtCharts::QBarSet{""};
    *barSet << data['S'] << data['M'] << data['L'];

    auto *series = new QtCharts::QBarSeries{};
    series->append(barSet);
    series->setLabelsVisible(true);

    //configure axes
    auto axisX = new QtCharts::QBarCategoryAxis();
    axisX->setLabelsVisible(true);
    axisX->append("S"); axisX->append("M"); axisX->append("L");

    auto axisY = new QtCharts::QValueAxis();
    axisY->setLabelsVisible(true);
    axisY->setMin(0);
    axisY->setMax(std::max(data['S'], std::max(data['M'], data['L'])));
    axisY->setTickInterval(1);
    axisY->applyNiceNumbers();
    axisY->setLabelFormat("%.0f");

    //create chart
    auto chart = new QtCharts::QChart();
    chart->setTheme(QtCharts::QChart::ChartThemeBlueNcs);
    chart->setMinimumSize(QSizeF{400, 400});

    auto font = QFont();
    font.setPixelSize(20);
    chart->setTitleFont(font);
    chart->setTitle(QString(chartName.c_str()));

    chart->createDefaultAxes();
    chart->addSeries(series);

    chart->setAxisX(axisX);
    chart->setAxisY(axisY);
    series->attachAxis(axisY);
    chart->legend()->setVisible(false);
    return chart;
}


QtCharts::QChart* StatisticGenerator::itemBarChart() {
    return generateBarChart(this->itemCount, "Trench Coat Models per Size");
}

QtCharts::QChart* StatisticGenerator::totalBarChart() {
    return generateBarChart(this->totalCount, "Total Trench Coats per Size");
}

QtCharts::QChart *StatisticGenerator::totalPieChart() {
    //init data
    auto *series = new QtCharts::QPieSeries{};
    for (const auto& item: this->coatsByModel)
        series->append(QString(item.first.c_str()), item.second);
    series->setLabelsVisible(false);
    //series->setLabelsPosition(QtCharts::QPieSlice::LabelInsideHorizontal);

    auto* chart = new QtCharts::QChart();
    chart->setMinimumSize(500,400);
    auto font = QFont();
    font.setPixelSize(40);
    chart->setTitleFont(font);
    chart->setTitle("Trench Coats by Style");
    chart->setTheme(QtCharts::QChart::ChartThemeBlueNcs);
    chart->addSeries(series);
    /*
    for (auto slice: series->slices()) {
        auto oldLabel = slice->label();
        slice->setLabel(QString("%1%").arg(100* slice->percentage(), 0, 'f', 1));
    }
    */

    chart->legend()->setAlignment(Qt::AlignRight);
    //chart->legend()->hide();

    return chart;
}

