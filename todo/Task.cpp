#include "Task.h"
#include <QInputDialog>
#include <QDebug>
#include "ui_Task.h"

Task::Task(const QString &name, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Task)
{
    ui->setupUi(this);
    setName(name);

    connect(ui->editButton, &QPushButton::clicked, this, &Task::rename);
    connect(ui->removeButton, &QPushButton::clicked, [this, name]{
        qDebug() << "Tring to remove task " << name;
         emit removed(this);
    });

    connect(ui->checkBox, &QCheckBox::toggled, this, &Task::checked);
}

Task::~Task()
{
    delete ui;
}

void Task::setName(const QString &name)
{
    ui->checkBox->setText(name);
}

QString Task::name() const
{
    return ui->checkBox->text();
}

bool Task::isCompleted() const
{
    return ui->checkBox->isChecked();
}

void Task::rename()
{
    bool bOK;
    QString name = QInputDialog::getText(this, tr("Rename Task"),
                                         tr("Task name"),
                                         QLineEdit::Normal,
                                         this->name(),
                                         &bOK);
    if (bOK && !name.isEmpty())
    {
        setName(name);
    }
}

void Task::checked(bool checked)
{
    QFont font(ui->checkBox->font());
    font.setStrikeOut(checked);
    ui->checkBox->setFont(font);
    emit statusChanged(this);
}
