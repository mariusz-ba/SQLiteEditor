#ifndef TABLEINFOREADER_H
#define TABLEINFOREADER_H

#include <QList>
#include "columninfo.h"

class TableInfoReader
{
public:
    virtual QList<ColumnInfo> retrieveTableInfo(const QString& tableName) = 0;
};

#endif // TABLEINFOREADER_H
