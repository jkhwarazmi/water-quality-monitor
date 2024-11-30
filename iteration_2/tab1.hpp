// Header for the first card - Dashboard
// Jamil

#include <QWidget>
#include <QtCharts>
#include "model.hpp"

class QPushButton;
class QLabel;

class Tab1: public QWidget
{
  Q_OBJECT

  public:
    Tab1(WaterModel *model, QWidget *parent = nullptr);

  signals:
    void reloadRequested();

  private slots:
    void initCharts();

  private:
    WaterModel* waterModel;
    QPushButton *reloadButton;
    QLabel *headingLabel;

    // Charts
    QChartView *complianceChart;
    QChartView *purposeChart;
    QChartView *pollutantsChart;
    QChartView *materialsChart;
    QChartView *categoriesChart;

    void createWidgets();
    void createLayout();
    void setupStyles();
    bool eventFilter(QObject *watched, QEvent *event) override;

    // Chart creation methods
    QChartView *createComplianceStatusChart();
    QChartView *createPurposeChart();
    QChartView *createPollutantsChart();
    QChartView *createMaterialsChart();
    QChartView *createCategoriesChart();
};