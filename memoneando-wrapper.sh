#!/bin/bash

# Wrapper script para Memoneando
# Este script evita conflictos con bibliotecas snap
# Versión: 1.0.1

# Crear un entorno completamente limpio sin interferencias de snap
exec env -i \
    HOME="$HOME" \
    USER="$USER" \
    LOGNAME="$LOGNAME" \
    PATH="/usr/local/bin:/usr/bin:/bin:/usr/local/sbin:/usr/sbin:/sbin" \
    LD_LIBRARY_PATH="/lib/x86_64-linux-gnu:/usr/lib/x86_64-linux-gnu:/lib:/usr/lib" \
    DISPLAY="$DISPLAY" \
    XAUTHORITY="$XAUTHORITY" \
    XDG_RUNTIME_DIR="$XDG_RUNTIME_DIR" \
    XDG_SESSION_TYPE="$XDG_SESSION_TYPE" \
    QT_QPA_PLATFORM="xcb" \
    QT_PLUGIN_PATH="/usr/lib/x86_64-linux-gnu/qt6/plugins:/usr/lib/qt6/plugins" \
    QT_XCB_GL_INTEGRATION="xcb_egl" \
    QML_IMPORT_PATH="/usr/lib/x86_64-linux-gnu/qt6/qml" \
    QML2_IMPORT_PATH="/usr/lib/x86_64-linux-gnu/qt6/qml" \
    /usr/bin/memoneando-bin "$@"
