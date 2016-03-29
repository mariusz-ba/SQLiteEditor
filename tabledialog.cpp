#include "tabledialog.h"
#include "ui_tabledialog.h"
#include <QDebug>

TableDialog::TableDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TableDialog)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("Table"));
    this->setWindowIcon(QIcon(":/img/icon-table.png"));
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    m_highlighter = new CodeEditorHighlighter(ui->textBrowser->document());

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

    ui->tableView->setEditTriggers(QTableView::AllEditTriggers);
    ui->tableView->setMouseTracking(true);


}

TableDialog::~TableDialog()
{
    delete ui;
}

/**
 * getQuery() method returns a SQL query responsible for creating new
 * table in database.
 */
QString TableDialog::getQuery() const
{
    return m_query;
}

/**
 * loadTable() function enables user to fill tableDialog with default data. It allows
 * user to edit structure of specific table
 */
void TableDialog::loadTable(const QSqlDatabase &database, const QString &tableName)
{
    int index = 0;
    ui->table_lineEdit->setText(tableName);

    TableInfoReaderFactory factory;
    TableInfoReader* reader = factory.createTableReader(database);

    QList<ColumnInfo> columnInfos = reader->retrieveTableInfo(tableName);

    foreach(ColumnInfo cInfo, columnInfos)
    {
        addField();
        m_model->setData(m_model->index(index, 0), cInfo.name());
        m_model->setData(m_model->index(index, 1), cInfo.type().mid(0, cInfo.type().indexOf(" ")));
        m_model->setData(m_model->index(index, 2), cInfo.type().contains("NOT NULL"));
        m_model->setData(m_model->index(index, 3), cInfo.type().contains("PRIMARY KEY"));
        m_model->setData(m_model->index(index, 4), cInfo.type().contains("AUTOINCREMENT"));
        ++index;
    }

    delete reader;
}

/**
 * Every time the user make changes in tableview or table lineedit the textBrowser is
 * updated. It contains whole SQL query which is needed to create table.
 */
void TableDialog::updateTextBrowser()
{
    QString text;

    //if(m_type == CREATE)
    //{
        text = QString("CREATE TABLE '%1' (\n").arg(ui->table_lineEdit->text());
        for(int i=0; i<m_model->rowCount(); ++i)
        {
            text += "    ";
            text.append("'"+m_model->index(i,0).data().toString()+"' "+m_model->index(i,1).data().toString());

            if(m_model->index(i, 2).data().toBool()) text.append(" NOT NULL");
            if(m_model->index(i, 3).data().toBool()) text.append(" PRIMARY KEY");
            if(m_model->index(i, 4).data().toBool()) text.append(" AUTOINCREMENT");

            text.append(",\n");
        }
        text.remove(text.lastIndexOf(","),1);
        text.append(");");
    //}
    /*
    else // MODIFY
    {
        //Alter table
    }
    */

    ui->textBrowser->setText(text);
}

/**
 * addField() function simply adds new row in the tableview that allows user to create
 * more fields in table.
 */
void TableDialog::addField()
{
    if(m_model->rowCount() != 0)
    {
        for(int i=0;i<2; ++i)
        {
            /**
             * New row can be inserted only if previous row is filled with data.
             * Informations required for every field are: its name and its type, so those
             * fields in table must be filled.
             */
            if(m_model->index(m_model->rowCount()-1, i).data().isNull())
                return;
        }
    }

    //On success: add row to table and update text in textbrowser
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
