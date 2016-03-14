#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), m_opened(false)
{
    ui->setupUi(this);
    db = QSqlDatabase::addDatabase("QSQLITE");

    setupUi();

    m_treeModel = new QStandardItemModel(this);
    m_treeModel->setColumnCount(2);
    m_treeModel->setHorizontalHeaderLabels({"Name", "Type"});

    m_sqlTableModel = new QSqlTableModel(this);
    m_sqlTableModel->setEditStrategy(QSqlTableModel::OnFieldChange);

    ui->treeView->setModel(m_treeModel);
    ui->tableView->setModel(m_sqlTableModel);
    emit databaseOpened(false);
}

MainWindow::~MainWindow()
{
    delete ui;
    if(db.isOpen())
        db.close();
}

void MainWindow::newDatabase()
{
    if(maybeSave())
    {
        //Create new database
        QFileDialog dialog(this);
        dialog.setModal(true);
        dialog.setAcceptMode(QFileDialog::AcceptSave);
        dialog.setDirectory(QDir::homePath());

        switch(dialog.exec())
        {
        case QFileDialog::Accepted:
            m_opened = true;
            break;
        case QFileDialog::Rejected:
            m_opened = false;
            return;
        }

        QString fileName = dialog.selectedFiles().first();

        db.setDatabaseName(fileName);
        db.open();

        QSqlQuery qry(db);
        qry.prepare("CREATE TABLE test_table(id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, some_text TEXT)");
        qry.exec();

        loadDatabase();

        emit databaseOpened(true);

        ui->actionModify_Table->setEnabled(false);
        ui->actionDelete_Table->setEnabled(false);

        ui->statusBar->showMessage("Database created.");
    }
}

void MainWindow::openDatabase()
{
    if(maybeSave())
    {
        //Open existing database
        QFileDialog dialog(this);
        dialog.setModal(true);
        dialog.setAcceptMode(QFileDialog::AcceptOpen);
        dialog.setDirectory(QDir::homePath());

        switch(dialog.exec())
        {
        case QFileDialog::Accepted:
            m_opened = true;
            break;
        case QFileDialog::Rejected:
            m_opened = false;
            return;
        }

        QString fileName = dialog.selectedFiles().first();
        db.setDatabaseName(fileName);
        db.open();

        loadDatabase();

        emit databaseOpened(true);

        ui->actionModify_Table->setEnabled(false);
        ui->actionDelete_Table->setEnabled(false);

        ui->statusBar->showMessage("Database opened.");
    }
}

void MainWindow::writeChanges()
{

}

void MainWindow::closeDatabase()
{
    if(db.isOpen())
        db.close();
    m_sqlTableModel->clear();
    m_treeModel->clear();
    ui->table_ComboBox->clear();

    emit databaseOpened(false);
    ui->statusBar->showMessage("Database not open.");
}

bool MainWindow::maybeSave()
{
    if(db.isOpen() && m_opened)
    {
        int ret = QMessageBox::warning(this, tr("Closing database"), tr("Are you sure to close current database?"),
                                       QMessageBox::No | QMessageBox::Yes);
        switch(ret)
        {
        case QMessageBox::Yes:
            closeDatabase();
            return true;
        case QMessageBox::No:
            return false;
        }
    }
    return true;
}

void MainWindow::about()
{

}

void MainWindow::addRecord()
{
    if(db.isOpen())
    {
        QSqlQuery insertion(db);
        insertion.prepare("INSERT INTO "+ui->table_ComboBox->currentText()+" DEFAULT VALUES");
        insertion.exec();

        qDebug() << "Adding row to " << ui->table_ComboBox->currentText();
        qDebug() << insertion.lastError().text();
        m_sqlTableModel->select();
        qDebug() << m_sqlTableModel->rowCount();
    }
}

void MainWindow::deleteRecord()
{
    if(db.isOpen())
    {
        m_sqlTableModel->removeRow(ui->tableView->currentIndex().row());
        qDebug() << "Row deleted";
        m_sqlTableModel->select();
    }
}

void MainWindow::setCurrentTable(QString table_name)
{
    m_sqlTableModel->setTable(table_name);
    qDebug() << "Setting table to: " << m_sqlTableModel->tableName();
    m_sqlTableModel->select();
}

void MainWindow::createTable()
{
    TableDialog dialog;
    switch(dialog.exec())
    {
    case TableDialog::Accepted:
    {
        QSqlQuery creating(dialog.getQuery());
        creating.exec();
        loadDatabase();
        break;
    }

    case TableDialog::Rejected:
    {
        break;
    }
    }
}

void MainWindow::modifyTable()
{

}

void MainWindow::deleteTable()
{
    QModelIndex index = ui->treeView->currentIndex();
    if(index.parent() == QModelIndex()) return;

    int ret = QMessageBox::warning(this, tr("Warning"), tr("Are you sure you want to delete table '%1' ?").arg(index.data().toString()), QMessageBox::Yes | QMessageBox::No);

    if(ret == QMessageBox::Yes) {
        qDebug() << "Deleting table " << index.data().toString();

        QSqlQuery deleting(db);
        deleting.prepare(QString("DROP TABLE %1").arg(index.data().toString()));
        deleting.exec();

        loadDatabase();
    }
}

