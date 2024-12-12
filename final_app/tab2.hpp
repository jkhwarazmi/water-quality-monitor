// Header for second card - Pollutant Overview
// Nicolas

#include <QWidget>
#include <QComboBox>
#include <QtCharts>
#include "model.hpp"

class QLabel;
class QChartView;

class Tab2 : public QWidget
{
    Q_OBJECT

public:
    Tab2(WaterModel* model, QWidget* parent = nullptr);
    void updatePollutantList(); 
    void updateLocationList();  
    void updateCharts();   

private slots:
    void onPollutantSelectionChanged(int index);
    void onLocationSelectionChanged(int index);
    void updateTimeSeriesChart(); 
    

private:
    WaterModel* waterModel;
    QComboBox* pollutantDropdown;
    QComboBox* locationDropdown;
    QLabel* pollutantLabel;
    QLabel* locationLabel;
    QChartView* currentChartView;
    QLabel *title;
    QLabel *description;

    void createWidgets();
    void createLayout();
    void setupGeneralStyles();
    void setupComboBoxStyles(); 
    void setupStyles();
    bool eventFilter(QObject* watched, QEvent* event) override;

    void updateLocationDropdown(const QString& pollutant);

    QChart* createTimeSeriesChart(const QString& pollutant, const QString& location);
};