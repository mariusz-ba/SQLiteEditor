#include "sqlitetableinforeader.h"
#include <QDebug>

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
            columnInfo.setType(getFiledType(tableInfo.tableName(), j, database));
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

QString SQLiteTableInfoReader::getFiledType(const QString &tableName, const int &column, const QSqlDatabase& database) const
{
    QSqlQuery information_query;
    information_query.prepare(QString("pragma table_info(%1);").arg(tableName));
    information_query.exec();

    QString return_value;

    //Retrieving information about record type

    for(int i=0;information_query.next();++i)
    {
        if(i == column) {
            qDebug() << information_query.record().value(2).toString();
            return_value = information_query.record().value(2).toString();
            if(information_query.record().value(5).toBool()) {
                return_value.append(" PRIMARY KEY");
                //Checking if table has autoincrement squence
                QSqlQuery autoincrement_query;
                autoincrement_query.prepare(QString("SELECT COUNT(*) FROM sqlite_sequence WHERE name='%1';").arg(tableName));
                autoincrement_query.exec();
                while(autoincrement_query.next())
                    return_value.append(" AUTOINCREMENT");
            }
            if(information_query.record().value(3).toBool()) return_value.append(" NOT NULL");
            if(!information_query.record().value(4).isNull()) return_value.append(" '" + information_query.record().value(4).toString() + "'");
            break;
        }
    }

    return return_value;
}
