#include "tabledialogdelegate.h"

TableDialogDelegate::TableDialogDelegate()
{

}

void TableDialogDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.column() >= 2 && index.column() <= 4)
    {
        if(index.model()->data(index).toBool())
        {
            QRect rect(option.rect.x()+2, option.rect.y()+2, option.rect.width()-5, option.rect.height()-5);
            QImage image(":/img/icon-tick.png");
            painter->drawImage(rect, image);
        }

    }
    else
    {
        QItemDelegate::paint(painter, option, index);
    }
}

QWidget* TableDialogDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.column() == 0)
    {
        QLineEdit* lineEdit = new QLineEdit(parent);
        lineEdit->setPlaceholderText(tr("Input field name"));
        return lineEdit;
    }
    if(index.column() == 1)
    {
        QComboBox* comboBox = new QComboBox(parent);
        QStringList types;
        types << "INTEGER" << "TEXT" << "BLOB" << "REAL" << "NUMERIC";
        comboBox->addItems(types);
        return comboBox;
    }
    if(index.column() == 2)
    {
        QCheckBox* checkBox = new QCheckBox(parent);
        checkBox->setToolTip(tr("NOT NULL"));
        return checkBox;
    }
    if(index.column() == 3)
    {
        QCheckBox* checkBox = new QCheckBox(parent);
        checkBox->setToolTip(tr("PRIMARY KEY"));
        return checkBox;
    }
    if(index.column() == 4)
    {
        QCheckBox* checkBox = new QCheckBox(parent);
        checkBox->setToolTip(tr("AUTOINCREMENT"));
        return checkBox;
    }

    return QItemDelegate::createEditor(parent, option, index);
}

void TableDialogDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}

void TableDialogDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if(index.column() == 0)
    {
        static_cast<QLineEdit*>(editor)->setText(index.model()->data(index).toString());
    }
    if(index.column() == 1)
    {
        static_cast<QComboBox*>(editor)->setCurrentText(index.model()->data(index).toString());
    }
    if(index.column() == 2)
    {
        static_cast<QCheckBox*>(editor)->setChecked(index.model()->data(index).toBool());
    }
    if(index.column() == 3)
    {
        static_cast<QCheckBox*>(editor)->setChecked(index.model()->data(index).toBool());
    }
    if(index.column() == 4)
    {
        static_cast<QCheckBox*>(editor)->setChecked(index.model()->data(index).toBool());
    }

    return QItemDelegate::setEditorData(editor, index);
}

void TableDialogDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if(index.column() == 0)
    {
        model->setData(index, static_cast<QLineEdit*>(editor)->text());
    }
    if(index.column() == 1)
    {
        model->setData(index, static_cast<QComboBox*>(editor)->currentText());
    }
    if(index.column() == 2)
    {
        model->setData(index, static_cast<QCheckBox*>(editor)->isChecked());
    }
    if(index.column() == 3)
    {
        model->setData(index, static_cast<QCheckBox*>(editor)->isChecked());
    }
    if(index.column() == 4)
    {
        model->setData(index, static_cast<QCheckBox*>(editor)->isChecked());
    }

    return QItemDelegate::setEditorData(editor, index);
}

QSize TableDialogDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QSize(option.rect.width(), option.rect.height());
}