void MainWindow::onCustomContextMenu(const QPoint& point)
{
    QPoint globalPos = ui->treeView->mapToGlobal(point);

    QModelIndex index = ui->treeView->indexAt(point);
    QString data = m_treeModel->index(index.row(), 1, index.parent()).data().toString();
    if(index.isValid() && data == "Table")
    {
        QMenu contextMenu;
        ui->actionModify_Table->setEnabled(true);
        ui->actionDelete_Table->setEnabled(true);
        QAction* browseAction = new QAction(QIcon(":/img/icon-table.png"), tr("Browse table"), &contextMenu);
        contextMenu.addActions({browseAction, ui->actionModify_Table, ui->actionDelete_Table});

        QAction* selectedAction = contextMenu.exec(globalPos + QPoint(0, ui->treeView->header()->height()));
        if(selectedAction == browseAction)
        {
            //Browse
            ui->tabWidget->setCurrentIndex(1);
            ui->table_ComboBox->setCurrentText(index.data().toString());
        }
    }
}

void MainWindow::treeViewActivated(const QModelIndex &index)
{
    if(m_treeModel->index(index.row(), 1, index.parent()).data().toString() != "Table")
    {
        ui->actionModify_Table->setEnabled(false);
        ui->actionDelete_Table->setEnabled(false);
    }
    else
    {
        ui->actionModify_Table->setEnabled(true);
        ui->actionDelete_Table->setEnabled(true);
    }
}

void MainWindow::setupUi()
{
    //Setup main window
    setWindowTitle("SQLite Editor");
    setWindowIcon(QIcon(":/img/icon-database.png"));

    //Setup database actions
    ui->actionNew_Database->setIcon(QIcon(":/img/icon-database-new.png"));
    ui->actionOpen_Database->setIcon(QIcon(":/img/icon-database-open.png"));
    ui->actionWrite_Changes->setIcon(QIcon(":/img/icon-database-savechanges.png"));

    //Setting up toolbar
    ui->mainToolBar->addActions({ui->actionNew_Database, ui->actionOpen_Database});
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addActions({ui->actionWrite_Changes});
    ui->mainToolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    //Setup ToolButtons and actions for adding, editing and deleting tables
    ui->toolButtonCreate->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    ui->toolButtonModify->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    ui->toolButtonDelete->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    ui->toolButtonCreate->setDefaultAction(ui->actionCreate_Table);
    ui->toolButtonModify->setDefaultAction(ui->actionModify_Table);
    ui->toolButtonDelete->setDefaultAction(ui->actionDelete_Table);

    ui->actionCreate_Table->setIcon(QIcon(":/img/icon-table-create.png"));
    ui->actionModify_Table->setIcon(QIcon(":/img/icon-table-edit.png"));
    ui->actionDelete_Table->setIcon(QIcon(":/img/icon-table-delete.png"));

    //Setting up connections for main actions
    connect(ui->actionNew_Database, SIGNAL(triggered()), this, SLOT(newDatabase()));
    connect(ui->actionOpen_Database, SIGNAL(triggered()), this, SLOT(openDatabase()));

    //Setting up connectins for actions on 'Browse Database' tab
    connect(ui->table_ComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(setCurrentTable(QString)));
    connect(ui->newRecord_Button, SIGNAL(clicked()), this, SLOT(addRecord()));
    connect(ui->deleteRecord_Button, SIGNAL(clicked()), this, SLOT(deleteRecord()));

    //Setting up connections for modify tables buttons
    connect(ui->actionCreate_Table, SIGNAL(triggered()), this, SLOT(createTable()));
    connect(ui->actionModify_Table, SIGNAL(triggered()), this, SLOT(modifyTable()));
    connect(ui->actionDelete_Table, SIGNAL(triggered()), this, SLOT(deleteTable()));
    //connect(ui->actionCreate_Table, SIGNAL(triggered()), this, SLOT(createTable()));

    //Setting up connections for enabling and disabling buttons
    connect(this, SIGNAL(databaseOpened(bool)), ui->actionCreate_Table, SLOT(setEnabled(bool)));
    connect(this, SIGNAL(databaseOpened(bool)), ui->actionModify_Table, SLOT(setEnabled(bool)));
    connect(this, SIGNAL(databaseOpened(bool)), ui->actionDelete_Table, SLOT(setEnabled(bool)));
    connect(this, SIGNAL(databaseOpened(bool)), ui->actionWrite_Changes, SLOT(setEnabled(bool)));

    connect(ui->treeView, &QTreeView::expanded, [&](){ui->treeView->resizeColumnToContents(0);});

    ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->treeView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(onCustomContextMenu(QPoint)));

    connect(ui->treeView, SIGNAL(clicked(QModelIndex)), this, SLOT(treeViewActivated(QModelIndex)));
}

void MainWindow::loadDatabase()
{
    if(db.isOpen())
    {
        m_treeModel->clear();
        m_treeModel->setHorizontalHeaderLabels({"Name", "Type"});
        ui->table_ComboBox->clear();

        TableInfoReaderFactory factory;
        TableInfoReader* reader = factory.createTableReader(db);

        QStringList tables = db.tables();

        QStandardItem* root_item = new QStandardItem(tr("Tables (%1)").arg(QString::number(tables.size())));
        for(int i=0; i<tables.size(); ++i)
        {
            QList<ColumnInfo> columnInfo = reader->retrieveTableInfo(tables.at(i));
            QStandardItem* table_item = new QStandardItem(QIcon(":/img/icon-table.png"),tables.at(i));
            for(int j=0; j<columnInfo.size(); ++j)
            {
                QStandardItem* field_item = new QStandardItem(QIcon(":/img/icon-field.png"), columnInfo.at(j).name());
                QStandardItem* field_item_type = new QStandardItem(columnInfo.at(j).type());
                table_item->appendRow({field_item, field_item_type});
            }
            root_item->appendRow({table_item, new QStandardItem("Table")});
            ui->table_ComboBox->addItem(QIcon(":/img/icon-table.png"), tables.at(i));
        }

        m_treeModel->appendRow(root_item);
        ui->treeView->expand(root_item->index());
    }
}
