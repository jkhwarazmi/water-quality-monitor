// Implementation for the second card - Tab2

#include <QtWidgets>
#include <QtCharts>
#include "styles.hpp"
#include "tab2.hpp"

QT_CHARTS_USE_NAMESPACE

Tab2::Tab2(WaterModel* model, QWidget* parent): waterModel(model), QWidget(parent)
{
    createWidgets();
    createLayout();
    setupStyles();
  
    // Install event filter for theme changes
    qApp->installEventFilter(this);

    // Connect signals to populate data when filters change
    connect(pollutantFilter, &QComboBox::currentTextChanged, this, &Tab2::populateData);
    connect(timeRangeFilter, &QComboBox::currentTextChanged, this, &Tab2::populateData);

    // Populate initial data
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
    // Filters
    pollutantFilter = new QComboBox();
    pollutantFilter->addItems({"1,1,2-Trichloroethane", "Chloroform", "CO", "NO2", "PM2.5"});

    timeRangeFilter = new QComboBox();
    timeRangeFilter->addItems({"Last Week", "Last Month", "Last Year"});

    // Chart
    chartView = new QChartView(new QChart());
    chartView->setRenderHint(QPainter::Antialiasing);

    // Data Table
    dataTable = new QTableView();

    // Placeholder Label
    placeholder = new QLabel(tr("Pollutant Overview"));
    QFont labelFont;
    labelFont.setPointSize(20);
    labelFont.setBold(true);
    placeholder->setFont(labelFont);
    placeholder->setAlignment(Qt::AlignCenter);
    placeholder->setObjectName("placeholderLabel");
}

void Tab2::createLayout()
{
    QVBoxLayout* layout = new QVBoxLayout;

    // Filters Layout
    QHBoxLayout* filterLayout = new QHBoxLayout;
    filterLayout->addWidget(new QLabel("Pollutant:"));
    filterLayout->addWidget(pollutantFilter);
    filterLayout->addWidget(new QLabel("Time Range:"));
    filterLayout->addWidget(timeRangeFilter);

    layout->addLayout(filterLayout);

    // Chart
    layout->addWidget(chartView);

    // Data Table
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
    .arg(Styles::textColor());        // %1

    this->setStyleSheet(styleSheet);
}

void Tab2::populateData()
{
    // Example: Populate chart with dummy data
    QChart *chart = chartView->chart();
    chart->removeAllSeries();

    QLineSeries *series = new QLineSeries();
    series->append(1, 3);
    series->append(2, 6);
    series->append(3, 9);

    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->setTitle("Pollutant Levels Over Time");

    // Example: Populate table with dummy data
    QStandardItemModel *model = new QStandardItemModel();
    model->setHorizontalHeaderLabels({"Date", "Pollutant", "Concentration", "Compliance"});
    model->appendRow({new QStandardItem("2024-11-01"),
                      new QStandardItem("CO"),
                      new QStandardItem("3.5 ppm"),
                      new QStandardItem("Green")});
    model->appendRow({new QStandardItem("2024-11-02"),
                      new QStandardItem("NO2"),
                      new QStandardItem("4.1 ppm"),
                      new QStandardItem("Amber")});

    dataTable->setModel(model);
}
