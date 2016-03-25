/**
 *
 * DatabaseTreeNode.h
 *
 * Created by Mariusz Baran on 22.03.2016
 * Copyright (c) 2016 Mariusz Baran. All rights reserved
 *
 * Node represents a single element in DatabaseTreeModel such as
 * table or tabel field. Every Node object except root node has
 * name and type. Nodes that represent table or table field also have
 * icons.
 */

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
