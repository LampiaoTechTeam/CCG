#!/bin/sh

echo "### BOOTSTRAP WINDOWS (EXECUÇÃO REAL)"
echo

die() {
  echo "ERRO: $1"
  exit 1
}

run() {
  echo "[RUN] $*"
  "$@"
}

# --------------------------------------------------
# 1) Detectar ambiente (OBRIGATÓRIO MSYS2)
# --------------------------------------------------
OS="$(uname -s)"

case "$OS" in
  MINGW*|MSYS*)
    echo "[OK] MSYS2 detectado: $OS"
    ;;
  *)
    die "Este projeto requer MSYS2 (bash). Não use cmd.exe ou PowerShell."
    ;;
esac

# Aviso educativo (não bloqueante)
if echo "$OS" | grep -qi MSYS; then
  echo "[WARN] Você está no shell MSYS."
  echo "       Recomendado: usar o shell MINGW64 para compilar."
fi

# --------------------------------------------------
# 2) Ferramentas obrigatórias
# --------------------------------------------------
command -v pacman >/dev/null 2>&1 || die "pacman não encontrado"
echo "[OK] pacman encontrado"

command -v gcc >/dev/null 2>&1 || die "gcc não encontrado"
echo "[OK] gcc encontrado"

command -v pkg-config >/dev/null 2>&1 || die "pkg-config não encontrado"
echo "[OK] pkg-config encontrado"

# --------------------------------------------------
# 3) Prefixo de build (MINGW64 é regra)
# --------------------------------------------------
MSYS_PREFIX="/mingw64"

MSYS_WIN_PREFIX="$(cd $MSYS_PREFIX 2>/dev/null && pwd -W)"
[ -z "$MSYS_WIN_PREFIX" ] && die "Falha ao resolver caminho Windows do prefixo"

echo "[OK] Prefixo POSIX : $MSYS_PREFIX"
echo "[OK] Prefixo WIN32 : $MSYS_WIN_PREFIX"

# --------------------------------------------------
# 4) Instalar dependências (real)
# --------------------------------------------------
echo
echo "### INSTALANDO DEPENDÊNCIAS"
run pacman -Sy
run pacman -S --needed \
  mingw-w64-x86_64-SDL2 \
  mingw-w64-x86_64-SDL2_image \
  mingw-w64-x86_64-SDL2_ttf \
  mingw-w64-x86_64-libxml2

# --------------------------------------------------
# 5) Coletar flags (REAIS)
# --------------------------------------------------
if pkg-config --exists sdl2; then
  echo "[OK] SDL2 detectado via pkg-config"
else
  die "SDL2 não encontrado (pkg-config)"
fi

SDL_CFLAGS="$(pkg-config --cflags sdl2 SDL2_image SDL2_ttf)"
SDL_LIBS="$(pkg-config --libs sdl2 SDL2_image SDL2_ttf)"

XML_CFLAGS="-I$MSYS_PREFIX/include/libxml2"
XML_LIBS="-lxml2"

# --------------------------------------------------
# 6) Gerar build/config.mk (real)
# --------------------------------------------------
echo
echo "### GERANDO build/config.mk"

run mkdir -p build

cat > build/config.mk <<EOF
MSYS_PREFIX     = $MSYS_PREFIX
MSYS_WIN_PREFIX = $MSYS_WIN_PREFIX
SDL_CFLAGS      = $SDL_CFLAGS
SDL_LIBS        = $SDL_LIBS
XML_CFLAGS      = $XML_CFLAGS
XML_LIBS        = $XML_LIBS
EOF

echo "[OK] build/config.mk gerado"

# --------------------------------------------------
# 7) Build (real)
# --------------------------------------------------
echo
echo "### BUILD"
run make clean
run make

echo
echo "### BOOTSTRAP FINALIZADO COM SUCESSO"
