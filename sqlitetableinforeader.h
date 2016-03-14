#ifndef SQLITETABLEINFOREADER_H
#define SQLITETABLEINFOREADER_H

#include <QList>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlRecord>
#include <QStringList>
#include <QSqlField>
#include <QVariant>
#include <QString>
#include "tableinfo.h"
#include "tableinforeader.h"

class SQLiteTableInfoReader : public TableInfoReader
{
public:
    SQLiteTableInfoReader(const QSqlDatabase& database);
    QList<ColumnInfo> retrieveTableInfo(const QString& tableName);
private:
    QList<TableInfo> m_tableInfo;
};

#endif // SQLITETABLEINFOREADER_H
