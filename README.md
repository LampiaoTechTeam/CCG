# 🃏 CCG – Console Card Game

Um jogo de cartas colecionáveis **em C (ANSI/C89/C99)**, rodando direto no **terminal**.  
O projeto é pensado para ser **leve, didático e open source**, servindo tanto como jogo quanto como exemplo de arquitetura em C.

---

## ✨ Features principais

- Sistema de **jogadores** e **monstros**
- **Decks** configuráveis
- Mercado / loja para compra de cartas
- Módulo de **batalha** entre jogadores
- Interface via **terminal**
- Estrutura modular (`src/` + `include/` + ... )
- Makefile estruturado
- Portabilidade Win32, Linux e Android
- Rotina de debug simples e robusta (modulo trace -> log/card_game.log)
- Rotina de historico de dialogo    (modulo trace -> log/card_game_dialog.log)

---

## 📂 Estrutura do projeto

```
├── assets/           # imagens (.png)
├── conf/             # xmls de configuracao (.xml)
├── include/          # Cabeçalhos (.h)
├── src/              # Implementações (.c)
├── fonts/            # Fontes/texto (.ttf)
├── scripts/          # scripts de compilação para prod/debug/fake (.sh)
├── doc/              # documentacoes (.md)
├── Makefile          # Build simplificado
├── .gitignore
└── README.md
```
## 📂 Diretorios temporários

```

├── build/            # arquivos de inclusao de makefile (.mk)
├── obj/              # Arquivos objetos da compilacao (.o)
├── log/              # Registros da execucao (.log)
└── bin/              # Local onde o executavel sera gerado

```

---

## 🛠️ Como compilar

Requisitos: GCC ou Clang + Make

```bash
# Clonar o projeto
git clone https://github.com/SEU_USUARIO/CCG.git
cd CCG

# Compilar
Windows:
./scripts/mk*_win32.sh
   % Com SDL2
   ./scripts/mk*S_win32.sh
Linux:
./scripts/mk*_linux.sh
   % Com SDL2
   ./scripts/mk*S_linux.sh
Onde:
 * => all - Executa o Clean + criacao de diretorios ( usualmente o primeiro make do dia deve ser este )
 * => d - Debug flags de gdb
 * => (apenas mk_win32 ou mk_linux) - monta apenas os arquivos alterados desde a ultima compilacao
# Executar
./bin/card_game
   % Com SDL2
   ./bin/card_game --sdl
```

Limpar artefatos:

```bash
make clean
```

---

## 🤝 Como contribuir

1. Faça um **fork** do repositório
2. Crie um branch:

   ```bash
   git checkout -b feature/minha-feature
   ```

3. Commit suas mudanças seguindo o padrão [Conventional Commits](https://www.conventionalcommits.org/):

   ```bash
   git commit -m "feat: adicionar nova funcionalidade"
   git commit -m "fix: corrigir bug no sistema de batalha"
   ```

4. Abra um **Pull Request** 🎉

**Configure os Git hooks** para validação automática:

```bash
./scripts/setup-git-hooks.sh
```

Consulte o arquivo [CONTRIBUTING.md](CONTRIBUTING.md) para mais detalhes sobre padrões de commit e código.

---

## 🗺️ Roadmap inicial

- [x] Melhorar loop de batalha
- [ ] Adicionar testes unitários
- [ ] Suporte a diferentes modos de jogo
- [ ] Localização (PT/EN)
- [x] Suporte a SDL2 - **NOVO** - Em desenvolvimento
---
