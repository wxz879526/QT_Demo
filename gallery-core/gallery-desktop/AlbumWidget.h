#ifndef ALBUMWIDGET_H
#define ALBUMWIDGET_H

#include <QWidget>
#include <QModelIndex>

namespace Ui {
class AlbumWidget;
}

class AlbumModel;
class PictureModel;
class QItemSelectionModel;
class ThumbnailProxyModel;

class AlbumWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AlbumWidget(QWidget *parent = nullptr);
    ~AlbumWidget();

    void setAlbumModel(AlbumModel *model);
    void setAlbumSelectionModel(QItemSelectionModel *model);
    void setPictureModel(ThumbnailProxyModel *pictureModel);
    void setPictureSelectionModel(QItemSelectionModel *model);

signals:
    void pictureActivated(const QModelIndex &index);

private:
    void deleteAlbum();
    void editAlbum();
    void addPictures();

private:
    void clearUi();
    void loadAlbum(const QModelIndex &index);

private:
    Ui::AlbumWidget *ui;
    AlbumModel *mAlbumModel;
    QItemSelectionModel *mAlbumSelectionModel;
    ThumbnailProxyModel *mThumbnailProxyModel;
    QItemSelectionModel *mPictureSelectionModel;
};

#endif // ALBUMWIDGET_H
