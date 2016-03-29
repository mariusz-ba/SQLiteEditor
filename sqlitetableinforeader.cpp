#include "sqlitetableinforeader.h"
#include <QDebug>

SQLiteTableInfoReader::SQLiteTableInfoReader(const QSqlDatabase& database)
{
    /**
     * Constructor of SQLiteTableInfoReader fills the list of TableInfo objects with data
     * from @database. For each table in database the list of ColumnInfo objects is created.
     */
    for(int i=0; i<database.tables().size(); ++i)
    {
        /**
         * Here the TableInfo obiect is created that represents a single table in database
         */
        TableInfo tableInfo;
        tableInfo.setTableName(database.tables().at(i));
        /**
         * Counting the number of columns in the table. For every table the column list is created
         * that stores information about each column.
         */
        int fieldsCount = database.driver()->record(tableInfo.tableName()).count();
        QList<ColumnInfo> columnInfos;
        for(int j=0; j<fieldsCount; ++j)
        {            
            /**
             * Here the ColumnInfo object is created. It represents a single column in database.
             * Next, it's filled with data retrieved from database: column name and its type.
             */
            ColumnInfo columnInfo;
            columnInfo.setName(database.driver()->record(tableInfo.tableName()).fieldName(j));
            columnInfo.setType(getFiledType(tableInfo.tableName(), j));
            columnInfos.push_back(columnInfo);
        }
        tableInfo.setColumnInfo(columnInfos);
        m_tableInfo.push_back(tableInfo);
    }
}

/**
 * retrieveTableInfo() function returns list of ColumnInfo objects for given table
 * @param tableName &QString - table name in database
 */
QList<ColumnInfo> SQLiteTableInfoReader::retrieveTableInfo(const QString &tableName)
{
    for(int i=0; i<m_tableInfo.size(); ++i)
    {
        if(m_tableInfo.at(i).tableName() == tableName)
            return m_tableInfo.at(i).retrieveColumnInfo();
    }
    return QList<ColumnInfo>();
}

/**
 * getFieldType() method returns type of specific column in table e.g. 'INTEGER PRIMARY KEY NOT NULL'
 * @param tableName &QString const - table name in database
 * @param column &int const - column number starting from 0
 * @param database &QSqlDatabase const - database which function is about to use
 */
QString SQLiteTableInfoReader::getFiledType(const QString &tableName, const int &column) const
{
    /**
     * For SQLite Database the 'pragma table_info(tablename)' query returns information
     * about table which goes like this:
     *
     * cid|name                 |type    |notnull |dflt_value |pk
     * 0  |id_fields_starring   |INTEGER |0       |           |1
     * 1  |fields_descriptor_id |INTEGER |1       |           |0
     * 2  |starring_id          |INTEGER |1       |           |0
     * 3  |form_mandatory       |INTEGER |1       |1          |0
     * 4  |form_visible         |INTEGER |1       |1          |0
     *
     */
    QSqlQuery information_query;
    information_query.prepare(QString("pragma table_info(%1);").arg(tableName));
    information_query.exec();

    QString return_value;

    /**
     * In this for loop return_value string is created. Every field in database is represented
     * by single line in example above. Every field has its id, name, type, notnull parameter, default value
     * and primary key indicator.
     */

    for(int i=0;information_query.next();++i)
    {
        /**
         * Firstly, searching for column that user gives in parameter @column
         */
        if(i == column) {
            qDebug() << information_query.record().value(2).toString();
            return_value = information_query.record().value(2).toString();
            /**
             * Checking if field has primary key indicator. If so, then we also need to check for autoincrement type.
             * If field is autoincremented then tablename and its rows count is stored in sqlite_squence table.
             */
            if(information_query.record().value(5).toBool()) {
                return_value.append(" PRIMARY KEY");
                /**
                 * If table name exists in sqlite_sequence table adding the AUTOINCREMENT type at the end of
                 * return_type string.
                 */
                QSqlQuery autoincrement_query;
                autoincrement_query.prepare(QString("SELECT COUNT(*) FROM sqlite_sequence WHERE name='%1';").arg(tableName));
                autoincrement_query.exec();
                while(autoincrement_query.next())
                    return_value.append(" AUTOINCREMENT");
            }
            /**
             * Simply checking for NOT NULL type and DEFAULT VALUE for field.
             */
            if(information_query.record().value(3).toBool()) return_value.append(" NOT NULL");
            if(!information_query.record().value(4).isNull()) return_value.append(" '" + information_query.record().value(4).toString() + "'");
            break;
        }
    }

    /**
     * Returning string that represents type of field in the table, e.g. 'INTEGER PRIMARY KEY NOT NULL AUTOINCREMENT'
     */
    return return_value;
}
