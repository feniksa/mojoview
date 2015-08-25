#pragma once

#include <db/MojDbClient.h>
#include <db/MojDbServiceClient.h>

class MojDbClientHandler : public MojSignalHandler
{
public:
	MojDbClientHandler()
	: m_dbErr(MojErrNone),
	m_callbackInvoked (false),
	m_slot(this, &MojDbClientHandler::handleResult),
	m_slotKinds(this, &MojDbClientHandler::handleKindsList)
	{
	}

	MojErr handleKindsList(MojObject& result, MojErr errCode)
	{
		MojErr err;
		MojErrCatchAll(errCode) {
			MojString what;
			err = MojErrToString(errCode, what);
			MojErrCheck(err);

			// notify about error
			return MojErrNone;
		}

		// parse result

		return MojErrNone;
	}

	MojErr handleResult(MojObject& result, MojErr errCode)
	{
		MojErr err;

		MojString str;
		err = result.stringValue(str);
		MojErrCheck(err);

		std::cerr << str.data() << std::endl;

		m_callbackInvoked = true;
		m_result = result;
		m_dbErr = errCode;
		if (errCode != MojErrNone) {
			bool found = false;
			err = result.get(MojServiceMessage::ErrorTextKey, m_errTxt, found);
			MojErrCheck(err);

		}
		return MojErrNone;
	}

	void reset()
	{
		m_dbErr = MojErrNone;
		m_callbackInvoked = false;
		m_errTxt.clear();
		m_result.clear();
	}


	MojErr m_dbErr;
	MojString m_errTxt;
	MojObject m_result;
	bool m_callbackInvoked;
	MojDbClient::Signal::Slot<MojDbClientHandler> m_slot;
	MojDbClient::Signal::Slot<MojDbClientHandler> m_slotKinds;

};
