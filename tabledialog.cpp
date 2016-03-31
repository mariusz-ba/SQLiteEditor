#include "tabledialog.h"
#include "ui_tabledialog.h"
#include <QDebug>

TableDialog::TableDialog(const DialogType& type, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TableDialog),
    m_type(type)
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

    //Setting resize mode in tableview
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

    connect(ui->moveUp_toolButton, SIGNAL(clicked()), this, SLOT(moveUpField()));
    connect(ui->addField_toolButton, SIGNAL(clicked()), this, SLOT(addField()));
    connect(ui->moveDown_toolButton, SIGNAL(clicked()), this, SLOT(moveDownField()));
    connect(ui->removeField_toolButton, SIGNAL(clicked()), this, SLOT(removeField()));

    connect(m_model, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)), this, SLOT(onModelDataChanged(QModelIndex, QModelIndex, QVector<int>)));

    ui->tableView->setEditTriggers(QTableView::CurrentChanged);
}

TableDialog::~TableDialog()
{
    delete ui;
}

/**
 * getQuery() method returns a SQL query responsible for creating new
 * table in database.
 */
QStringList TableDialog::getQueryList()
{
    if(m_type == CREATE)
        return m_queryList;
    return copyTable();
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
        m_oldFields << cInfo.name();
        m_newFields << cInfo.name();
        m_model->insertRow(index);
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
    QString text = QString("CREATE TABLE '%1' (\n").arg(ui->table_lineEdit->text());
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

    //add empty string to list
    m_oldFields << QString();
    m_newFields << QString();

    //On success: add row to table and update text in textbrowser
    m_model->insertRow(m_model->rowCount());
    updateTextBrowser();
}

void TableDialog::removeField()
{
    //Removing fields from fields lists
    m_oldFields.removeAt(ui->tableView->currentIndex().row());
    m_newFields.removeAt(ui->tableView->currentIndex().row());

     //Removing row from table model and updating table browser text.
    m_model->removeRow(ui->tableView->currentIndex().row());
    updateTextBrowser();
}

void TableDialog::moveUpField()
{
    //Make sure the current index row isn't the first one
    int row = ui->tableView->currentIndex().row();
    if(row == 0)
        return;

    //Swap fields names in lists
    qSwap(m_oldFields[row], m_oldFields[row-1]);
    qSwap(m_newFields[row], m_newFields[row-1]);

    //Swap items in the table
    QList<QStandardItem*> items = m_model->takeRow(row);
    m_model->insertRow(row-1, items);
    updateTextBrowser();
}

void TableDialog::moveDownField()
{
    //Make sure the current index row isn't the last one
    int row = ui->tableView->currentIndex().row();
    if(row == m_model->rowCount()-1)
        return;

    //Swap fields names in lists
    qSwap(m_oldFields[row], m_oldFields[row+1]);
    qSwap(m_newFields[row], m_newFields[row+1]);

    //Swap items in the table
    QList<QStandardItem*> items = m_model->takeRow(row);
    m_model->insertRow(row+1, items);
    updateTextBrowser();
}

/**
 * This function is called whenever user modify data in tableview. If user modified field name
 * (which is stored in first column) we need to change corresponding field name in m_newFields list.
 */
void TableDialog::onModelDataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& roles)
{
    //change name only in newFields list
    if(topLeft == bottomRight && topLeft.column() == 0 && topLeft.row() >= 0) {
        m_newFields[topLeft.row()] = topLeft.model()->data(topLeft).toString();
    }
}

void TableDialog::accept()
{
    if(m_type == CREATE)
    {
        QString query = ui->textBrowser->toPlainText().remove("\n");
        query.replace("    ", " ");
        m_queryList << query;
    }

    QDialog::accept();
}

/**
 * copyTable() function creates copy of modifying table with SQL statement from textBrowser.
 * In SQLite when it comes to modify table structure, we need to create new copy of the table
 * with different shema and then copy old data to new table.
 * We use m_oldFields and m_newFields lists to store fields existed in old table and fields available
 * in new. It allows to put old data in proper place in new table.
 */
QStringList TableDialog::copyTable()
{
    QStringList returnList;
    //Create table copy
    returnList << ui->textBrowser->toPlainText().replace(ui->table_lineEdit->text(), ui->table_lineEdit->text() + "_temp_copy");

    int index = m_oldFields.indexOf(QString(), 0);
    while(index >= 0)
    {
        m_oldFields.removeAt(index);
        m_newFields.removeAt(index);
        index = m_oldFields.indexOf(QString(), index);
    }

    //Copy old table data
    QString insertion = QString("INSERT INTO '%1' (%2) SELECT %3 FROM '%4'").arg(ui->table_lineEdit->text() + "_temp_copy").arg(m_newFields.join(',')).arg(m_oldFields.join(',')).arg(ui->table_lineEdit->text());
    returnList << insertion;

    //Delete old table
    returnList << QString("DROP TABLE '%1';").arg(ui->table_lineEdit->text());

    //Rename new table to old table name
    returnList << QString("ALTER TABLE '%1' RENAME TO '%2';").arg(ui->table_lineEdit->text() + "_temp_copy").arg(ui->table_lineEdit->text());

    //returnList contains all SQL statements that are needed for modifying table
    return returnList;
}
