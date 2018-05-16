#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class MainWindow : public QMainWindow {
    Q_OBJECT
    void saveSettings();
    void loadSettings();
    void registerQmlType();
    bool m_closeApp;
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void closeEvent(QCloseEvent *event);
    void launchApp();
public slots:
    void closeApp();
};

#endif // MAINWINDOW_H
