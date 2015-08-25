#pragma once

#include <QtGui/QMainWindow>

class QWebView;
class QTimer;
class QAction;
class QMenu;
class QToolBar;
class QVBoxLayout;
class QProgressBar;
class QTreeView;

class MojDbClient;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
	explicit MainWindow(MojDbClient* client, QWidget* parent = 0, Qt::WindowFlags flags = 0);

protected:
    void closeEvent(QCloseEvent *event);

private slots:

    void aboutSlot();
    void settingsSlot();

private:
    void createActions();
    void createMenus();
    void createToolBar();

    void saveSettings();
    void loadSettings();

    bool userReallyWantsToQuit();

    QAction* exitAction;
    QAction* addConnectionActions;
    QAction* settingsAction;
    QAction* aboutAction;

    QMenu* fileMenu;
    QMenu* settingsMenu;
    QMenu* aboutMenu;

    QToolBar* actionsToolbar;
    QVBoxLayout* mainLayout;
    QProgressBar* progressBar;

    QTreeView *treeView;
};
