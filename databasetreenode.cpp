#include "databasetreenode.h"

DatabaseTreeNode::DatabaseTreeNode(const QString& name, const QString& type)
    : name(name), type(type), parent(0)
{
}

DatabaseTreeNode::~DatabaseTreeNode()
{
    qDeleteAll(children);
}

