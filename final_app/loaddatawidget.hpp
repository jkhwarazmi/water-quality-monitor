// The data setup page

#pragma once

#include <QWidget>

class QLabel;
class QPushButton;
class QComboBox;

class LoadDataWidget : public QWidget
{
  Q_OBJECT

  public:
    LoadDataWidget(const QString& initialPath, QWidget* parent = nullptr);
    void updatePath(const QString& path);
    QString getPath() const { return currentPath; }
    void setDataLoaded(bool loaded);

  signals:
    void dataFileSelected();
    void loadRequested();
    void languageChanged(QString);
    void cancelRequested();

  private:
    QString currentPath;
    bool dataLoaded = false;
    QLabel *titleLabel;
    QLabel *pathLabel;
    QLabel *languageLabel;
    QPushButton *selectButton;
    QPushButton *loadButton;
    QPushButton *cancelButton;
    QComboBox *languageComboBox;

    void createWidgets();
    void createLayout();
    void setupStyles();
    bool eventFilter(QObject *watched, QEvent *event) override;
};
