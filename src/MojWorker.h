#pragma once

#include <QtCore/QThread>
#include "MojAppMain.h"

class MojWorker : public QThread
{
	 Q_OBJECT

public:
	MojWorker(int argc, char** argv)
	: m_argc(argc),
	  m_argv(argv)
	{
	}

	void run() override
	{
		 m_app.reset(new MojAppMain);
		 m_exitStatus = m_app->main(m_argc, m_argv);
	}

	void stop()
	{
		m_lastErr = m_app->close();
		m_app->shutdown();
	}

	MojDbClient* client() { return m_app->client(); }

private:
	int m_argc;
	char** m_argv;
	MojAutoPtr<MojAppMain> m_app;
	MojErr m_lastErr;
	int m_exitStatus;
};
