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

#include "sqlitetableinforeader.h"
#include "tableinforeaderfactory.h"

namespace Ui {
class TableDialog;
}

class TableDialog : public QDialog
{
    Q_OBJECT

public:
    enum DialogType {
        CREATE,
        MODIFY
    };

    explicit TableDialog(const DialogType& type = CREATE, QWidget *parent = 0);
    ~TableDialog();
    QStringList getQueryList();
    void loadTable(const QSqlDatabase& database, const QString& tableName);

private slots:
    void updateTextBrowser();
    void addField();
    void removeField();
    void moveUpField();
    void moveDownField();

    void onModelDataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& roles);

protected:
    virtual void accept();
    QStringList copyTable();

private:
    Ui::TableDialog *ui;
    DialogType m_type;
    QStandardItemModel* m_model;
    TableDialogDelegate* m_delegate;
    CodeEditorHighlighter* m_highlighter;

    //return query list
    QStringList m_queryList;

    //fields lists used for modifying table
    QStringList m_oldFields;
    QStringList m_newFields;
};

#endif // TABLEDIALOG_H
