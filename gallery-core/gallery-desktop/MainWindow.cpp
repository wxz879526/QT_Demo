#include "MainWindow.h"
#include "ui_MainWindow.h"

#include<QItemSelectionModel>
#include<GalleryWidget.h>
#include<AlbumModel.h>
#include<PictureModel.h>
#include"ThumbnailProxyModel.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , mGalleryWidget(new GalleryWidget(this))
    , mStackWidget(new QStackedWidget(this))
{
    ui->setupUi(this);

    AlbumModel *albumModel = new AlbumModel(this);
    QItemSelectionModel *albumSelectionModel = new QItemSelectionModel(albumModel, this);
    mGalleryWidget->setAlbumModel(albumModel);
    mGalleryWidget->setAlbumSelectionModel(albumSelectionModel);

    PictureModel *pictureModel = new PictureModel(*albumModel, this);
    ThumbnailProxyModel *thumbnailModel = new ThumbnailProxyModel(this);
    thumbnailModel->setSourceModel(pictureModel);

    QItemSelectionModel *pictureSelectionModel = new QItemSelectionModel(thumbnailModel, this);
    mGalleryWidget->setPictureModel(thumbnailModel);
    mGalleryWidget->setPictureSelectionModel(pictureSelectionModel);

    mStackWidget->addWidget(mGalleryWidget);
    displayGallery();

    setCentralWidget(mStackWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::displayGallery()
{
    mStackWidget->setCurrentWidget(mGalleryWidget);
}

void MainWindow::displayPicture()
{

}

