#include "AlbumListWidget.h"
#include "ui_AlbumListWidget.h"
#include <QInputDialog>
#include "AlbumModel.h"

AlbumListWidget::AlbumListWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AlbumListWidget),
    mAlbumModel(nullptr)
{
    ui->setupUi(this);

    connect(ui->createAlbumButton, &QPushButton::clicked, this, &AlbumListWidget::createAlbum);
}

AlbumListWidget::~AlbumListWidget()
{
    delete ui;
}

void AlbumListWidget::setModel(AlbumModel *model)
{
    mAlbumModel = model;
    ui->albumList->setModel(mAlbumModel);
}

void AlbumListWidget::setSelectionModel(QItemSelectionModel *selectionModel)
{
    ui->albumList->setSelectionModel(selectionModel);
}

void AlbumListWidget::createAlbum()
{
    if (!mAlbumModel) return;

    bool bOk;
    QString strAlbumName = QInputDialog::getText(this, "Create a new Album", "Choose an name", QLineEdit::Normal, "New album", &bOk);
    if (bOk && !strAlbumName.isEmpty())
    {
        Album album(strAlbumName);
        QModelIndex createIndex = mAlbumModel->addAlbum(album);
        ui->albumList->setCurrentIndex(createIndex);
    }
}
