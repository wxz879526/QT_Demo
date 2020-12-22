#include "PictureModel.h"
#include "Picture.h"


PictureModel::PictureModel(const AlbumModel &albumModel, QObject *parent)
    : QAbstractListModel(parent)
    , mDb(DatabaseManager::instance())
    , mAlbumId(-1)
    , mPictures(new std::vector<std::unique_ptr<Picture>>())
{
    connect(&albumModel, &AlbumModel::rowsRemoved, this, &PictureModel::deletePicturesForAlbum);
}

QModelIndex PictureModel::addPicture(const Picture &pic)
{
    int rowIndex = rowCount();
    beginInsertRows(QModelIndex(), rowIndex, rowIndex);
    std::unique_ptr<Picture> picture(new Picture(pic));
    mDb.picDao.addPictureInAlbum(mAlbumId, *picture);
    mPictures->push_back(std::move(picture));
    endInsertRows();
    return index(rowIndex);
}

void PictureModel::setAlbumId(int albumId)
{
    beginResetModel();
    mAlbumId = albumId;
    loadPictures(mAlbumId);
    endResetModel();
}

void PictureModel::clearAlbum()
{
    setAlbumId(-1);
}

int PictureModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return mPictures->size();
}

QVariant PictureModel::data(const QModelIndex& index, int role) const
{
    if (!isIndexValid(index)) {
        return QVariant();
    }

    const Picture& picture = *mPictures->at(index.row());
    switch (role) {
        case Qt::DisplayRole:
            return picture.fileUrl().fileName();
            break;

        case Roles::UrlRole:
            return picture.fileUrl();
            break;

        case Roles::FilePathRole:
            return picture.fileUrl().toLocalFile();
            break;


        default:
            return QVariant();
    }
}

bool PictureModel::removeRows(int row, int count, const QModelIndex &parent)
{
    if (row < 0
               || row >= rowCount()
               || count < 0
               || (row + count) > rowCount()) {
           return false;
       }

       beginRemoveRows(parent, row, row + count - 1);
       int countLeft = count;
       while(countLeft--) {
           const Picture& picture = *mPictures->at(row + countLeft);
           mDb.picDao.removePicture(picture.id());
       }
       mPictures->erase(mPictures->begin() + row,
                       mPictures->begin() + row + count);
       endRemoveRows();


       return true;
}

QHash<int, QByteArray> PictureModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Roles::UrlRole] = "url";
    roles[Roles::FilePathRole] = "filepath";
    roles[Qt::DisplayRole] = "name";
    return roles;
 }

void PictureModel::loadPictures(int albumId)
{
    if (albumId <= 0) {
        mPictures.reset(new std::vector<std::unique_ptr<Picture>>());
        return;
    }
    mPictures = mDb.picDao.picturesForAlbum(albumId);
}

void PictureModel::deletePicturesForAlbum()
{
    mDb.picDao.removePicturesForAlbum(mAlbumId);
    clearAlbum();
}

bool PictureModel::isIndexValid(const QModelIndex &index) const
{
    if (index.row() < 0 || index.row() >= rowCount() || !index.isValid())
        return false;

    return true;
}
