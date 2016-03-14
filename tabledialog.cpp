#include "tabledialog.h"
#include "ui_tabledialog.h"
#include <QDebug>

TableDialog::TableDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TableDialog)
{
    ui->setupUi(this);
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    m_delegate = new TableDialogDelegate();
    m_model = new QStandardItemModel(this);
    m_model->setColumnCount(5);
    m_model->setHeaderData(0, Qt::Horizontal, tr("Field Name"));
    m_model->setHeaderData(1, Qt::Horizontal, tr("Type"));
    m_model->setHeaderData(2, Qt::Horizontal, tr("NN"));
    m_model->setHeaderData(3, Qt::Horizontal, tr("PK"));
    m_model->setHeaderData(4, Qt::Horizontal, tr("AI"));
    ui->tableView->setModel(m_model);

    for(int i=0; i<m_model->columnCount(); ++i) {
        if(i<2)
            ui->tableView->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);
        else{
            ui->tableView->horizontalHeader()->resizeSection(i, ui->tableView->horizontalHeader()->height()*double(1.33));
            ui->tableView->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Fixed);
        }
    }

    ui->tableView->setItemDelegate(m_delegate);

    connect(m_model, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(updateTextBrowser()));
    connect(ui->table_lineEdit, SIGNAL(textEdited(QString)), this, SLOT(updateTextBrowser()));

    connect(ui->addField_toolButton, SIGNAL(clicked()), this, SLOT(addField()));
    connect(ui->removeField_toolButton, SIGNAL(clicked()), this, SLOT(removeField()));
    connect(ui->moveUp_toolButton, SIGNAL(clicked()), this, SLOT(moveUpField()));
    connect(ui->moveDown_toolButton, SIGNAL(clicked()), this, SLOT(moveDownField()));
}

TableDialog::~TableDialog()
{
    delete ui;
}

QString TableDialog::getQuery() const
{
    return m_query;
}

void TableDialog::updateTextBrowser()
{
    QString text = QString("CREATE TABLE '%1' (\n").arg(ui->table_lineEdit->text());
    for(int i=0; i<m_model->rowCount(); ++i)
    {
        text += "    ";
        text.append("'"+m_model->index(i,0).data().toString()+"' "+m_model->index(i,1).data().toString());
        text.append(",\n");
    }
    text.remove(text.lastIndexOf(","),1);
    text.append(");");
    ui->textBrowser->setText(text);
}

void TableDialog::addField()
{
    if(m_model->rowCount() != 0)
    {
        for(int i=0;i<2; ++i)
        {
            if(m_model->index(m_model->rowCount()-1, i).data().isNull())
                return;
        }
    }

    m_model->insertRow(m_model->rowCount());
    updateTextBrowser();
}

void TableDialog::removeField()
{
    m_model->removeRow(ui->tableView->currentIndex().row());
    updateTextBrowser();
}

void TableDialog::moveUpField()
{
    int row = ui->tableView->currentIndex().row();
    if(row == 0)
        return;

    QList<QStandardItem*> items = m_model->takeRow(row);
    m_model->insertRow(row-1, items);
    updateTextBrowser();
}

void TableDialog::moveDownField()
{
    int row = ui->tableView->currentIndex().row();
    if(row == m_model->rowCount()-1)
        return;

    QList<QStandardItem*> items = m_model->takeRow(row);
    m_model->insertRow(row+1, items);
    updateTextBrowser();
}

void TableDialog::accept()
{
    m_query = ui->textBrowser->toPlainText().remove("\n");
    m_query.replace("    ", " ");
    qDebug() << m_query;
    QDialog::accept();
}
