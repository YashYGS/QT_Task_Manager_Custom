

// mainwindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void displayProcesses();
    QString displaySystemInfo();
    void createMenus();
    void createActions();
    void activeProccesses();
    void allProcessesFunction();
    void myProcessesFunction();
    void viewSystemHome();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

