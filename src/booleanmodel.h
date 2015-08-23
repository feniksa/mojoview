#ifndef BOOLEAN_MODEL_H
#define BOOLEAN_MODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>

class Node;

class BooleanModel : public QAbstractItemModel 
{
	Q_OBJECT
public:
    explicit BooleanModel(QObject* parent = 0);
    ~BooleanModel();
	
	void setRootNode(Node* node);
	
	QModelIndex index (int row, int column, const QModelIndex& parent) const;
	QModelIndex parent(const QModelIndex& child) const;
	
	int rowCount(const QModelIndex& parent) const; 
	int columnCount(const QModelIndex& parent) const;
	QVariant data(const QModelIndex& index, int role) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role) const;
	
private:
	Node* nodeFromIndex(const QModelIndex& index) const;
	Node* rootNode;
};

#endif