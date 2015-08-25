#pragma once

#include <QtCore/QAbstractItemModel>
#include <QtCore/QSharedPointer>
#include <QtCore/QVector>

#include "core/MojSignal.h"
#include "db/MojDbClient.h"

struct KindNode
{
	QString id;
	quint32 rev;
	QString owner;
	KindNode* parent;
	QVector<QSharedPointer<KindNode>> subkinds;
	// indexes
	// subkinds
};


class KindsModel : public QAbstractItemModel, public MojSignalHandler
{
	Q_OBJECT
public:
	explicit KindsModel(MojDbClient* client, QObject* parent = 0);

	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

	QModelIndex index(int row, int column, const QModelIndex& parent) const override;

	QModelIndex parent(const QModelIndex& child) const override;

	int columnCount(const QModelIndex& parent = QModelIndex()) const override;
	int rowCount(const QModelIndex& parent = QModelIndex()) const override;

	void load();
private:
	KindNode* kindNodeFromIndex(const QModelIndex& index) const;

	MojErr handleKindsList(MojObject& result, MojErr errCode);

	QSharedPointer<KindNode> rootKind;
	MojDbClient* mojoClient;
	MojDbClient::Signal::Slot<KindsModel> m_kindSlot;
};


