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
