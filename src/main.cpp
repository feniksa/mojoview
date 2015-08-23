#include <QApplication>
#include "mainwindow.h"

#include <iostream>

#include <core/MojGmainReactor.h>
#include <core/MojReactorApp.h>
#include <core/MojServiceMessage.h>
#include <db/MojDbClient.h>
#include <db/MojDbServiceClient.h>
#include <luna/MojLunaService.h>

#include <core/MojMessageDispatcher.h>

class MojDbClientHandler : public MojSignalHandler
{
public:
	MojDbClientHandler()
	: m_dbErr(MojErrNone),
	m_callbackInvoked (false),
	m_slot(this, &MojDbClientHandler::handleResult)
	{
	}

	MojErr handleResult(MojObject& result, MojErr errCode)
	{
		m_callbackInvoked = true;
		m_result = result;
		m_dbErr = errCode;
		if (errCode != MojErrNone) {
			bool found = false;
			MojErr err = result.get(MojServiceMessage::ErrorTextKey, m_errTxt, found);
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

	MojErr wait(MojService* service)
	{
		while (!m_callbackInvoked) {
			MojErr err = service->dispatch();
			MojErrCheck(err);
		}
		return MojErrNone;
	}

	MojErr m_dbErr;
	MojString m_errTxt;
	MojObject m_result;
	bool m_callbackInvoked;
	MojDbClient::Signal::Slot<MojDbClientHandler> m_slot;

};

const char* json = _T("{")
				_T("\"id\":\"com.test.test:1\",")
				_T("\"owner\":\"com.test.test\",")
				_T("\"indexes\":[")
						_T("{\"name\":\"foo\", \"props\":[{\"name\":\"foo\"}]},")
						_T("{\"name\":\"barfoo\",\"props\":[{\"name\":\"bar\"},{\"name\":\"foo\"}] }")
					_T("]")
				_T("}");

class MojDbLunaServiceApp : public MojReactorApp<MojGmainReactor>
{
public:
	using  Base = MojReactorApp<MojGmainReactor>;

	MojDbLunaServiceApp(MojUInt32 majorVersion = MajorVersion, MojUInt32 minorVersion = MinorVersion, const MojChar* versionString = 0)
	: MojReactorApp<MojGmainReactor>(MajorVersion, MinorVersion, VersionString)
	{
	}

	MojErr configure(const MojObject& conf) override
	{
		MojErr err = Base::configure(conf);
		MojErrCheck(err);

		return MojErrNone;
	}

	MojErr init() override
	{
		MojErr err = Base::init();
		MojErrCheck(err);

		return MojErrNone;
	}

	MojErr open() override {
		MojErr err = Base::open();
		MojErrCheck(err);

		// start message queue thread pool
		err = m_dispatcher.start(2);
		MojErrCheck(err);

		return MojErrNone;
	}
	MojErr close() override
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

private:
	static const MojChar* const VersionString;

	MojAutoPtr<MojDbServiceClient> m_dbClient;
	//(new MojDbServiceClient(m_service.get()));
	//MojRefCountedPtr<MojDbClientHandler> handler (new MojDbClientHandler);

	MojMessageDispatcher m_dispatcher;
};

const MojChar* const MojDbLunaServiceApp::VersionString = "0.0.1";

MojErr test()
{
	MojErr err;

	MojAutoPtr<MojGmainReactor> reactor(new MojGmainReactor);
	err = reactor->init();
	MojErrCheck(err);

	MojAutoPtr<MojLunaService> service(new MojLunaService);
	err = service->open(_T("com.palm.configurator"));
	MojErrCheck(err);

	err = service->attach(reactor->impl());
	MojErrCheck(err);

	MojAutoPtr<MojDbServiceClient> m_dbClient(new MojDbServiceClient(service.get()));
	MojRefCountedPtr<MojDbClientHandler> handler (new MojDbClientHandler);

	MojObject obj;
	err = obj.fromJson(json);
	MojErrCheck(err);


	err = m_dbClient->putKind(handler->m_slot, obj);
	MojErrCheck(err);

	// block until repsonse received
	err = handler->wait(m_dbClient->service());
	MojErrCheck(err);

	MojString str;
	err = handler->m_result.stringValue(str);
	MojErrCheck(err);

	std::cerr << str.data() << std::endl;
	handler->reset();


	// --------------------------------------------------------------------
	// select obj via query
	MojDbQuery q;
	err = q.from(_T("com.test.test:1"));
	MojErrCheck(err);

	MojString bar;
	err = bar.assign(_T("aaa"));
	MojErrCheck(err);

	err = q.where(_T("bar"), MojDbQuery::OpEq, bar);
	MojErrCheck(err);

	err = m_dbClient->find(handler->m_slot, q);
	MojErrCheck(err);

	// block until repsonse received
	err = handler->wait(m_dbClient->service());
	MojErrCheck(err);

	err = handler->m_result.stringValue(str);
	MojErrCheck(err);

	std::cerr << str.data() << std::endl;
	handler->reset();
}

int main(int argc, char** argv)
{
    /*QApplication app(argc, argv);

    MainWindow mainwindow;
    mainwindow.show();

    return app.exec();*/

	MojErr err; // = test();

	/*if (err != MojErrNone)
		return -1;*/

	MojAutoPtr<MojDbLunaServiceApp> app(new MojDbLunaServiceApp);
	MojAllocCheck(app.get());

	return app->main(argc, argv);


}
