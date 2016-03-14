#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QStandardItemModel>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include "tabledialog.h"
#include "sqlitetableinforeader.h"
#include "tableinforeaderfactory.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void databaseOpened(bool);

private slots:
    void newDatabase();
    void openDatabase();
    void writeChanges();
    void closeDatabase();
    bool maybeSave();
    void about();
    void addRecord();
    void deleteRecord();
    void setCurrentTable(QString table_name);
    void createTable();
    void modifyTable();
    void deleteTable();

    void onCustomContextMenu(const QPoint& point);
    void treeViewActivated(const QModelIndex& index);

private:
    Ui::MainWindow *ui;
    void setupUi();
    void loadDatabase();

    QSqlDatabase db;
    QStandardItemModel* m_treeModel;
    QSqlTableModel* m_sqlTableModel;

    bool m_opened;
};

#endif // MAINWINDOW_H
