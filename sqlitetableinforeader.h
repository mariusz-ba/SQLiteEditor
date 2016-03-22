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
    QString getFiledType(const QString &tableName, const int &column, const QSqlDatabase& database) const;
private:
    QList<TableInfo> m_tableInfo;
};

#endif // SQLITETABLEINFOREADER_H
