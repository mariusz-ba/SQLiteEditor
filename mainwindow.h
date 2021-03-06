/**
 *
 * MainWindow.h
 *
 * Created by Mariusz Baran on 9.03.2016
 * Copyright (c) 2016 Mariusz Baran. All rights reserved
 *
 * This class represents main window of the application.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QTextBlock>

#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>

#include <QFileDialog>
#include <QFile>

#include "databasetreeparser.h"
#include "databasetreemodel.h"
#include "tabledialog.h"
#include "aboutdialog.h"

#include "codeeditor.h"

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

    void openSqlFile();
    void executeSQL();
    void executeSQLline();

private:
    Ui::MainWindow *ui;
    void setupUi();
    void loadDatabase();

    QSqlDatabase db;
    DatabaseTreeModel* m_treeModel;
    QSqlTableModel* m_sqlTableModel;
    QSqlQueryModel* m_sqlQueryModel;

    bool m_opened;

    QCompleter* completer;
};

#endif // MAINWINDOW_H
