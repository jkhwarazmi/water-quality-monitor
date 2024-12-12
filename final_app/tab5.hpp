#include <QWidget>
#include <QLabel>
#include <QComboBox>
#include <QFrame>
#include <QChartView>
#include <QBarSeries>
#include <QBarSet>
#include <QValueAxis>
#include <QBarCategoryAxis>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QDebug>
#include "model.hpp"

class QLabel;
class QChartView;
class WaterModel; 

class Tab5 : public QWidget
{
    Q_OBJECT

public:
    Tab5(WaterModel* model, QWidget* parent = nullptr);
    void updatePollutantList(); 
    void updateLocationList(); 
    void updateComplianceStatus(const QString& pollutant, const QString& location);
    void updateBarChart(); 
    void updateCharts();

private slots:
    void onPollutantSelectionChanged(int index);
    void onLocationSelectionChanged(int index);

private:
    WaterModel* waterModel;
    QLabel *title;
    QLabel* overviewLabel;
    QLabel* pollutantLabel;
    QLabel* locationLabel;
    QLabel* definitionLabel;
    QLabel* thresholdLabel;
    QLabel* currentValueLabel;

    QComboBox* pollutantDropdown;
    QComboBox* locationDropdown;

    // Compliance Widgets
    QLabel* complianceStatusLabel;
    QWidget* trafficLightWidget;
    QFrame* complianceCard;

    // For charting
    QChartView* chartView;
    QChart* chart;
    QBarSeries* barSeries;
    QHBoxLayout* complianceLayout; 
    QBarSeries *pollutantSeries;
    QChartView *pollutantChartView; 

    void createWidgets();
    void createLayout();
    void createCharts();
    void setupStyles();
    void createOverviewLabel();
    bool eventFilter(QObject* watched, QEvent* event) override;

    QChart* createTimeSeriesChart(const QString& pollutant, const QString& location);
};
