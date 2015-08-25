#pragma once

#include <core/MojGmainReactor.h>
#include <core/MojReactorApp.h>
#include <core/MojServiceMessage.h>
#include <core/MojMessageDispatcher.h>
#include <luna/MojLunaService.h>
#include "MojClientHandler.h"


class MojAppMain : public MojReactorApp<MojGmainReactor>
{
public:
	using  Base = MojReactorApp<MojGmainReactor>;

	MojAppMain(MojUInt32 majorVersion = MajorVersion, MojUInt32 minorVersion = MinorVersion, const MojChar* versionString = 0);

	MojErr configure(const MojObject& conf);
	MojErr init();
	MojErr open();
	MojErr close();
	//MojErr run();

	MojDbServiceClient* client() { return m_dbClient.get(); }
	MojRefCountedPtr<MojDbClientHandler> handler() { return m_handler; }

private:
	static const MojChar* const VersionString;

	MojAutoPtr<MojLunaService> m_service;
	MojRefCountedPtr<MojDbServiceClient> m_dbClient;
	MojRefCountedPtr<MojDbClientHandler> m_handler;// (new MojDbClientHandler);

	MojMessageDispatcher m_dispatcher;
};
