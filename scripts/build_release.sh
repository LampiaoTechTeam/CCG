#!/bin/bash

set -e

mkdir -p dist

echo "Building Linux binary..."
./mkall_linux.sh
cp bin/card_game dist/card_game-linux-amd64

echo "Building Windows binary..."
./mkall_win32.sh
cp bin/card_game.exe dist/card_game-windows-amd64.exe

echo "Build complete! Binaries are in the dist/ directory:"
ls -la dist/

echo "Testing Linux binary:"

echo "Windows binary created: dist/card_game-windows-amd64.exe"
echo "You can test it with Wine if available: wine dist/card_game-windows-amd64.exe --help"
