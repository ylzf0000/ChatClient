#ifndef PDFLISTITEMDELEGATE_H
#define PDFLISTITEMDELEGATE_H

#include <QObject>
#include <QStyledItemDelegate>

class PdfListItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit PdfListItemDelegate(QObject *parent = nullptr);

signals:

public slots:

    // QAbstractItemDelegate interface
public:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif // PDFLISTITEMDELEGATE_H
