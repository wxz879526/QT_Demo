#include "PictureDelegate.h"
#include <QPainter>

PictureDelegate::PictureDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{

}

void PictureDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{

}

QSize PictureDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    const QPixmap &pixmap = index.model()->data(index, Qt::DecorationRole).value<QPixmap>();
    return pixmap.size();
}
