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
