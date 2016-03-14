#include "tableinfo.h"

TableInfo::TableInfo()
{

}

void TableInfo::setTableName(const QString& tableName)
{
    m_tableName = tableName;
}

QString TableInfo::tableName() const
{
    return m_tableName;
}

void TableInfo::setColumnInfo(const QList<ColumnInfo>& columnInfos)
{
    m_columnInfos = columnInfos;
}

QList<ColumnInfo> TableInfo::retrieveColumnInfo() const
{
    return m_columnInfos;
}

