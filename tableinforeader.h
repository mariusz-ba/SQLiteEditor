/**
 *
 * TableInfoReader.h
 *
 * Created by Mariusz Baran on 14.03.2016
 * Copyright (c) 2016 Mariusz Baran. All rights reserved
 *
 * Abstract reader class for extracting database structure from database.
 *
 */

#ifndef TABLEINFOREADER_H
#define TABLEINFOREADER_H

#include <QList>
#include "columninfo.h"

class TableInfoReader
{
public:
    virtual ~TableInfoReader() {}
    virtual QList<ColumnInfo> retrieveTableInfo(const QString& tableName) = 0;
};

#endif // TABLEINFOREADER_H
