#include "MojClientHandler.h"

MojDbClientHandler::MojDbClientHandler()
: m_slot(this, &MojDbClientHandler::handleResult),
  m_slotKinds(this, &MojDbClientHandler::handleKindsList)
{
}

MojErr MojDbClientHandler::handleKindsList(MojObject& result, MojErr errCode)
{
	MojErr err;
	MojErrCatchAll(errCode) {
		MojString what;
		err = MojErrToString(errCode, what);
		MojErrCheck(err);

		/*if (errCode != MojErrNone) {
			bool found = false;
			err = result.get(MojServiceMessage::ErrorTextKey, m_errTxt, found);
			MojErrCheck(err);
		}*/

		// notify about error
		return MojErrNone;
	}

	err = result.getRequired(_T("result"), m_kinds);
	MojErrCheck(err);

	// parse result

	return MojErrNone;
}

MojErr MojDbClientHandler::handleResult(MojObject& result, MojErr errCode)
{
	MojErr err;

	return MojErrNone;
}
