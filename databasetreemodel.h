/**
 *
 * DatabaseTreeModel.h
 *
 * Created by Mariusz Baran on 22.03.2016
 * Copyright (c) 2016 Mariusz Baran. All rights reserved
 *
 * This model is storing database structure.
 */

#ifndef DatabaseTreeModel_H
#define DatabaseTreeModel_H

#include <QAbstractItemModel>
#include "databasetreenode.h"

class DatabaseTreeModel : public QAbstractItemModel
{
public:
    DatabaseTreeModel(QObject* parent = 0);
    ~DatabaseTreeModel();
    void setRootNode(DatabaseTreeNode* node);
    void clear();

    virtual QModelIndex index(int row, int column, const QModelIndex &parent) const;
    virtual QModelIndex parent(const QModelIndex &child) const;

    virtual int rowCount(const QModelIndex &parent) const;
    virtual int columnCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;

private:
    DatabaseTreeNode *nodeFromIndex(const QModelIndex& index) const;
    DatabaseTreeNode* rootNode;
};

#endif // DatabaseTreeModel_H
