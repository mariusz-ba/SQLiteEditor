/**
 *
 * SQLiteTableInfoReader.h
 *
 * Created by Mariusz Baran on 14.03.2016
 * Copyright (c) 2016 Mariusz Baran. All rights reserved
 *
 * This class is responsible for extracting data from SQLite database.
 */

#ifndef SQLITETABLEINFOREADER_H
#define SQLITETABLEINFOREADER_H

#include <QList>

#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlRecord>
#include <QSqlField>
#include <QSqlQuery>

#include <QStringList>
#include <QVariant>

#include "tableinfo.h"
#include "tableinforeader.h"

class SQLiteTableInfoReader : public TableInfoReader
{
public:
    SQLiteTableInfoReader(const QSqlDatabase& database);
    QList<ColumnInfo> retrieveTableInfo(const QString& tableName);
private:
    QString getFiledType(const QString &tableName, const int &column) const;
    QList<TableInfo> m_tableInfo;
};

#endif // SQLITETABLEINFOREADER_H
