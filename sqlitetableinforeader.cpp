#include "sqlitetableinforeader.h"

SQLiteTableInfoReader::SQLiteTableInfoReader(const QSqlDatabase& database)
{
    for(int i=0; i<database.tables().size(); ++i)
    {
        TableInfo tableInfo;
        tableInfo.setTableName(database.tables().at(i));
        int fieldsCount = database.driver()->record(tableInfo.tableName()).count();
        QList<ColumnInfo> columnInfos;
        for(int j=0; j<fieldsCount; ++j)
        {
            ColumnInfo columnInfo;
            columnInfo.setName(database.driver()->record(tableInfo.tableName()).fieldName(j));
            columnInfos.push_back(columnInfo);
        }
        tableInfo.setColumnInfo(columnInfos);
        m_tableInfo.push_back(tableInfo);
    }
}

QList<ColumnInfo> SQLiteTableInfoReader::retrieveTableInfo(const QString &tableName)
{
    for(int i=0; i<m_tableInfo.size(); ++i)
    {
        if(m_tableInfo.at(i).tableName() == tableName)
            return m_tableInfo.at(i).retrieveColumnInfo();
    }
    return QList<ColumnInfo>();
}


