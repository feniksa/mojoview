#include <QApplication>
#include "mainwindow.h"

#include "MojAppMain.h"

int main(int argc, char** argv)
{
    /*QApplication app(argc, argv);

    MainWindow mainwindow;
    mainwindow.show();

    return app.exec();*/

	MojErr err; // = test();

	/*if (err != MojErrNone)
		return -1;*/

	MojAutoPtr<MojAppMain> app(new MojAppMain);
	MojAllocCheck(app.get());

	return app->main(argc, argv);


}
