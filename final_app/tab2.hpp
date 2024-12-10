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
    void updatePollutantList(); // Updates the pollutant dropdown
    void updateLocationList();  // Updates to update the location dropdown
    void updateCharts();        // Updates all charts

private slots:
    void onPollutantSelectionChanged(int index);
    void onLocationSelectionChanged(int index);
    void updateTimeSeriesChart(); // Updates to update the time series chart
    

private:
    WaterModel* waterModel;
    QComboBox* pollutantDropdown;
    QComboBox* locationDropdown;  //
    QLabel* pollutantLabel;
    QLabel* locationLabel;
    QChartView* currentChartView; //Holds current view
    QLabel *title;

    void createWidgets();
    void createLayout();
    void setupStyles();
    bool eventFilter(QObject* watched, QEvent* event) override;

    void updateLocationDropdown(const QString& pollutant);

    QChart* createTimeSeriesChart(const QString& pollutant, const QString& location);
};