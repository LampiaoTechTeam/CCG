# 🎮 Game Concept

## 🧪 Debuff System
- **POISON**  
  - Cada X turnos (X ≥ 1) reduz o dano por turno
- **TOXIC** (?)  
  - Cada turno adiciona modificador (a definir)
- **FROZEN** (?)  
  - Perde turno / Jogada de dado (a definir)
- **STAMINA_DEBUFF** (????)  
  - Redução de velocidade / penalidades de turno / penalidade de vida
- **BURN** (?)  
  - Penaliza status de ataque, divide valores de ataque por modificador
- **PARALYZE** (?)  
  - Aumenta custo de energia da carta / Jogada de dado (a definir)

---

## 🃏 Card System
- **Tipos(classes)/subtipos**
  - Ofensivas
  - Defensivas
  - Suporte
  - (De)buffs

- **ATTACK**  
- **BLOCK**  
  - Funciona como vida extra
- **DEFENSE** (?)  
  - Reduz ataque físico do inimigo por valor definido
- **MAGIC**  
  - Aplica valor da carta em inimigos selecionados ou arbitrários (todos, alguns)
  - **Channeling Cards (?)**  
    - Aguardam antes de aplicar efeito | Jogam em ordem específica (primeiro, último, após, condição)
      - (?) Comprar cartas enquanto espera
      - (?) Descartar enquanto espera
      - **COND** – [Após alguma carta, (NÃO) tendo qualquer, tendo apenas...]
  - **Board Effect Cards**  
    - *Earthquake* – Afeta o grupo inimigo (?) dano único/múltiplos turnos
    - **Debuffs (?)**
- **ARTIFACTS (?)**

---

## 📚 Deck System
- **Deck Preset**  
  - *Initial (default)*
  - *Player Conf* – Setup pessoal + novas cartas compradas na loja
  - Quantidade máxima de cartas por tipo/subtipo  
    - Ex.: `Debuff + Poison >= 4 ? NOT_ALLOWED : OK`
- **DrawPile**
- **DiscardPile**
- **Hand**

### Funções
- Manter X cartas na mão (?) – efeitos colaterais para balanceamento
- Comprar cartas durante o turno do jogador
- **Shuffle DrawPile (?)**  
  - Quando não houver cartas suficientes para a mão → embaralhar descarte e drawpile restantes
- **Swap**  
  - Substituição e ordenação de cartas na mão
- **Sort**  
  - Ordenar por ordem alfabética após descarte
- **Discard**  
  - Ao usar | Em condição

---

## 🛒 Shop System
- Baseado no wallet do jogador (ouro)
- **Upgrade de cartas**
  - *Atk* – Aumenta valor de ataque da carta
  - *Blk* – Aumenta valor de defesa do jogador
- Comprar novas cartas (não pertencentes ao deck)
- Reroll de pool de cartas

---

## 🏰 Dungeon System
- **Progressão de nível da dungeon**
  - HP de monstros – aumentado (multiplicado por taxa)
  - ATK de monstros – aumentado (multiplicado por taxa)
  - Debuff de monstros (?)
  - Recompensa em ouro – aumentada por valor fixo + modificador de nível

---

## ⚔️ Battle System
- **Turno**
  - *Player* – Joga carta por tipo | Respeita limite de energia
  - *Monster* – Executa uma de suas ações possíveis de forma aleatória
  - TODO: Seleção de monstro pelo jogador
  - Sistema de skip de turno – pula turno ou X turnos sequenciais do mesmo lado
