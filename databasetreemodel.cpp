#include "databasetreemodel.h"
#include <QDebug>

DatabaseTreeModel::DatabaseTreeModel(QObject* parent)
    : QAbstractItemModel(parent), rootNode(0)
{
}

DatabaseTreeModel::~DatabaseTreeModel()
{
    delete rootNode;
}

void DatabaseTreeModel::setRootNode(DatabaseTreeNode *node)
{
    beginResetModel();
    delete rootNode;
    rootNode = node;
    endResetModel();
}

QModelIndex DatabaseTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if(!rootNode || row < 0 || column < 0)
        return QModelIndex();

    DatabaseTreeNode* parentNode = nodeFromIndex(parent);
    DatabaseTreeNode* childNode = parentNode->children.value(row);

    if(!childNode)
        return QModelIndex();

    return createIndex(row, column, childNode);
}

QModelIndex DatabaseTreeModel::parent(const QModelIndex &child) const
{
    DatabaseTreeNode* node = nodeFromIndex(child);
    if(!node)
        return QModelIndex();
    DatabaseTreeNode* parentNode = node->parent;
    if(!parentNode)
        return QModelIndex();
    DatabaseTreeNode* grandparentNode = parentNode->parent;
    if(!grandparentNode)
        return QModelIndex();

    int row = grandparentNode->children.indexOf(parentNode);
    return createIndex(row, 0, parentNode);
}

int DatabaseTreeModel::rowCount(const QModelIndex &parent) const
{
    if(parent.column() > 0)
        return 0;
    DatabaseTreeNode* parentNode = nodeFromIndex(parent);
    if(!parentNode)
        return 0;
    return parentNode->children.count();
}

int DatabaseTreeModel::columnCount(const QModelIndex &/*parent*/) const
{
    return 2;
}

QVariant DatabaseTreeModel::data(const QModelIndex &index, int role) const
{
    DatabaseTreeNode* node = nodeFromIndex(index);
    if(!node)
        return QVariant();

    if(role == Qt::DisplayRole) {
        if(index.column() == 0) {
            return node->name;
        } else if (index.column() == 1) {
            return node->type;
        }
    }

    if(role == Qt::DecorationRole)
    {
        if(index.column() == 0)
            return node->icon;
    }

    return QVariant();
}

QVariant DatabaseTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        if(section == 0) return tr("Name");
        else if(section == 1) return tr("Type");
    }

    return QVariant();
}

DatabaseTreeNode *DatabaseTreeModel::nodeFromIndex(const QModelIndex &index) const
{
    if(index.isValid()) {
        return static_cast<DatabaseTreeNode*>(index.internalPointer());
    } else {
        return rootNode;
    }
}

