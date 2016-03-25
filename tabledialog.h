/**
 *
 * TableDialog.h
 *
 * Created by Mariusz Baran on 11.03.2016
 * Copyright (c) 2016 Mariusz Baran. All rights reserved
 *
 * This is dialog window responsible for taking information from
 * user which is needed to create new table such as table name or list
 * of fields and its types.
 */

#ifndef TABLEDIALOG_H
#define TABLEDIALOG_H

#include <QDialog>
#include <QStandardItemModel>

#include "tabledialogdelegate.h"
#include "codeeditorhighlighter.h"

namespace Ui {
class TableDialog;
}

class TableDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TableDialog(QWidget *parent = 0);
    ~TableDialog();
    QString getQuery() const;

private slots:
    void updateTextBrowser();
    void addField();
    void removeField();
    void moveUpField();
    void moveDownField();

    void onTableViewEntered(const QModelIndex& index);

protected:
    virtual void accept();

private:
    Ui::TableDialog *ui;
    QStandardItemModel* m_model;
    TableDialogDelegate* m_delegate;
    QString m_query;
    CodeEditorHighlighter* m_highlighter;
};

#endif // TABLEDIALOG_H
