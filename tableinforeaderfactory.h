#ifndef TABLEINFOREADERFACTORY_H
#define TABLEINFOREADERFACTORY_H

#include "tableinforeader.h"
#include "sqlitetableinforeader.h"

#include <QSqlDatabase>

class TableInfoReaderFactory
{
public:
    TableInfoReader* createTableReader(const QSqlDatabase& database);
};

#endif // TABLEINFOREADERFACTORY_H
