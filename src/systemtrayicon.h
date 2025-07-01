#ifndef SYSTEMTRAYICON_H
#define SYSTEMTRAYICON_H

#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>

class MainWindow;

class SystemTrayIcon : public QSystemTrayIcon
{
    Q_OBJECT

public:
    explicit SystemTrayIcon(MainWindow *parent = nullptr);

private slots:
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    void showMainWindow();
    void quitApplication();

private:
    void createTrayMenu();

    MainWindow *m_mainWindow;
    QMenu *m_trayMenu;
    QAction *m_showAction;
    QAction *m_quitAction;
};

#endif // SYSTEMTRAYICON_H
