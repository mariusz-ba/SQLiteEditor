#ifndef TABLEDIALOGDELEGATE_H
#define TABLEDIALOGDELEGATE_H

#include <QItemDelegate>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QPainter>

class TableDialogDelegate : public QItemDelegate
{
public:
    TableDialogDelegate();

    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // TABLEDIALOGDELEGATE_H
