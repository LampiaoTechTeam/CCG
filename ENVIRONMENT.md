# ⚙️ Environment Setup – CCG (Console Card Game)

Este guia mostra como preparar o ambiente para compilar e rodar o **CCG**  
em **Linux** e **Windows (MinGW/Cygwin/MSYS2)** usando `make`.

---

## 🐧 Linux (Debian/Ubuntu/Fedora/Arch)

### 1. Instalar dependências básicas

```bash
# Debian/Ubuntu
sudo apt update
sudo apt install build-essential gcc make

# Fedora
sudo dnf groupinstall "Development Tools"

# Arch
sudo pacman -S base-devel
```

### 2. Clonar e compilar

```bash
git clone https://github.com/repiazza/CCG.git
cd CCG
make LINUX=1
./bin/card_game
```

### 3. Opções extras

- Compilação com debug:

  ```bash
  make DEBUG=1
  ```

- Limpar artefatos:

  ```bash
  make clean
  ```

---

## 🪟 Windows (MSYS2/MinGW)

### 1. Instalar MSYS2

Baixe e instale [MSYS2](https://www.msys2.org/).

### 2. Instalar toolchain MinGW

Abra o terminal **MSYS2 MinGW64** e execute:

```bash
pacman -Syu
pacman -S --needed base-devel mingw-w64-x86_64-toolchain
```

### 3. Clonar e compilar

```bash
git clone https://github.com/repiazza/CCG.git
cd CCG
make
./bin/card_game.exe
```

### 4. Observações

- O Makefile já detecta **mingw32/cygwin** via `gcc -dumpmachine`
- Variáveis auxiliares no Makefile:
  - `_WIN32` → ativa flags `-D_WIN32`
  - `LINUX` → ativa flags `-DLINUX` e rpath
- `mkdir` e `rm` são resolvidos automaticamente no MSYS2.

---

## 📌 Dicas úteis

- Sempre execute `make clean` ou `make all` antes de builds grandes.
- Use `make -j$(nproc)` para compilar em paralelo (Linux).
- Para Windows, compile **no terminal MinGW64**, não no MSYS2 padrão.
- Caso queira instalar em pasta fixa, ajuste a variável `PREFIX` no `Makefile`.
- O diretorio scripts contem rotinas automaticas para compilacao all ou so de fontes recém alterados.

---

## ✅ Resumo

- **Linux:** `apt install build-essential` → `make`
- **Windows:** instalar **MSYS2 + MinGW** → `make`
- Binários ficam em `bin/card_game` (Linux) ou `bin/card_game.exe` (Windows).
