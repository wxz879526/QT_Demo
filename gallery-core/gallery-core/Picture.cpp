#include "Picture.h"

Picture::Picture(const QString &filePath)
    : Picture(QUrl::fromLocalFile(filePath))
{

}

Picture::Picture(const QUrl &fileUrl)
    : mId(-1)
    , mAlumId(-1)
    , mFileUrl(fileUrl)
{

}

int Picture::id() const
{
    return mId;
}

void Picture::setId(int id)
{
    mId = id;
}

int Picture::albumId() const
{
    return mAlumId;
}

void Picture::setAlbumId(int albumId)
{
    mAlumId = albumId;
}

QUrl Picture::fileUrl() const
{
    return mFileUrl;
}

void Picture::setFileUrl(const QUrl &url)
{
    mFileUrl = url;
}