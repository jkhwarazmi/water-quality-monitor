// Implementation for the second card - Pollutant Overview
// Nicolas

#include <QtWidgets>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include "styles.hpp"
#include "tab2.hpp"

using namespace QtCharts;

Tab2::Tab2(WaterModel *model, QWidget *parent)
    : waterModel(model), QWidget(parent)
{
    createWidgets();
    createLayout();
    setupStyles();

    qApp->installEventFilter(this);

    connect(pollutantFilter, &QComboBox::currentTextChanged, this, &Tab2::populateData);
    connect(timeRangeFilter, &QComboBox::currentTextChanged, this, &Tab2::populateData);

    populateData();
}

bool Tab2::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == qApp && event->type() == QEvent::ApplicationPaletteChange) {
        setupStyles();
        return true;
    }
    return QWidget::eventFilter(watched, event);
}

void Tab2::createWidgets()
{
    pollutantFilter = new QComboBox(this);
    pollutantFilter->addItems({"1,1,2-Trichloroethane", "Chloroform", "CO", "NO2", "PM2.5"});

    timeRangeFilter = new QComboBox(this);
    timeRangeFilter->addItems({"Last Week", "Last Month", "Last Year"});

    chartView = new QChartView(new QChart(), this);
    chartView->setRenderHint(QPainter::Antialiasing);

    dataTable = new QTableView(this);

    placeholder = new QLabel(tr("Pollutant Overview"), this);
    QFont labelFont;
    labelFont.setPointSize(20);
    labelFont.setBold(true);
    placeholder->setFont(labelFont);
    placeholder->setAlignment(Qt::AlignCenter);
    placeholder->setObjectName("placeholderLabel");
}

void Tab2::createLayout()
{
    QVBoxLayout *layout = new QVBoxLayout;

    QHBoxLayout *filterLayout = new QHBoxLayout;
    filterLayout->addWidget(new QLabel("Pollutant:", this));
    filterLayout->addWidget(pollutantFilter);
    filterLayout->addWidget(new QLabel("Time Range:", this));
    filterLayout->addWidget(timeRangeFilter);

    layout->addLayout(filterLayout);
    layout->addWidget(chartView);
    layout->addWidget(dataTable);

    layout->setSpacing(20);
    layout->setContentsMargins(30, 30, 30, 30);

    setLayout(layout);
}

void Tab2::setupStyles()
{
    QString styleSheet = QString(R"(
        QWidget {
            background-color: transparent;
            color: %1;
        }

        QLabel#placeholderLabel {
            font-size: 20px;
            font-weight: bold;
            color: %1;
        }
    )")
    .arg(Styles::textColor());

    setStyleSheet(styleSheet);
}

void Tab2::populateData()
{
    QChart *chart = chartView->chart();
    chart->removeAllSeries();

    QLineSeries *series = new QLineSeries();
    series->append(0, 0);
    series->append(1, 3);
    series->append(2, 5);

    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->setTitle("Pollutant Levels Over Time");

    QStandardItemModel *model = new QStandardItemModel();
    model->setHorizontalHeaderLabels({"Date", "Pollutant", "Concentration", "Compliance"});
    model->appendRow({new QStandardItem("2024-01-01"),
                      new QStandardItem("CO"),
                      new QStandardItem("3.5 ppm"),
                      new QStandardItem("Green")});
    model->appendRow({new QStandardItem("2024-01-02"),
                      new QStandardItem("NO2"),
                      new QStandardItem("5.1 ppm"),
                      new QStandardItem("Amber")});

    dataTable->setModel(model);
}