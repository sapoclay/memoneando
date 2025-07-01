#!/bin/bash

# Script para crear el paquete .deb de Memoneando
# Autor: Memoneando Team
# Versión: 1.0.0

set -e

PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PACKAGE_DIR="$PROJECT_ROOT/debian-package"
BUILD_DIR="$PROJECT_ROOT/build"

echo "=== Construcción del paquete .deb de Memoneando ==="

# Limpiar compilación anterior
if [ -d "$BUILD_DIR" ]; then
    echo "Limpiando build anterior..."
    rm -rf "$BUILD_DIR"
fi

# Crear directorio de build
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

echo "Compilando Memoneando..."
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)

# Verificar que el ejecutable se creó
if [ ! -f "bin/Memoneando" ]; then
    echo "Error: No se pudo compilar Memoneando"
    exit 1
fi

echo "Preparando estructura del paquete..."

# Limpiar directorio del paquete
rm -rf "$PACKAGE_DIR/usr/bin/memoneando"
rm -rf "$PACKAGE_DIR/usr/share/applications/memoneando.desktop"
rm -rf "$PACKAGE_DIR/usr/share/icons/hicolor/48x48/apps/memoneando.png"

# Copiar ejecutable real con nombre diferente
cp "bin/Memoneando" "$PACKAGE_DIR/usr/bin/memoneando-bin"
chmod 755 "$PACKAGE_DIR/usr/bin/memoneando-bin"

# Copiar wrapper script como comando principal
cp "$PROJECT_ROOT/memoneando-wrapper.sh" "$PACKAGE_DIR/usr/bin/memoneando"
chmod 755 "$PACKAGE_DIR/usr/bin/memoneando"

# Copiar archivo desktop
cp "$PROJECT_ROOT/memoneando.desktop" "$PACKAGE_DIR/usr/share/applications/"
chmod 644 "$PACKAGE_DIR/usr/share/applications/memoneando.desktop"

# Copiar icono
cp "$PROJECT_ROOT/img/logo.png" "$PACKAGE_DIR/usr/share/icons/hicolor/48x48/apps/memoneando.png"
chmod 644 "$PACKAGE_DIR/usr/share/icons/hicolor/48x48/apps/memoneando.png"

# Copiar documentación
cp "$PROJECT_ROOT/README.md" "$PACKAGE_DIR/usr/share/doc/memoneando/"
chmod 644 "$PACKAGE_DIR/usr/share/doc/memoneando/README.md"

# Hacer ejecutables los scripts de control
chmod 755 "$PACKAGE_DIR/DEBIAN/postinst"
chmod 755 "$PACKAGE_DIR/DEBIAN/postrm"

echo "Calculando dependencias..."

# Calcular el tamaño instalado (en KB)
INSTALLED_SIZE=$(du -sk "$PACKAGE_DIR/usr" | cut -f1)
sed -i "/^Installed-Size:/d" "$PACKAGE_DIR/DEBIAN/control"
echo "Installed-Size: $INSTALLED_SIZE" >> "$PACKAGE_DIR/DEBIAN/control"

echo "Construyendo paquete .deb..."

# Extraer versión del archivo control
VERSION=$(grep "^Version:" "$PACKAGE_DIR/DEBIAN/control" | cut -d' ' -f2)
PACKAGE_NAME="memoneando_${VERSION}_amd64.deb"

# Crear el paquete .deb
cd "$PROJECT_ROOT"
dpkg-deb --build debian-package "$PACKAGE_NAME"

if [ $? -eq 0 ]; then
    echo ""
    echo "✅ ¡Paquete .deb creado exitosamente!"
    echo ""
    echo "📦 Archivo: $PACKAGE_NAME"
    echo "📏 Tamaño: $(du -h "$PACKAGE_NAME" | cut -f1)"
    echo ""
    echo "Para instalar:"
    echo "  sudo dpkg -i $PACKAGE_NAME"
    echo "  sudo apt-get install -f  # Si hay dependencias faltantes"
    echo ""
    echo "Para desinstalar:"
    echo "  sudo dpkg -r memoneando"
    echo ""
else
    echo "❌ Error al crear el paquete .deb"
    exit 1
fi
