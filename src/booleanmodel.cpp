#include "booleanmodel.h"
#include "node.h"

BooleanModel::BooleanModel(QObject* parent): QAbstractItemModel(parent)
{
	rootNode = 0;
}

BooleanModel::~BooleanModel()
{
	if (rootNode)
		delete rootNode;
}

void BooleanModel::setRootNode(Node* node)
{
	if (rootNode)
		delete rootNode;
	rootNode = node;
	reset();
}

QModelIndex BooleanModel::index(int row, int column, const QModelIndex& parent) const
{
	if (!rootNode || row < 0 || column < 0)
		return QModelIndex();
	
	Node* parentNode = nodeFromIndex(parent);
	Node* childNode = parentNode->children.value(row);
	
	if (!childNode)
		return QModelIndex();	
	
	return createIndex(row, column, childNode);	
}

Node* BooleanModel::nodeFromIndex(const QModelIndex& index) const 
{
	if (index.isValid())
		return static_cast<Node*>(index.internalPointer());
	
	return rootNode;
}

int BooleanModel::rowCount(const QModelIndex& parent) const
{
	if (parent.column() > 0)
		return 0;
	Node* parentNode = nodeFromIndex(parent);
	if (!parentNode)
		return 0;
	return parentNode->children.count();	
}

int BooleanModel::columnCount(const QModelIndex&/* parent*/) const
{
	return 2;	/* first column - type of headers, second - value */
}

QModelIndex BooleanModel::parent(const QModelIndex& child) const
{
	Node* node = nodeFromIndex(child);
	if (!node)
		return QModelIndex();
	
	Node* parentNode = node->parent;
	if (!parentNode)
		return QModelIndex();
	
	Node* grandParentNode = parentNode->parent;
	if (!grandParentNode)
		return QModelIndex();
	
	int row = grandParentNode->children.indexOf(parentNode);
	return createIndex(row, 0, parentNode);
}

QVariant BooleanModel::data(const QModelIndex& index, int role) const
{
	if (role != Qt::DisplayRole)
		return QVariant();
	
	Node* node = nodeFromIndex(index);
	if (!node)
		return QVariant();
	
	if (index.column() == 0) {
		switch (node->type) {
			case Node::Root:
				return tr("Root");
			case Node::OrExpression:
				return tr("OR Expression");
			case Node::AndExpression:
				return tr("AND Expression");
			case Node::NotExpression:
				return tr("NOT Expression");
			case Node::Atom:
				return tr("Atom");
			case Node::Identifier:
				return tr("Identifier");
			case Node::Operator:
				return tr("Operator");
			case Node::Punctuator:
				return tr("Punctuator");
			default:
				return tr("unknown");
		}
	}
	else if (index.column() == 1)
		return node->str;
	
	return QVariant();
}

QVariant BooleanModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
		if (section == 0) 
			return tr("Node");
		else if (section == 1)
			return tr("Value");
	}
    return QVariant();
}






