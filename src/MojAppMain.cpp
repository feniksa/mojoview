#include "MojAppMain.h"

const MojChar* const MojAppMain::VersionString = "0.0.1";

MojAppMain::MojAppMain(MojUInt32 majorVersion, MojUInt32 minorVersion, const MojChar* versionString)
: MojReactorApp<MojGmainReactor>(MajorVersion, MinorVersion, VersionString)
{
}

MojErr MojAppMain::configure(const MojObject& conf)
{
	MojErr err = Base::configure(conf);
	MojErrCheck(err);

	return MojErrNone;
}

MojErr MojAppMain::init()
{
	MojErr err = Base::init();
	MojErrCheck(err);

	m_service.reset(new MojLunaService);
	m_dbClient.reset(new MojDbServiceClient(m_service.get()));
	m_handler.reset(new MojDbClientHandler);

	return MojErrNone;
}

MojErr MojAppMain::open()
{
	MojErr err = Base::open();
	MojErrCheck(err);

	// start message queue thread pool
	err = m_dispatcher.start(2);
	MojErrCheck(err);

	err = m_service->open(_T("com.palm.configurator"));
	MojErrCheck(err);

	err = m_service->attach(m_reactor.impl());
	MojErrCheck(err);

	return MojErrNone;
}

MojErr MojAppMain::close()
{
	// stop dispatcher
	MojErr err = MojErrNone;
	MojErr errClose = m_dispatcher.stop();
	MojErrAccumulate(err, errClose);
	errClose = m_dispatcher.wait();
	MojErrAccumulate(err, errClose);

	errClose = Base::close();
	MojErrAccumulate(err, errClose);

	return MojErrNone;
}
/*
MojErr MojAppMain::run()
{
	MojErr err;

	err = m_dbClient->kinds(m_handler->m_slotKinds);
	MojErrCheck(err);

	MojDbQuery q;
	err = q.from(_T("com.200volts.mojoview:1"));
	MojErrCheck(err);

	MojString bar;
	err = bar.assign(_T("aaa"));
	MojErrCheck(err);

	err = q.where(_T("bar"), MojDbQuery::OpEq, bar);
	MojErrCheck(err);

	err = m_dbClient->find(m_handler->m_slot, q);
	MojErrCheck(err);

	err = Base::run();
	MojErrCheck(err);

	return MojErrNone;
}*/
