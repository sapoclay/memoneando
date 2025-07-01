#include "systemtrayicon.h"
#include "mainwindow.h"
#include <QApplication>
#include <QIcon>

SystemTrayIcon::SystemTrayIcon(MainWindow *parent)
    : QSystemTrayIcon(parent)
    , m_mainWindow(parent)
{
    // Configurar el ícono
    setIcon(QIcon(":/icons/logo.png"));
    setToolTip("Memoneando - Editor de Memes");
    
    createTrayMenu();
    
    // Conectar señales
    connect(this, &QSystemTrayIcon::activated, this, &SystemTrayIcon::iconActivated);
}

void SystemTrayIcon::createTrayMenu()
{
    m_trayMenu = new QMenu();
    
    m_showAction = new QAction("Mostrar Memoneando", this);
    connect(m_showAction, &QAction::triggered, this, &SystemTrayIcon::showMainWindow);
    
    m_quitAction = new QAction("Salir", this);
    connect(m_quitAction, &QAction::triggered, this, &SystemTrayIcon::quitApplication);
    
    m_trayMenu->addAction(m_showAction);
    m_trayMenu->addSeparator();
    m_trayMenu->addAction(m_quitAction);
    
    setContextMenu(m_trayMenu);
}

void SystemTrayIcon::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::DoubleClick:
        showMainWindow();
        break;
    case QSystemTrayIcon::MiddleClick:
        showMessage("Memoneando", "Editor de memes activo en la bandeja del sistema",
                   QSystemTrayIcon::Information, 2000);
        break;
    default:
        break;
    }
}

void SystemTrayIcon::showMainWindow()
{
    if (m_mainWindow) {
        m_mainWindow->show();
        m_mainWindow->raise();
        m_mainWindow->activateWindow();
    }
}

void SystemTrayIcon::quitApplication()
{
    if (m_mainWindow) {
        m_mainWindow->close();
    }
    QApplication::quit();
}
