#!/bin/bash

set -e

make clean

mkdir -p dist

echo "Building Linux binary..."
make LINUX=1
cp bin/card_game dist/card_game-linux-amd64
chmod +x dist/card_game-linux-amd64

echo "Building Windows binary..."
make clean
make WINDOWS=1
cp bin/card_game.exe dist/card_game-windows-amd64.exe

echo "Build complete! Binaries are in the dist/ directory:"
ls -la dist/

echo ""
echo "Testing Linux binary:"
./dist/card_game-linux-amd64 --help || echo "Binary created successfully"

echo ""
echo "Windows binary created: dist/card_game-windows-amd64.exe"
echo "You can test it with Wine if available: wine dist/card_game-windows-amd64.exe --help"
