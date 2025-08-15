#!/bin/bash
echo "=== UtecCompiler Auto-Build Installer by Luis"
# Crea carpeta temporal
WORKDIR="$HOME/uteccompiler-build"
mkdir -p "$WORKDIR"
cd "$WORKDIR"
# Clona tu repo de código fuente
echo "=== Cloning source code..."
git clone https://github.com/LuisEnriqueCortijoGonzales/uteccompiler-repo-source.git .
# Compila UtecC y UtecCop
echo "=== Compiling UtecC..."
g++ main.cpp parser.cpp scanner.cpp token.cpp visitor.cpp exp.cpp -o UtecC
echo "=== Compiling UtecCop..."
g++ main.cpp parser.cpp scanner.cpp token.cpp visitor.cpp exp.cpp -o UtecCop
# Instala en /usr/local/bin (requiere sudo)
echo "=== Installing executables..."
sudo mv UtecC /usr/local/bin/
sudo mv UtecCop /usr/local/bin/
# Limpia
echo "=== Cleaning up..."
cd ~
rm -rf "$WORKDIR"
echo " Done!"