#include <QApplication>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    if (!QSystemTrayIcon::isSystemTrayAvailable())
        return 1;

    QMenu *menu = new QMenu();
    QAction *quitAction = new QAction(QObject::tr("&Quit"));
    QObject::connect(quitAction, &QAction::triggered, &app, &QCoreApplication::quit);
    menu->addAction(quitAction);

    QSystemTrayIcon *trayIcon = new QSystemTrayIcon();
    trayIcon->setIcon(QIcon::fromTheme("application-exit"));
    trayIcon->setContextMenu(menu);
    trayIcon->show();

    return app.exec();
}
