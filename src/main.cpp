#include <QApplication>
#include <QSystemTrayIcon>
#include <QMessageBox>
#include <QStyleFactory>
#include <QDir>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // Configurar información de la aplicación
    app.setApplicationName("Memoneando");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("Memoneando Team");
    app.setApplicationDisplayName("Memoneando - Editor de Memes");
    
    // Verificar si el sistema soporta bandeja del sistema
    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        QMessageBox::critical(nullptr, QObject::tr("Bandeja del Sistema"),
                            QObject::tr("No se pudo detectar la bandeja del sistema en este sistema."));
        return 1;
    }
    
    // Prevenir que la aplicación se cierre cuando se cierre la última ventana
    QApplication::setQuitOnLastWindowClosed(false);
    
    // Crear y mostrar la ventana principal
    MainWindow window;
    window.show();
    
    return app.exec();
}
