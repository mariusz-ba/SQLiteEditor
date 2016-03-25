/**
 *
 * DatabaseTreeParser.h
 *
 * Created by Mariusz Baran on 22.03.2016
 * Copyright (c) 2016 Mariusz Baran. All rights reserved
 *
 * This class extract data from SQL database using TableInfoReader class
 * and fills DatabaseTreeModel with it.
 */

#ifndef DatabaseTreeParser_H
#define DatabaseTreeParser_H

#include <QSqlDatabase>
#include <QString>
#include "tableinforeaderfactory.h"
#include "sqlitetableinforeader.h"
#include "databasetreenode.h"

class DatabaseTreeParser
{
public:
    DatabaseTreeParser();
    DatabaseTreeNode *parse(const QSqlDatabase& database);
};

#endif // DatabaseTreeParser_H
