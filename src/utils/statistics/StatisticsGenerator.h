#pragma once

#include "TrenchCoat.h"

#include <QtCharts/QChart>

class StatisticGenerator {
private:
    std::map<char,int> itemCount, totalCount;
    std::map<std::string, int> coatsByModel;
public:
    explicit StatisticGenerator(std::vector<TrenchCoat>& data);

    static QtCharts::QChart* generateBarChart(std::map<char,int>& data, std::string chartName);
    QtCharts::QChart* itemBarChart();
    QtCharts::QChart* totalBarChart();
    QtCharts::QChart* totalPieChart();
};
