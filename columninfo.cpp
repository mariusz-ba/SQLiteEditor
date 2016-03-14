#include "columninfo.h"

ColumnInfo::ColumnInfo()
{

}

void ColumnInfo::setName(const QString& name)
{
    m_name = name;
}

void ColumnInfo::setType(const QString& type)
{
    m_type = type;
}

QString ColumnInfo::name() const
{
    return m_name;
}

QString ColumnInfo::type() const
{
    return m_type;
}
