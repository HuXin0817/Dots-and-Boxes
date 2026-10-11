# Dots and Boxes

A browser implementation of Dots and Boxes with a strong, training-free AI written entirely in JavaScript. The default game uses a 6 x 6 box board and runs the search engine in a Web Worker so the interface remains responsive.

The project combines rule-guided PUCT, exact alpha-beta search, symmetry reduction, transposition caching, and Dots and Boxes chain theory. It does not use a neural network, offline training, or an opening book.

## Run

Requires Node.js 18, 20, or 22+ and npm.

```bash
npm install
npm run dev
```

Open the local URL printed by Vite. Both players are robots by default; use query parameters to choose human players:

```text
?player1=human&player2=robot
?player1=human&player2=human
?player1=robot&player2=robot
```

Click an unclaimed edge to play. Completing a box scores one point and keeps the turn. Use `Ctrl/Cmd` + `+`, `-`, or `0` to resize or reset the board scale.

For a production build:

```bash
npm run build
npm run preview
```

## AI Overview

The robot selects a move through a hybrid search pipeline:

1. **Exact root search** is attempted when at most 30 edges remain, or when the position consists only of recognized chains and loops.
2. **PUCT search** handles positions outside the exact-search horizon.
3. **Exact leaf search** is attempted inside PUCT when at most 16 edges remain.
4. **Proof propagation** carries exact leaf results through the PUCT graph. Statistical rollout values never count as proofs.

All values represent the future score margin from the player-to-move perspective. A capturing move keeps that perspective; a non-capturing move passes the turn and negates the child value.

### Rule-Guided PUCT

PUCT selects the action maximizing

```text
Q(s, a) + c * P(s, a) * sqrt(N(s)) / (1 + N(s, a))
```

where `Q` is the mean backed-up value, `P` is the rule-based prior, and `c` is the exploration constant. Every root action representative is visited once before normal PUCT selection, and the final move is chosen primarily by visit count.

The evaluator supplies both parts normally produced by a policy-value network:

- **Policy:** favors hard-hearted handouts, immediate captures, and safe edges; penalizes sacrifices by the forced boxes they expose. A 10% uniform mixture keeps every legal move reachable.
- **Value:** performs a rule-based rollout that takes useful handouts, captures forced boxes, prefers safe moves, and otherwise minimizes the opponent's forced capture sequence.

The search graph is retained across moves and shares nodes reached through transpositions. Actions equivalent under symmetries that preserve the current position are represented by a single PUCT edge.

### Exact Solver

The endgame solver uses alpha-beta negamax adapted to the extra-turn rule. Its main reductions are:

- D4 canonicalization across the board's eight rotations and reflections.
- A fixed-capacity transposition table storing independent lower and upper bounds.
- Capture-first, safe-move-second, sacrifice-last move ordering.
- Equivalent corner-edge removal.
- Half-open chain macro moves that compare taking the entire chain with the two-box hard-hearted handout.

The solver returns an exact future score margin when it completes within its node budget. Otherwise, control returns to PUCT.

### Chain and Loop Solver

When every unfinished box has at most two free sides, the position can often be decomposed into chains, loops, open chains, and open loops. A memoized component recurrence evaluates the strategic choices directly:

- take the whole opened component;
- hand out two boxes in a chain to retain control;
- hand out four boxes in a loop to retain control.

The recurrence also handles the special split of a closed two-chain. These values are exact and can terminate both rollouts and tree search.

### Proof Propagation

Exact endgame and chain values are stored separately from rollout estimates. A PUCT edge becomes solved only from an exact child, and a node becomes solved only after every representative action is solved. This lets the statistical search gradually turn into a minimax proof without allowing noisy evaluations to claim certainty.

## Default Search Configuration

| Parameter                  |              Value |
| -------------------------- | -----------------: |
| PUCT iterations per move   |              1,024 |
| Exploration constant       |                  2 |
| Exact root threshold       | 30 remaining edges |
| Exact root node limit      |          2,000,000 |
| Exact leaf threshold       | 16 remaining edges |
| Exact leaf node limit      |             20,000 |
| Exact-cache capacity       |    500,000 entries |
| PUCT graph reset threshold |      250,000 nodes |
| Policy sharpness           |                1.5 |

## Project Structure

```text
src/
|-- App.jsx                  Game interface and worker coordination
|-- config.js                Board and player configuration
`-- game/
    |-- board.js             Live game state
    |-- board-topology.js    Geometry, adjacency, and D4 mappings
    |-- robots.js            Hybrid search controller
    |-- puct-search.js       PUCT graph and proof propagation
    |-- rule-evaluator.js    Policy heuristics and rollouts
    |-- exact-solver.js      Exact alpha-beta endgame search
    |-- search-board.js      Reversible search state and chain analysis
    `-- search-cache.js      Bounded transposition table
```

The board size is set by `BoardSize` in `src/config.js`. The compact typed-array representation supports sizes from 1 x 1 through 6 x 6.

## License

[MIT](LICENSE)
