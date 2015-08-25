#include "KindsModel.h"

KindsModel::KindsModel(MojDbClient* client, QObject* parent)
: QAbstractItemModel(parent),
  mojoClient(client),
  m_kindSlot(this, &KindsModel::handleKindsList)
{
	rootKind =  QSharedPointer<KindNode> (new KindNode {"com.adm:1", 1, "com.adm", nullptr });

	rootKind->subkinds.push_back(QSharedPointer<KindNode>(new KindNode {"com.palm.test:1", 1, "com.webos.service.tv.program", rootKind.data()}));

	QSharedPointer<KindNode> secondaryKind (new KindNode {"com.palm.app:1", 1, "com.webos.service.tv.program", rootKind.data()});
	secondaryKind->subkinds.push_back(QSharedPointer<KindNode>(new KindNode {"com.palm.app.test:3", 1, "com.webos.service.tv.program", secondaryKind.data()}));

	rootKind->subkinds.push_back(secondaryKind);
}

QVariant KindsModel::data(const QModelIndex& index, int role) const
{
	QVariant result;

	if (role != Qt::DisplayRole) {
		return QVariant();
	}

	KindNode* node = kindNodeFromIndex(index);
	if (!node)
		return QVariant();

	if (index.column() == 0) {
		return node->id;
	} else if (index.column() == 1) {
		return node->rev;
	} else if (index.column() == 2) {
		return node->owner;
	}

	return result;
}

QVariant KindsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
		if (section == 0) {
			return tr("Kind");
		}
		if (section == 1) {
			return tr("Rev");
		}
		if (section == 2) {
			return tr("Owner");
		}
	}

	return QVariant();
}

QModelIndex KindsModel::index(int row, int column, const QModelIndex& parent) const
{
	if (row < 0 || column < 0)
		return QModelIndex();

	KindNode* kind = kindNodeFromIndex(parent);

	if (row >= kind->subkinds.size()) {
		return QModelIndex();
	}

	KindNode* childNode = kind->subkinds.at(row).data();
	return createIndex(row, column, childNode);

}

QModelIndex KindsModel::parent(const QModelIndex& child) const
{
	KindNode* node = kindNodeFromIndex(child);
	if (!node)
		return QModelIndex();

	KindNode* parent = node->parent;
	if (!parent)
		return QModelIndex();

	KindNode* grandParentNode = parent->parent;
	if (!grandParentNode)
		return QModelIndex();

	int row;
	for (row = 0; row < grandParentNode->subkinds.size(); ++row) {
		if (grandParentNode->subkinds[row].data() == parent)
			break;
	}

	return createIndex(row, 0, parent);
}

int KindsModel::columnCount(const QModelIndex& parent) const
{
	return 3;	// id, rev, owner
}

int KindsModel::rowCount(const QModelIndex& parent) const
{
	if (parent.column() > 0)
		return 0;

	KindNode* node = kindNodeFromIndex(parent);
	if (!node)
		return 0;

	return node->subkinds.count();
}

KindNode* KindsModel::kindNodeFromIndex(const QModelIndex& index) const
{
	if (index.isValid())
		return static_cast<KindNode*>(index.internalPointer());

	return rootKind.data();
}

void KindsModel::load()
{
	MojErr err;
	err = mojoClient->kinds(m_kindSlot);
}

MojErr KindsModel::handleKindsList(MojObject& result, MojErr errCode)
{
	MojString str;
	result.toJson(str);

	result.getRequired(_T("results"))

	return MojErrNone;
}

