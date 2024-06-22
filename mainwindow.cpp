// mainwindow.cpp
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QSysInfo>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QStorageInfo>
#include <QProcess>
#include <QtWidgets>

QMenu *home;
QMenu *viewMenu;
QAction *activeProcesses;
QAction *allProcesses;
QAction *myProcesses;
QAction *systemView;
QLabel *mainInfoLabel;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QWidget *widget = new QWidget;
    this->setCentralWidget(widget);

//    QWidget *topFiller = new QWidget;
//    topFiller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);


    mainInfoLabel = new QLabel(tr("information"));
    mainInfoLabel->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    mainInfoLabel->setAlignment(Qt::AlignCenter);


    QVBoxLayout *layout = new QVBoxLayout;
    layout->setContentsMargins(5,5,5,5);
    layout->addWidget(mainInfoLabel);
//    layout->addWidget(topFiller);

    widget->setLayout(layout);

    createActions();
    createMenus();
    setWindowTitle(tr("System Monitor"));
    setMinimumSize(160,160);
    resize(480, 320);

    // Call the function to display processes
    displayProcesses();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::displayProcesses()
{
    QDir procDir("/proc");
    QFileInfoList entries = procDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);

    foreach (QFileInfo entry, entries) {
        QString processId = entry.fileName();
        QFile commFile("/proc/" + processId + "/comm");
        QFile statFile("/proc/" + processId + "/stat");
        QFile statusFile("/proc/" + processId + "/status");

        if (commFile.open(QIODevice::ReadOnly | QIODevice::Text) &&
            statFile.open(QIODevice::ReadOnly | QIODevice::Text) &&
            statusFile.open(QIODevice::ReadOnly | QIODevice::Text)) {

            QTextStream commStream(&commFile);
            QTextStream statStream(&statFile);
            QTextStream statusStream(&statusFile);

            QString processName = commStream.readLine().trimmed();

            // Extracting Parent PID and State from /proc/[pid]/stat
            QString statLine = statStream.readLine();
            QStringList statInfo = statLine.split(' ');
            QString parentPid = statInfo[3];
            QString state = statInfo[2];

            // Extracting Owner from /proc/[pid]/status
            QString owner;
            while (!statusStream.atEnd()) {
                QString line = statusStream.readLine();
                if (line.startsWith("Uid:")) {
                    QStringList uidInfo = line.mid(5).trimmed().split('\t');
                    owner = uidInfo[0];
                    break;
                }
            }

            qDebug() << "PID:" << processId
                     << "Parent PID:" << parentPid
                     << "Name:" << processName
                     << "State:" << state
                     << "Owner:" << owner;

            // You can further process and display information as needed

            commFile.close();
            statFile.close();
            statusFile.close();
        }
    }
}

#ifdef Q_OS_LINUX
#include <sys/sysinfo.h>
#endif

#ifdef Q_OS_WIN
#include <windows.h>
#endif

qint64 getTotalVirtualMemory()
{
    qint64 totalMemory = 0;

#ifdef Q_OS_LINUX
    struct sysinfo info;
    if (sysinfo(&info) == 0)
    {
        totalMemory = info.totalram;
        totalMemory *= info.mem_unit;
    }
#endif

#ifdef Q_OS_WIN
    MEMORYSTATUSEX status;
    status.dwLength = sizeof(status);
    if (GlobalMemoryStatusEx(&status))
    {
        totalMemory = status.ullTotalVirtual;
    }
#endif

    return totalMemory;
}


QString MainWindow::displaySystemInfo()
{
        // Example: Print basic system information to the terminal
    // Example: Print basic system information to the terminal
    // Example: Print basic system information to the terminal
    // Example: Print basic system information to the terminal
    // Example: Print basic system information to the terminal


      qDebug() << "System Information:";
      QString Architecture = QSysInfo::currentCpuArchitecture(); // ARCHITECTURE
      qDebug() << "Architecture:" << Architecture;
      QString os_release_version= QSysInfo::prettyProductName(); // OS RELEASE VERISON
      qDebug() << "OS Release Version:" << os_release_version;
      QString kernel_version = QSysInfo::prettyProductName(); // KERNEL VERSION
      qDebug() << "Kernel Version:" << kernel_version;
      qDebug() << "Memory Information:";
      QString total_ram = QString::number(getTotalVirtualMemory() / (1024.0 * 1024.0 * 1024.0)) + "GB";
      qDebug() << "Total RAM:" << total_ram;
      // Get disk space information
      qDebug() << "User Disk Space Information:";

      QStorageInfo systemDrive("/usr");
      QString user_disk_info = systemDrive.rootPath() + "\n" + QString::number(systemDrive.bytesTotal() / (1024.0 * 1024.0 * 1024.0))
              + "GB\n" + QString::number(systemDrive.bytesAvailable() / (1024.0 * 1024.0 * 1024.0)) + "GB";
      qDebug() << "Drive:" << systemDrive.rootPath();
      qDebug() << "Total Space:" << systemDrive.bytesTotal() / (1024.0 * 1024.0 * 1024.0) << "GB";
      qDebug() << "Available Space:" << systemDrive.bytesAvailable() / (1024.0 * 1024.0 * 1024.0) << "GB";
      qDebug() << "System Disk Space Information:";

      qDebug() << "System Disk Space Information:";

      QStorageInfo systemDrive2("/");
      qDebug() << "Drive:" << systemDrive2.rootPath();
      qDebug() << "Total Space:" << systemDrive2.bytesTotal() / (1024.0 * 1024.0 * 1024.0) << "GB";
      qDebug() << "Available Space:" << systemDrive2.bytesAvailable() / (1024.0 * 1024.0 * 1024.0) << "GB";
      QString final_system_info =  "Architecture: " + Architecture + "\nOs Release version: " + os_release_version + "\nKernel version: " + kernel_version + "\nTotal ram: " + total_ram + "\n User Disk Info: \n" + user_disk_info;
      return final_system_info;

    }

void MainWindow::createMenus()
{
    home = menuBar()->addMenu(tr("&Home"));
    home->addAction(systemView);

    viewMenu = menuBar()->addMenu(tr("&View"));
    viewMenu->addAction(activeProcesses);
    viewMenu->addAction(allProcesses);
    viewMenu->addAction(myProcesses);

}

void MainWindow::createActions()
{

    systemView = new QAction(tr("&System View"), this);
    systemView->setStatusTip(tr("Look at all active proccesses"));
    connect(systemView, &QAction::triggered, this, &MainWindow::viewSystemHome);

    activeProcesses = new QAction(tr("&Active Process"), this);
    activeProcesses->setStatusTip(tr("Look at all active proccesses"));
    connect(activeProcesses, &QAction::triggered, this, &MainWindow::activeProccesses);

    allProcesses = new QAction(tr("&All Process"), this);
    allProcesses->setStatusTip(tr("Look at all proccesses"));
    connect(allProcesses, &QAction::triggered, this, &MainWindow::allProcessesFunction);

    myProcesses = new QAction(tr("&My Process"), this);
    myProcesses->setStatusTip(tr("Look at my proccesses"));
    connect(myProcesses, &QAction::triggered, this, &MainWindow::myProcessesFunction);

}


void MainWindow::activeProccesses()
{
    qDebug() << "active proccesses";
}

void MainWindow::allProcessesFunction()
{
    qDebug() << "all proccesses";
}

void MainWindow::myProcessesFunction()
{
    qDebug() << "View My Proccesses";
}

void MainWindow::viewSystemHome()
{
    QString sys_info = displaySystemInfo();
    mainInfoLabel->setText(sys_info);
}
