#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QInputDialog>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->addTaskButton, &QPushButton::clicked, this, &MainWindow::addTask);
    updateStatus();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateStatus()
{
    int completedCount = 0;
    for(auto t : mTasks)
    {
        if (t->isCompleted())
        {
            ++completedCount;
        }
    }

    int todoCount = mTasks.size() - completedCount;
    ui->statusLabel->setText(QString("Status: %1 todo / %2 completed").arg(todoCount).arg(completedCount));

}

void MainWindow::addTask()
{
    bool bOK;
    QString name = QInputDialog::getText(this, tr("Add Task"),
                                         tr("Task name"),
                                         QLineEdit::Normal,
                                         tr("Untitled task"),
                                         &bOK);
    if (bOK && !name.isEmpty())
    {
        qDebug() << "Add new task";
        Task *task = new Task(name);
        mTasks.append(task);
        ui->tasksLayout->addWidget(task);
        connect(task, &Task::removed, this, &MainWindow::removeTask);
        connect(task, &Task::statusChanged, this, &MainWindow::statusChanged);
        updateStatus();
    }
}

void MainWindow::removeTask(Task *task)
{
    mTasks.removeOne(task);
    ui->tasksLayout->removeWidget(task);
    delete task;
    updateStatus();
}

void MainWindow::statusChanged(Task *task)
{
    updateStatus();
}

