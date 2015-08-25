#include "mainwindow.h"

#include <QDebug>
#include <QSettings>

#include <QtGui/QTreeView>
#include <QtGui/QPushButton>
#include <QtGui/QAction>

#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QMessageBox>
#include <QtGui/QStyle>

#include <QtGui/QCloseEvent>
#include <QtGui/QToolBar>
#include <QtGui/QProgressBar>
#include <QtGui/QVBoxLayout>

#include <QtGui/QStandardItemModel>
#include "KindsModel.h"

MainWindow::MainWindow(MojDbClient* client, QWidget* parent, Qt::WindowFlags flags):QMainWindow(parent, flags)
{
    QWidget* mainWidget = new QWidget();

    progressBar = new QProgressBar;

    createActions();
    createMenus();
    createToolBar();

	KindsModel* kindsModel = new KindsModel(client, this);

	treeView = new QTreeView();
	treeView->setModel(kindsModel);

	mainLayout = new QVBoxLayout;
	mainLayout->addWidget(treeView);

	mainWidget->setLayout(mainLayout);

    setCentralWidget(mainWidget);

    setWindowTitle(tr("MojoDB admin"));

    loadSettings();

	kindsModel->load();
}

void MainWindow::createActions()
{
    exitAction = new QAction(tr("&Quit"), this);
    exitAction->setStatusTip(tr("Exit from this program"));
    exitAction->setShortcut(QKeySequence::Quit);
    exitAction->setIcon(style()->standardIcon(QStyle::SP_TitleBarCloseButton));
    connect(exitAction, SIGNAL(triggered()), SLOT(close()));

    addConnectionActions = new QAction(tr("&Open"), this);
    addConnectionActions->setStatusTip(tr("Open connection to mojo database"));
    addConnectionActions->setShortcut(QKeySequence::Open);
    addConnectionActions->setIcon(style()->standardIcon(QStyle::SP_DialogOpenButton));

    settingsAction = new QAction(tr("&Settings"), this);
    settingsAction->setStatusTip(tr("Settings for program"));
    connect(settingsAction, SIGNAL(triggered()), SLOT(settingsSlot()));

    aboutAction = new QAction(tr("About"), this);
    aboutAction->setStatusTip(tr("Show information about this program"));
    aboutAction->setIcon(style()->standardIcon(QStyle::SP_MessageBoxInformation));
    connect(aboutAction, SIGNAL(triggered()), SLOT(aboutSlot()));
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(addConnectionActions);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    settingsMenu = menuBar()->addMenu(tr("&Preferences"));
    settingsMenu->addAction(settingsAction);

    aboutMenu = menuBar()->addMenu(tr("&Help"));
    aboutMenu->addAction(aboutAction);
}

void MainWindow::createToolBar()
{
    actionsToolbar = addToolBar(tr("Actions"));
    actionsToolbar->addAction(addConnectionActions);
}

void MainWindow::aboutSlot()
{
    QMessageBox::information(this, tr("About"), tr( "MojDB viewer. <br><br>Author Maksym Sditanov (<email>msditanov@200volts.com</email>)"
                             "<br><a href=\"http://github.com/feniksa/mojoview\">http://github.com/feniksa/mojoview</a>"));
}

void MainWindow::settingsSlot()
{
    /*settings settingswindow;

    settingswindow.setMessage(message);
    settingswindow.setUserName(userName);
    settingswindow.setPassword(userPassword);

    if (settingswindow.exec() == QDialog::Accepted) {
        message = settingswindow.getMessage();
        userName = settingswindow.getUserName();
        userPassword = settingswindow.getPassword();

        saveSettings();
    }*/

	saveSettings();
}

void MainWindow::loadSettings()
{
    QSettings settings("200volts", "Dating");

    settings.beginGroup("mainwindow");
    resize(settings.value("size", sizeHint()).toSize());
    bool maximazied = settings.value("maximazed", true).toBool();
    if (maximazied)
        showMaximized();
    settings.endGroup();

    settings.beginGroup("settings");
    /*message = settings.value("message", tr("Hello")).toString();
    userName = settings.value("user").toString();
    userPassword = settings.value("password").toString();*/
    settings.endGroup();
}

void MainWindow::saveSettings()
{
    QSettings settings("200volts.com", "MojoDB");

    settings.beginGroup("mainwindow");
    settings.setValue("size", size());
    settings.setValue("maximazed", isMaximized());
    settings.endGroup();

    settings.beginGroup("settings");
    settings.endGroup();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (userReallyWantsToQuit()) {
        saveSettings();
        event->accept();
    } else {
        event->ignore();
    }
}

bool MainWindow::userReallyWantsToQuit()
{
    int res = QMessageBox::question(  this,
                            tr("Quit?"),
                            tr("Do you realy want to quit?"),
                            QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Cancel);

    return (res == QMessageBox::Ok) ? true : false;
}
