/**
 *
 * TableInfoReaderFactory.h
 *
 * Created by Mariusz Baran on 14.03.2016
 * Copyright (c) 2016 Mariusz Baran. All rights reserved
 *
 * Factory class is creating proper TableInfoReader depending on the SQL
 * driver.
 */

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
