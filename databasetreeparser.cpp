#include "databasetreeparser.h"
#include <QDebug>

DatabaseTreeParser::DatabaseTreeParser()
{

}

DatabaseTreeNode *DatabaseTreeParser::parse(const QSqlDatabase &database)
{
    TableInfoReaderFactory factory;
    TableInfoReader* reader = factory.createTableReader(database);

    QStringList tables = database.tables();

    //Root node is not displayed, so we need to create root subnode

    DatabaseTreeNode* rootNode = new DatabaseTreeNode("root", "root");

    DatabaseTreeNode* treeRootNode = new DatabaseTreeNode(QString("Tables (%1)").arg(tables.size()), QString());
    treeRootNode->parent = rootNode;
    rootNode->children.push_back(treeRootNode);
    qDebug() << treeRootNode->name << " " << treeRootNode->type;

    for(int i=0; i<tables.size(); ++i)
    {
        QList<ColumnInfo> columnInfo = reader->retrieveTableInfo(tables.at(i));

        DatabaseTreeNode* tableNode = new DatabaseTreeNode(tables.at(i), "Table");
        tableNode->parent = treeRootNode;
        tableNode->icon = QIcon(":/img/icon-table.png");
        treeRootNode->children.push_back(tableNode);

        qDebug() << "\t" << tableNode->name << " " << tableNode->type;

        for(int j=0; j<columnInfo.size(); ++j)
        {
            DatabaseTreeNode* fieldNode = new DatabaseTreeNode(columnInfo.at(j).name(), columnInfo.at(j).type());
            fieldNode->parent = tableNode;
            fieldNode->icon = QIcon(":/img/icon-field.png");
            tableNode->children.push_back(fieldNode);

            qDebug() << "\t\t" << fieldNode->name << " " << fieldNode->type;
        }
    }

    return rootNode;
}

