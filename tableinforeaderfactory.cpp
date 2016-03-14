#include "tableinforeaderfactory.h"

TableInfoReader* TableInfoReaderFactory::createTableReader(const QSqlDatabase &database)
{
    if(database.driverName() == "QSQLITE")
        return new SQLiteTableInfoReader(database);

    return NULL;
}
