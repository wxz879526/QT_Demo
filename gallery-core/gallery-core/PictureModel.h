#ifndef PICTUREMODEL_H
#define PICTUREMODEL_H

#include <QAbstractListModel>
#include <QHash>
#include <vector>
#include <memory>

#include "gallery-core_global.h"
#include "AlbumModel.h"
#include "Picture.h"
#include "DatabaseManager.h"


class GALLERYCORE_EXPORT PictureModel : public QAbstractListModel
{
    Q_OBJECT
public:

    enum Roles {
        UrlRole = Qt::UserRole + 1,
        FilePathRole
    };

    PictureModel(const AlbumModel &albumModel, QObject *parent = nullptr);

    QModelIndex addPicture(const Picture &pic);
    void setAlbumId(int albumId);
    void clearAlbum();

public slots:
    void deletePicturesForAlbum();


    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool removeRows(int row, int count, const QModelIndex &parent);
    QHash<int, QByteArray> roleNames() const;

private:
    void loadPictures(int albumId);
    bool isIndexValid(const QModelIndex &index) const;

private:
    DatabaseManager &mDb;
    int mAlbumId;
    std::unique_ptr<std::vector<std::unique_ptr<Picture>>> mPictures;
};

#endif // PICTUREMODEL_H
