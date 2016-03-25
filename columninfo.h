/**
 *
 * ColumnInfo.h
 *
 * Created by Mariusz Baran on 14.03.2016
 * Copyright (c) 2016 Mariusz Baran. All rights reserved
 *
 * This class is responsible for stroing data that describes
 * information about specified coulmn in the table. Every column
 * has its name and type.
 */

#ifndef COLUMNINFO_H
#define COLUMNINFO_H

#include <QString>

class ColumnInfo
{
public:
    ColumnInfo();

    void setName(const QString& name);
    void setType(const QString& type);

    QString name() const;
    QString type() const;

private:
    QString m_name;
    QString m_type;
};

#endif // COLUMNINFO_H
