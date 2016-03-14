#ifndef TABLEINFO_H
#define TABLEINFO_H

#include <QList>
#include <QString>
#include "columninfo.h"

class TableInfo
{
public:
    TableInfo();
    void setTableName(const QString& tableName);
    QString tableName() const;

    void setColumnInfo(const QList<ColumnInfo>& columnInfos);
    QList<ColumnInfo> retrieveColumnInfo() const;
private:
    QString m_tableName;
    QList<ColumnInfo> m_columnInfos;
};

#endif // TABLEINFO_H
