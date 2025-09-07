# 🤝 Guia de Contribuição

Obrigado por querer contribuir com o **CCG – Console Card Game**!  
Aqui estão algumas orientações rápidas para mantermos o projeto organizado.

---

## 🚀 Fluxo de contribuição
1. Crie um fork do repositório
2. Crie um branch descritivo:
   ```bash
   git checkout -b feature/minha-feature
   ```
3. Implemente sua alteração
4. Confirme que o projeto compila com `make`
5. Abra um Pull Request relacionando a issue (se houver)

---

## � Padrão de Commits (Conventional Commits)
Utilizamos o padrão [Conventional Commits](https://www.conventionalcommits.org/en/v1.0.0/) para mensagens de commit:

```
<tipo>[escopo opcional]: <descrição>

[corpo opcional]

[rodapé(s) opcional(is)]
```

### Tipos de commit:
- `feat`: Nova funcionalidade
- `fix`: Correção de bug
- `docs`: Mudanças na documentação
- `style`: Formatação, ponto e vírgula ausente, etc (sem mudança de código)
- `refactor`: Refatoração de código (sem nova feature ou correção)
- `perf`: Melhoria de performance
- `test`: Adição ou correção de testes
- `build`: Mudanças no sistema de build ou dependências
- `ci`: Mudanças nos arquivos de CI
- `chore`: Outras mudanças que não modificam src ou test

### Exemplos:
```bash
feat: adicionar sistema de batalha
fix: corrigir cálculo de dano das cartas
docs: atualizar README com instruções de instalação
feat!: mudar estrutura da API (BREAKING CHANGE)
```

**Importante**: Commits que quebram compatibilidade devem usar `!` após o tipo ou incluir `BREAKING CHANGE:` no rodapé.

---

## �📏 Padrões de código
- Linguagem: **C99**
- Indentação: **2 espaços**
- Variáveis declaradas no **início do escopo**
- Notação húngara:
  - `iVariavel` → int  
  - `dVariavel` → double  
  - `szTexto` → string estática  
  - `pszTexto` → ponteiro para string  
  - `stEstrutura` → struct  
  - `pstEstrutura` → ponteiro para struct
  - `a+variavel` -> array de tipo de variavel. Ex.: aszStringLst -> char *szString[]
  - `g+variavel` -> global
- Seguir boas práticas de modularização: headers em `include/`, código em `src/`

---

## ✅ Checklist para Pull Requests
- [ ] Código compila e roda (`make` sem erros)
- [ ] Alterações testadas manualmente
- [ ] Código segue padrão do projeto
- [ ] Mensagem de commit segue o padrão Conventional Commits
- [ ] Comentários/documentação atualizados

---

## 📬 Dúvidas?
Abra uma [Discussion](https://github.com/SEU_USUARIO/CCG/discussions) ou crie uma issue.
