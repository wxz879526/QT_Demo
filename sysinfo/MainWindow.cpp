#include "MainWindow.h"
#include <QHBoxLayout>
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , mCPUWidget(this)
    , mMemoryWidget(this)
{
    ui->setupUi(this);
    ui->centralwidget->setLayout(new QHBoxLayout());
    ui->centralwidget->layout()->addWidget(&mCPUWidget);
    ui->centralwidget->layout()->addWidget(&mMemoryWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}

