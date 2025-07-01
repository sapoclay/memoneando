#!/bin/bash

# Script para ejecutar Memoneando en un entorno completamente limpio
# sin interferencias de snap

echo "🧹 Limpiando entorno de bibliotecas..."

# Crear un entorno completamente limpio
env -i \
    HOME="$HOME" \
    USER="$USER" \
    PATH="/usr/local/bin:/usr/bin:/bin" \
    LD_LIBRARY_PATH="/lib/x86_64-linux-gnu:/usr/lib/x86_64-linux-gnu" \
    DISPLAY="$DISPLAY" \
    XAUTHORITY="$XAUTHORITY" \
    XDG_RUNTIME_DIR="$XDG_RUNTIME_DIR" \
    QT_QPA_PLATFORM="xcb" \
    QT_PLUGIN_PATH="/usr/lib/x86_64-linux-gnu/qt6/plugins" \
    bash -c '
        echo "🚀 Iniciando Memoneando en entorno limpio..."
        echo "📚 LD_LIBRARY_PATH: $LD_LIBRARY_PATH"
        cd "/var/www/html/C++/memes"
        ./build/bin/Memoneando "$@"
    '
