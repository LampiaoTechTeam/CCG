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
- Estrutura modular (`src/` + `include/`)
- Makefile estruturado
- Portabilidade Win32 e Linux

---

## 📂 Estrutura do projeto
```
├── include/          # Cabeçalhos (.h)
├── src/              # Implementações (.c)
├── scripts/          # scripts de compilação para prod/debug/fake (.sh)
├── Makefile          # Build simplificado
├── .gitignore
└── README.md
```

---

## 🛠️ Como compilar
Requisitos: GCC ou Clang + Make

```bash
# Clonar o projeto
git clone https://github.com/SEU_USUARIO/CCG.git
cd CCG

# Compilar
make

# Executar
./ccg
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
3. Commit suas mudanças (use mensagens claras!)
4. Abra um **Pull Request** 🎉

Consulte o arquivo [CONTRIBUTING.md](CONTRIBUTING.md) para mais detalhes.

---

## 🗺️ Roadmap inicial
- [ ] Melhorar loop de batalha
- [ ] Adicionar testes unitários
- [ ] Suporte a diferentes modos de jogo
- [ ] Localização (PT/EN)

---
