#include "AlbumWidget.h"
#include "ui_AlbumWidget.h"

#include <QInputDialog>
#include <QFileDialog>
#include "AlbumModel.h"
#include "PictureModel.h"
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
}

AlbumWidget::~AlbumWidget()
{
    delete ui;
}

void AlbumWidget::setAlbumModel(AlbumModel *model)
{

}

void AlbumWidget::setAlbumSelectionModel(QItemSelectionModel *model)
{

}

void AlbumWidget::setPictureModel(ThumbnailProxyModel *pictureModel)
{

}

void AlbumWidget::setPictureSelectionModel(QItemSelectionModel *model)
{

}

void AlbumWidget::deleteAlbum()
{

}

void AlbumWidget::editAlbum()
{

}

void AlbumWidget::addPictures()
{

}

void AlbumWidget::clearUi()
{

}
