#include <QApplication>
#include "mainwindow.h"

#include "MojWorker.h"

int main(int argc, char** argv)
{
    /*QApplication app(argc, argv);

    MainWindow mainwindow;
    mainwindow.show();

    return app.exec();*/

	//MojErr err; // = test();

	/*if (err != MojErrNone)
		return -1;*/

	/*MojAutoPtr<MojAppMain> app(new MojAppMain);
	MojAllocCheck(app.get());

	return app->main(argc, argv);*/

	MojWorker mojWorker(argc, argv);
	mojWorker.start();

	QApplication app(argc, argv);

	MainWindow mainwindow(mojWorker.client());
	mainwindow.show();

	int res = app.exec();

	mojWorker.stop();
	mojWorker.wait();

	return res;
}
