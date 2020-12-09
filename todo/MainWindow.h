#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

#include<QVector>
#include"Task.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void updateStatus();

public slots:
    void addTask();
    void removeTask(Task *task);
    void statusChanged(Task *task);

private:
    Ui::MainWindow *ui;
    QVector<Task*> mTasks;
};
#endif // MAINWINDOW_H
