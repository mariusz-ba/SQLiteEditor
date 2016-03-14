#ifndef TABLEDIALOG_H
#define TABLEDIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include "tabledialogdelegate.h"

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

protected:
    virtual void accept();

private:
    Ui::TableDialog *ui;
    QStandardItemModel* m_model;
    TableDialogDelegate* m_delegate;
    QString m_query;
};

#endif // TABLEDIALOG_H
