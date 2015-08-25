#pragma once

#include <db/MojDbClient.h>
#include <db/MojDbServiceClient.h>

class MojDbClientHandler : public MojSignalHandler
{
public:
	MojDbClientHandler();

	MojErr handleKindsList(MojObject& result, MojErr errCode);
	MojErr handleResult(MojObject& result, MojErr errCode);

	MojDbClient::Signal::Slot<MojDbClientHandler> m_slot;
	MojDbClient::Signal::Slot<MojDbClientHandler> m_slotKinds;

	MojObject m_kinds;
};
