#!/bin/bash

# Script de construcción para Memoneando
# Autor: Memoneando Team
# Versión: 1.0

set -e

echo "🎨 Construyendo Memoneando - Editor de Memes..."
echo "=============================================="

# Verificar que estamos en el directorio correcto
if [ ! -f "CMakeLists.txt" ]; then
    echo "❌ Error: No se encontró CMakeLists.txt. Ejecuta este script desde el directorio raíz del proyecto."
    exit 1
fi

# Verificar dependencias
echo "📋 Verificando dependencias..."

# Verificar CMake
if ! command -v cmake &> /dev/null; then
    echo "❌ Error: CMake no está instalado."
    echo "   Ubuntu/Debian: sudo apt install cmake"
    echo "   Fedora/RHEL: sudo dnf install cmake"
    exit 1
fi

# Verificar Qt6
if ! pkg-config --exists Qt6Core; then
    echo "❌ Error: Qt6 no está instalado."
    echo "   Ubuntu/Debian: sudo apt install qt6-base-dev qt6-tools-dev"
    echo "   Fedora/RHEL: sudo dnf install qt6-qtbase-devel qt6-qttools-devel"
    exit 1
fi

echo "✅ Dependencias verificadas correctamente"

# Crear directorio de construcción
echo "📁 Preparando directorio de construcción..."
if [ -d "build" ]; then
    echo "   Limpiando construcción anterior..."
    rm -rf build
fi

mkdir build
cd build

# Configurar con CMake
echo "⚙️ Configurando proyecto con CMake..."
cmake .. -DCMAKE_BUILD_TYPE=Release

# Compilar
echo "🔨 Compilando Memoneando..."
make -j$(nproc)

# Verificar que se compiló correctamente
if [ -f "bin/Memoneando" ]; then
    echo ""
    echo "🎉 ¡Compilación exitosa!"
    echo "   Ejecutable: $(pwd)/bin/Memoneando"
    echo ""
    echo "Para ejecutar Memoneando:"
    echo "   cd build && ./bin/Memoneando"
    echo ""
    echo "O desde el directorio raíz:"
    echo "   ./build/bin/Memoneando"
    echo ""
    echo "¡Disfruta creando memes! 🎭"
else
    echo "❌ Error: La compilación falló. Revisa los mensajes de error arriba."
    exit 1
fi
