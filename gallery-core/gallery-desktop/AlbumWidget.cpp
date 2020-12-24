#include "AlbumWidget.h"
#include "ui_AlbumWidget.h"

#include <QInputDialog>
#include <QFileDialog>
#include "AlbumModel.h"
#include <QItemSelectionModel>
#include "PictureModel.h"
#include "PictureDelegate.h"
#include "ThumbnailProxyModel.h"


AlbumWidget::AlbumWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AlbumWidget),
    mAlbumModel(nullptr),
    mAlbumSelectionModel(nullptr),
    mThumbnailProxyModel(nullptr),
    mPictureSelectionModel(nullptr)
{
    ui->setupUi(this);
    clearUi();

    ui->thumbnailListView->setSpacing(5);
    ui->thumbnailListView->setResizeMode(QListView::Adjust);
    ui->thumbnailListView->setFlow(QListView::LeftToRight);
    ui->thumbnailListView->setWrapping(true);
    ui->thumbnailListView->setItemDelegate(new PictureDelegate);

    connect(ui->thumbnailListView, &QListView::doubleClicked, this, &AlbumWidget::pictureActivated);
    connect(ui->deleteButton, &QPushButton::clicked, this, &AlbumWidget::deleteAlbum);
    connect(ui->editButton, &QPushButton::clicked, this, &AlbumWidget::editAlbum);
    connect(ui->addPicturesButton, &QPushButton::clicked, this, &AlbumWidget::addPictures);
}

AlbumWidget::~AlbumWidget()
{
    delete ui;
}

void AlbumWidget::setAlbumModel(AlbumModel *model)
{
    mAlbumModel = model;

    connect(mAlbumModel, &QAbstractItemModel::dataChanged, [this](const QModelIndex &topLeft){
       if (topLeft == mAlbumSelectionModel->currentIndex())
           loadAlbum(topLeft);
    });
}

void AlbumWidget::setAlbumSelectionModel(QItemSelectionModel *model)
{
    mAlbumSelectionModel = model;
    connect(mAlbumSelectionModel, &QItemSelectionModel::selectionChanged, [this](const QItemSelection &selected){
       if (selected.isEmpty())  {
           clearUi();
           return;
       }

       loadAlbum(selected.indexes().first());
    });
}

void AlbumWidget::setPictureModel(ThumbnailProxyModel *pictureModel)
{
    mThumbnailProxyModel = pictureModel;
    ui->thumbnailListView->setModel(pictureModel);
}

void AlbumWidget::setPictureSelectionModel(QItemSelectionModel *model)
{
    ui->thumbnailListView->setSelectionModel(model);
}

void AlbumWidget::deleteAlbum()
{
    if (mAlbumSelectionModel->selectedIndexes().empty())
        return;

    int row = mAlbumSelectionModel->currentIndex().row();
    mAlbumModel->removeRow(row);

    QModelIndex previousModelIndex = mAlbumModel->index(row - 1, 0);
    if (previousModelIndex.isValid()) {
        mAlbumSelectionModel->setCurrentIndex(previousModelIndex, QItemSelectionModel::SelectCurrent);
        return;
    }

    QModelIndex nextModelIndex = mAlbumModel->index(row, 0);
    if (nextModelIndex.isValid()) {
        mAlbumSelectionModel->setCurrentIndex(nextModelIndex, QItemSelectionModel::SelectCurrent);
        return;
    }
}

void AlbumWidget::editAlbum()
{
    if (mAlbumSelectionModel->selectedIndexes().isEmpty())
        return;

    QModelIndex currentAlbumIndex = mAlbumSelectionModel->selectedIndexes().first();
    QString oldAbumName = mAlbumModel->data(currentAlbumIndex, AlbumModel::Roles::NameRole).toString();

    bool bOk;
    QString newName = QInputDialog::getText(this,
                                                "Album's name",
                                                "Change Album name",
                                                QLineEdit::Normal,
                                                oldAbumName,
                                                &bOk);
    if (bOk && !newName.isEmpty())
    {
        mAlbumModel->setData(currentAlbumIndex, newName, AlbumModel::Roles::NameRole);
    }
}

void AlbumWidget::addPictures()
{
    QStringList fileNames = QFileDialog::getOpenFileNames(this, "Add Pictures", QDir::homePath(), "Picture files (*.jpg *.png)");
    if (!fileNames.empty())
    {
        QModelIndex lastModelIndex;
        for (auto fileName : fileNames) {
            Picture picture(fileName);
            lastModelIndex = mThumbnailProxyModel->pictureModel()->addPicture(picture);
        }

        lastModelIndex = mThumbnailProxyModel->index(lastModelIndex.row(), 0);
        ui->thumbnailListView->setCurrentIndex(lastModelIndex);
    }
}

void AlbumWidget::clearUi()
{
    ui->albumName->setText("");
    ui->deleteButton->setVisible(false);
    ui->editButton->setVisible(false);
    ui->addPicturesButton->setVisible(false);
}

void AlbumWidget::loadAlbum(const QModelIndex &index)
{
    mThumbnailProxyModel->pictureModel()->setAlbumId(mAlbumModel->data(index, AlbumModel::Roles::IdRole).toInt());
    ui->albumName->setText(mAlbumModel->data(index, Qt::DisplayRole).toString());
    ui->deleteButton->setVisible(true);
    ui->editButton->setVisible(true);
    ui->addPicturesButton->setVisible(true);
}
