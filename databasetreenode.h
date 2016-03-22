#ifndef DatabaseTreeNode_H
#define DatabaseTreeNode_H

#include <QString>
#include <QList>
#include <QIcon>

class DatabaseTreeNode
{
public:
    DatabaseTreeNode(const QString& name, const QString& type);
    ~DatabaseTreeNode();

    QString name;
    QString type;
    QIcon icon;
    DatabaseTreeNode* parent;
    QList<DatabaseTreeNode*> children;
};

#endif // DatabaseTreeNode_H
