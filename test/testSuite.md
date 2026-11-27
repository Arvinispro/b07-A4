# Test Suite

## Expected outputs (.out files)

Each `.in` file has a `.out` file next to it, except the nine skipped files below. The `.out` is stdout and stderr merged in program order (as if run with `2>&1`), so it includes the prompts and the messages `main.cc` sends to `cerr`.

How they were produced: the current source, run on every `.in`, with one scratch fix for a bug that stops the intended behaviour (the files in your folder were not changed):
- **`resetRuleState`** is never called when a game starts, so castling rights and the en passant square carry over from the previous game. The scratch copy calls `validate->resetRuleState(false)` in `System::gameStart`.
- The earlier promotion-letter bug (`Validate::validate`) and the mirrored row lookup in `main.cc` are already fixed in the source, so they need no scratch fix.

Every move the program accepted or rejected was also checked against python-chess (1,788 attempts, no differences). Running every file twice gave identical output.

What the outputs contain (from the current print statements):
- The prompt `game, move, setup, resign: ` is printed (with a newline) before every top-level command, and once more at the end of input.
- `from:` and `to:` (stderr) and `promote to(QRNB): ` appear only after an invalid or incomplete move; a normal `move e2 e4` reads both points directly. The `White(...)` and `Black(...)` prompts are no longer printed.
- The board is printed once per `move` command (also for a rejected move), once at game start, and after every `+` and `-` in setup.
- When a game ends by checkmate or stalemate the output is `White won!`, `Black won!` or `Draw`, then `Summary` and `White:xBlack:y`, then the standard board. The board comes last because `System::notify` resets it before `Board::notify` draws.
- `resign` prints nothing. `quit` and end of input print no final score.
- A draw adds no points (the scores are ints), so the 0.5-per-draw figures in Test 3 are the intended scores, not what is printed.
- `done` is also rejected with `Stalemate` when either colour has no legal move and is not in check.

Computer moves are skipped. A bot picks its move at random (level 1) or at random among its preferred moves (levels 2 and 3), so the board printed after a bot move cannot be fixed in advance. Any file that makes a bot move has no `.out`:

| File | Why it is skipped |
|---|---|
| test0_1 | every bot pairing and level, with bots moving |
| test0_9 | games from a setup with bots moving |
| test0_10 | session j starts a bot game after a repaired setup |
| test0_11 | the control games at the end start bots and move them |
| test2_7 | a bare `move` for a bot's turn |
| test2_16 | forced bot replies, but several blocks leave the bot a choice (promotion piece, two or three moves) |
| test2_17 | bot level 2 and 3 behaviour, with preferred sets of one or two moves |
| test3_8 | eight bot-vs-bot games run to the end |
| test3_9 | 61 games that include bot games |

Files that only create a bot and never move it (test0_12_7, test0_12_8, test0_12_9) are deterministic and do have a `.out`. The Test 1 outputs are `test1_1.out` to `test1_6.out`.

## Test 0: Game and setup commands

Checks starting games (`game`) and the setup commands (`setup`, `+`, `-`, `=`, `done`).

Rules shared by every test 0 file:
- Every setup is self-contained: pieces it relies on are placed with `+`, and unused standard squares are cleared with `-`. The files do not depend on whether `setup` starts from the standard or an empty board.
- A setup always ends with a valid `done`. A rejected `done` leaves the program in setup mode, so the file repairs the board and issues `done` again.
- After a valid `done`, `game human human` starts on the setup board. Each game ends with `resign //end game`.
- Bot games use a bare `move` for the bot's turn. Bot moves are random, so each game has at most 3 bot moves and the human moves stay legal against any bot reply.
- Level is written with brackets: `computer [1]`, `[2]` or `[3]`.

| Test | What it checks | File | Size | Expected output |
|---|---|---|---|---|
| 0.1 | Player/bot vs player/bot: every pairing and level | test0_1.in | 17 games | skipped: bot moves are random |
| 0.2 | Legal setups | test0_2.in | 10 setups | `test0_2.out` |
| 0.3 | Illegal setups: every rule and combinations, repaired one rule at a time | test0_3.in | 19 ladders, 76 `done` lines | `test0_3.out` |
| 0.4 | Correct `+` | test0_4.in | 4 sessions | `test0_4.out` |
| 0.5 | Incorrect `+` (must change nothing) | test0_5.in | 4 sessions | `test0_5.out` |
| 0.6 | Correct `-` | test0_6.in | 6 sessions | `test0_6.out` |
| 0.7 | Incorrect `-` (must change nothing) | test0_7.in | 4 sessions | `test0_7.out` |
| 0.8 | `=` sets the side to move first | test0_8.in | about 25 sessions | `test0_8.out` |
| 0.9 | Start a game after a legal setup | test0_9.in | 10 games | skipped: bot moves are random |
| 0.10 | Try to start a game after an illegal setup | test0_10.in | 10 sessions | skipped: bot moves are random |
| 0.11 | `game` with a missing or wrong bot level | test0_11.in | 34 `game` lines | skipped: bot moves are random |
| 0.12 | End of input (EOF) at every prompt | test0_12_1.in to test0_12_26.in | 26 tiny files | `test0_12_1.out` to `test0_12_26.out` |
| 0.13 | Odd whitespace and split tokens | test0_13.in | 88 lines | `test0_13.out` |
| 0.14 | Unknown commands, extra tokens, `quit`, commands at the wrong time | test0_14.in | 87 lines | `test0_14.out` |
| 0.15 | Board display | test0_15.in | 153 lines | `test0_15.out` |

### Test 0.1: Player / bot vs player / bot

| Pairing | Games |
|---|---|
| Human vs human | 2 |
| Human vs computer (levels 1, 2, 3) | 4 |
| Computer (levels 1, 2, 3) vs human | 3 |
| Computer vs computer (1/1, 2/2, 3/3, 2/3, 1/3, 3/1, 1/2, 3/2) | 8 |

A new game with different player types starts right after each `resign`.

### Test 0.2: Legal setup

| Setup | Position |
|---|---|
| 1 | Bare K vs k |
| 2 | K+Q vs k |
| 3 | K+R vs k+r |
| 4 | Pawns on rows 2 to 7 for both colours, with two promotions |
| 5 | Three white queens vs a lone king |
| 6 | Kings in corners plus a pawn |
| 7 | Standard position rebuilt with 32 `+` commands |
| 8 | Standard position plus an extra queen, with a `-` on an empty square |
| 9 | Empty `setup` then `done` (relies on the default board being valid) |
| 10 | Second setup after a game, with `+` replacing a piece |

### Test 0.3: Illegal setup

Each `done` is rejected until the last one. The rules checked: exactly one white king, exactly one black king, no pawn on row 1 or 8, neither king in check.

| Ladder | Violation | Repair |
|---|---|---|
| 1 to 3 | No kings, one king missing, two or three kings per colour | one king at a time |
| 4, 5 | Pawns on row 1 or 8 (each colour, alone and all four at once) | one pawn at a time |
| 6, 9, 10, 11 | Check by queen, rook, bishop, knight, pawn; both kings in check | remove the checker |
| 7, 8 | A pawn on row 1 or 8 that also gives check | one `-` fixes both |
| 12 | Adjacent kings | move a king |
| 13 to 18 | A missing or extra king together with a row-1/8 pawn or a check | one rule at a time |
| 19 | All four rules violated at once | one rule at a time |

### Test 0.4: Correct `+`

| Session | Case |
|---|---|
| 1 | All 12 piece letters on empty squares |
| 2 | Replace a different piece, same piece again (no change), replace an enemy piece, corners |
| 3 | Three queens of one colour, pawns on rows 2 to 7 |
| 4 | King relocated by `+` then `-`, edge squares |

Each session plays moves that prove the placed pieces behave as placed.

### Test 0.5: Incorrect `+` (no change)

| Session | Bad input |
|---|---|
| A | Bad piece letters (`X`, `a`, `Z`, `5`, `#`) on empty and occupied squares |
| B | Off-board squares (`i9`, `a0`, `h9`, `z1`, `a10`) and malformed (`6e`, `19`, `E2`, `e#`) |
| C | Both invalid, and swapped arguments (`+ e4 K`) |
| D | A bad `+` as the first command, with pawn and knight markers |

The later game only works if the board is unchanged (for example, `e2 e4` needs `e4` still empty).

### Test 0.6: Correct `-`

| Session | Case |
|---|---|
| 1 | One piece of each of the 12 kinds placed then removed |
| 2 | King removed and re-placed |
| 3 | Blocking pieces removed so a rook, bishop or queen can slide |
| 4 | Board wiped down to the kings |
| 5 | Remove and replace with a different piece, double removal |
| 6 | Removing a checking piece makes a rejected `done` valid |

### Test 0.7: Incorrect `-` (no change)

| Session | Bad input |
|---|---|
| 1 | Empty squares, off-board squares (`i9`, `a0`, `h9`, `z1`, `a10`) |
| 2 | Malformed (`6e`, `19`, `E2`, `e#`, `ee`, `22`) |
| 3 | Plus-style arguments (`- K e1`) |
| 4 | Squares just emptied, removed again |

### Test 0.8: `=` sets the turn

| Cases |
|---|
| `= black` then `done`: black moves first, a white move first is rejected |
| `= white`, no `=` (default white first) |
| Last `=` wins (`= black` then `= white`, and the reverse) |
| Same colour twice, `=` at the start, middle or end of setup |
| `= Black` and `= White` (capitalised) |
| Invalid colours (`= green`, `= w`, `= 1`) change nothing |
| A normal game after a setup game starts with white to move |
| `= black` where black has exactly one legal move |

### Test 0.9: Game after a legal setup

| Session | Case |
|---|---|
| 1 | Custom board used (a piece that exists only there moves) |
| 2 | `= black`, black moves first |
| 3a, 3b | Human vs bot, bot vs human on a custom board |
| 4 | Bot vs bot on a custom board |
| 5 | After `resign`, the next game is on the standard board |
| 6 | Two setups before one game: the second one is used |
| 7 | Promotion on a custom board |
| 8 | A restricted king: rejected moves into attacked squares |

### Test 0.10: Game after an illegal setup

Each session builds an invalid board, `done` is rejected, then `game` and `move` are tried (both print `command not found` in setup mode), the board is repaired, and a real game starts.

| Session | Violation |
|---|---|
| a, b | No white king, no black king |
| c, d | Two white kings, two black kings |
| e, f | Pawns on row 1 or 8 (white, black) |
| g, h | White king in check, black king in check |
| i | Several violations repaired one at a time |
| j | Bot opponent, starting `game human computer [1]` |

### Test 0.11: Missing or wrong bot level

Each failed `game` line is followed by a valid `game` line that starts, to show no game was running.

| Kind | Cases |
|---|---|
| No level | `game computer` (then a throwaway `x` line), `game computer human`, `game human computer` (with `x`), `computer [1] computer` (with `x`) |
| Out of range | `[0]`, `[4]`, `[10]`, `[-1]` |
| Missing brackets | `2`, `[1`, `3]` |
| Wrong content | `[a]`, `[]`, `high`, `one` |
| Level given to a human | `game human [1] human` |
| Both bots bad | `computer [x] computer [1]` |
| Controls | `[1]`, `[2]`, `[3]` for white and for black |

### Test 0.12: End of input (EOF)

A file can only end at one point, so this test is a set of 26 tiny files, each ending at a different prompt. In every case the program must terminate cleanly: no hang, no crash, no endless re-prompting. Files marked "no newline" end right after the last token, with no trailing newline, which sets EOF while that token is being read. In files 12 to 26 the comment line at the top is the only comment.

| File | Where the input ends |
|---|---|
| 1 | Empty file |
| 2 | A lone newline |
| 3 | After `game` (white player prompt) |
| 4 | After `game human` (black player prompt) |
| 5 | After `game computer` (white level) |
| 6 | After `game human computer` (black level) |
| 7 | After `game computer [1]` (black player prompt) |
| 8 | Same as 7, no newline |
| 9 | `game human computer [1]`, no newline |
| 10 | After a complete `game human human`, at the main prompt |
| 11 | `game human human`, no newline |
| 12 | After `move` (`from:` prompt) |
| 13 | After `move e2` (`to:` prompt) |
| 14 | A complete `move e2 e4` as the last line, no newline |
| 15 | After white has moved, at black's `from:` prompt |
| 16 | After an invalid point (`move e2 e9`), at the retry prompt |
| 17 | `resign` as the last line, no newline |
| 18 | At the promotion prompt (`move a7 a8` from a setup board, no letter) |
| 19 | After `setup` |
| 20 | After `+` inside setup |
| 21 | After `+ K` inside setup (square missing) |
| 22 | After `-` inside setup |
| 23 | After `=` inside setup |
| 24 | After `= white`, still inside setup |
| 25 | A rejected `done` (white king removed), then EOF still inside setup |
| 26 | Inside setup after an edit (`- e2`) |

Files 19 to 26 end inside setup mode. The setup loop in `main.cc` has no EOF check, so these are the ones most likely to hang.

### Test 0.13: Odd whitespace and split tokens

The program reads input as whitespace-separated tokens, so these must behave like the tidy forms. Line ranges refer to the file as written (no comments).

| Lines | What it tests |
|---|---|
| 1 to 8 | Many spaces between tokens, tab-separated lines, leading and trailing whitespace, whitespace-only lines, a capture |
| 9 to 33 | `game`, `human`, `human` and `move`, `e2`, `e4` each on three lines, runs of 3 and 10 blank lines |
| 34 to 45 | Split points (`move e 2 e4`, `move    d2          d` then three blank lines then `4`, a point split by tabs), several commands on one line (`move d8 d5 move b1 c3`) |
| 46 to 52 | A whole game with CRLF line endings |
| 53 to 58 | Invalid points with odd spacing (`move e   9`, `move 6    e`), each followed by its retry line |
| 59 to 85 | A setup with odd spacing (`+    K       e1`, `+ k` then two newlines then `e8`, `=` and `white` on separate lines), then a promotion letter on a later line (`move a7 a8`, blank lines, `Q`) |
| 86 to 88 | Blank lines, then `quit` |

### Test 0.14: Unknown commands, extra tokens, `quit`, wrong-time commands

| Lines | What it tests |
|---|---|
| 1 to 9 | No game: unknown words (`foo`, `help`, `exit`, `123`, `#`) and `+`, `-`, `=`, `done` at top level |
| 10 to 18 | Wrong-case commands: `GAME`, `Game`, `MOVE`, `Move`, `RESIGN`, `Resign`, `SETUP`, `QUIT`, `Quit` |
| 19 to 20 | `move e2 e4` and `resign` with no game running |
| 21 to 34 | A game with extra tokens (`move e2 e4 extra`, `move e7 e5 e5`), `game human human` while a game runs, `setup` mid-game then `done`, a wrong-case `MOVE`, a wrong-case `Resign` |
| 35 to 37 | `game human human extra tokens` |
| 38 to 73 | A setup that is given `game`, `move`, `resign`, `quit`, `foo`, `DONE`, `Done` and `+K e1` before a valid `done` (`quit` does not exit inside setup) |
| 74 to 79 | A game from that setup |
| 80 to 83 | A new game, two moves, then `quit` while it is running |
| 84 to 87 | Lines after `quit` that must never be read |

Unspecified: lines 19 to 20 (whether the arguments of `move` and `resign` are consumed), 24 to 28 (`game` and `setup` while a game is running). The black move on line 28 may be rejected if `setup` resets the turn.

### Test 0.15: Board display

| Lines | What is displayed |
|---|---|
| 1 to 11 | Standard board at game start, after normal moves, after a rejected move, after two captures, and again after `resign` and a new game |
| 12 to 45 | 32 `-` commands taking the board from standard to empty (one redraw each), plus a `-` on an empty square |
| 46 to 66 | All 12 piece letters on light and dark squares, corners and edges; `+ N e4` replacing `+ P e4`; a game from that sparse board |
| 67 to 105 | A kings-only board that shows the checkerboard pattern, with the kings swapped to the other square colours, then a game |
| 106 to 153 | White promotion to a queen, a black promotion to a knight, a capture of the promoted piece, one rejected king move |

## Test 1: Internal validation

Checks each piece's own movement rules: legal moves are accepted, and moves with the wrong shape, a blocked path, or a landing on an own piece are rejected.

Rules shared by every test 1 file:
- Every game is `game human human`.
- After an illegal attempt, the same colour plays a legal move, so turns stay in order.
- No illegal attempt depends on check, pins, castling or en passant. Those belong to external validation.
- No king is ever in check and no game ends early.
- A promotion letter (`Q`/`R`/`N`/`B`) is given only when a pawn reaches the last row.


### Test 1.1: Pawn

| Case | Expected | Example (white) | Example (black) |
|---|---|---|---|
| Forward 1 | Legal | `e2 e3` | `d4 d3` |
| Forward 2 from start | Legal | `d2 d4` | `e7 e5` |
| Diagonal capture | Legal | `a6 b7` | `e5 d4` |
| Capture + promotion | Legal | `b7 a8 R`, `c7 d8 B` | `c2 b1 Q`, `e2 f1 N` |
| Forward 2 not from start | Illegal | `e3 e5` | `d4 d2` |
| Forward 2, piece in between | Illegal | `h2 h4` | `c7 c5` |
| Forward 1 into occupied square | Illegal | `h2 h3` | `c7 c6` |
| Diagonal to empty square | Illegal | `e3 f4` | `d4 c3` |
| Sideways | Illegal | `e3 d3` | `d4 c4` |
| Backward | Illegal | `e3 e2` | `d4 d5` |
| More than 2 forward | Illegal | `h2 h5` | `a7 a4` |
| Capture straight forward | Illegal | `a6 a7` | `h4 h3` |
| Irregular shape | Illegal | `g3 d4`, `f2 g5` | `a7 d6`, `f7 g4` |
| Knight-like | Illegal | `g3 h5` | `a7 b5` |
| Promoted queen moves like a queen (backward, sideways, diagonal) | Legal | `g8 g5`, `g5 a5` | `a1 f6`, `f6 f5` |
| Promoted queen breaks queen rules (knight-shape, blocked, own piece) | Illegal | `g5 e4`, `g5 c1` | `a1 c2`, `a1 h8` |
| Promoted rook moves like a rook | Legal | `a8 b8` | — |
| Promoted rook breaks rook rules (diagonal, blocked, own piece) | Illegal | `a8 b7`, `a8 a4` | — |
| Promoted knight moves like a knight | Legal | — | `f1 g3`, `g3 e4` |
| Promoted knight breaks knight rules (straight, diagonal, own piece) | Illegal | — | `f1 d1`, `f1 h3` |
| Promoted bishop moves like a bishop | Legal | `d8 c7`, `c7 e5` | — |
| Promoted bishop breaks bishop rules (straight, own piece, blocked) | Illegal | `d8 d7`, `c7 h2` | — |

### Test 1.2: Knight

| Case | Expected | Example (white) | Example (black) |
|---|---|---|---|
| L-shape (all 8 directions, for each colour) | Legal | `g1 f3`, `d4 b3` | `g8 h6`, `a2 c3` |
| Jump over pieces from start | Legal | `b1 c3` | `b8 c6` |
| Capture | Legal | `b7 d8` | `c6 e5` |
| Forward 1 | Illegal | `f3 f4` | `b4 b3` |
| Diagonal 1 | Illegal | `f3 g4` | `h6 g5` |
| Straight 2 | Illegal | `b1 b3` | `g8 g6` |
| Straight 3 | Illegal | `d8 d5` | `c6 c3` |
| 2x2 diagonal | Illegal | — | `c6 e4` |
| Near-L (3+1) | Illegal | `c3 f4` | `e5 h4` |
| Lands on own piece | Illegal | `b1 d2` | `b8 d7` |
| Irregular shape (3+2, 2+3, 1+3) | Illegal | `b3 e5`, `b3 d6`, `b3 a6` | `c3 f5`, `d8 b5`, `c3 b6` |

### Test 1.3: Bishop

| Case | Expected | Example (white) | Example (black) |
|---|---|---|---|
| Diagonal, short and long (all 4 directions, for each colour) | Legal | `f1 c4`, `c1 g5` | `f8 c5`, `c8 g4` |
| Capture | Legal | `g5 d8` | `g4 d1` |
| Jump over a piece | Illegal | `c1 e3` | `c8 e6` |
| Lands on own piece | Illegal | `f1 g2` | `c8 d7` |
| Straight along a rank | Illegal | `c4 a4` | `g4 e4` |
| Straight along a file | Illegal | `c4 c3` | `c5 c3` |
| Knight-like | Illegal | `c4 e3` | `c5 a6` |
| Irregular shape | Illegal | `d6 g7`, `d6 f3` | `b6 c3`, `d3 h5` |

### Test 1.4: Rook

| Case | Expected | Example (white) | Example (black) |
|---|---|---|---|
| Straight (all 4 directions, for each colour), short and long | Legal | `a1 a3`, `a3 h3` | `h8 h6`, `h6 a6`, `c1 c3` |
| Capture | Legal | `h3 h5` | `b6 b2` |
| Jump over own piece | Illegal | `a1 a5` | `h8 h4` |
| Jump over enemy piece | Illegal | `h3 h6` | `b6 b1` |
| Lands on own piece | Illegal | `h3 h2` | `a6 a7` |
| Diagonal | Illegal | `a3 c5` | `b2 c3` |
| Knight-like | Illegal | `h7 f8` | `h6 f5` |
| Irregular shape | Illegal | `g8 d7`, `g8 e5` | `c1 f2`, `c1 a4` |

### Test 1.5: Queen

| Case | Expected | Example (white) | Example (black) |
|---|---|---|---|
| Straight along file / rank | Legal | `d1 d3` | `h4 d4`, `a3 e3` |
| Diagonal, long | Legal | `d3 h7`, `f3 b7` | `d8 h4` |
| Capture | Legal | `d3 h7` | `h4 d4` |
| Jump over own piece | Illegal | `d1 d3` (pawn on d2) | `d8 d6` |
| Jump over enemy piece | Illegal | `h7 f7` | `h4 a4` |
| Lands on own piece | Illegal | `d1 e2` | `d4 e5` |
| Knight-like | Illegal | `d1 e3` | `d8 f7` |
| Irregular shape | Illegal | `d3 g4` | `h4 e6` |

### Test 1.6: King

| Case | Expected | Example (white) | Example (black) |
|---|---|---|---|
| One step (all 8 directions, for each colour) | Legal | `e1 e2`, `f3 g4`, `d3 e2` | `e8 e7`, `e7 d6` |
| Capture undefended piece | Legal | `f5 e5` | `d4 e4` |
| Lands on own piece | Illegal | `e1 e2` (pawn on e2) | `e8 d8` |
| 2 squares diagonal | Illegal | `e2 c4` | `e7 c5` |
| 2 squares straight (after king has moved) | Illegal | `f3 f5` | `c5 a5` |
| 3+ squares | Illegal | `e3 a3` | `c6 f6` |
| Knight-like | Illegal | `e2 g3` | `d6 e8` |
| Irregular shape | Illegal | `e2 h3`, `e2 a4` | `e7 b6`, `e7 c4` |

## Test 2: External validation

Checks the rules around the whole board: bad input, check, checkmate, stalemate, resign, bots and turn order.

Rules shared by every test 2 file:
- After a rejected move, the same colour moves again.
- When the program meets an invalid point it prints an error, throws away the rest of that line, and prompts again. So the line after every invalid-point line is a retry written as `<from> <to>` with no `move` prefix.
- No setup mode, no castling, no en passant.

| Test | What it checks | File | Size | Expected output |
|---|---|---|---|---|
| 2.1 | Out-of-bounds `from` and/or `to` | test2_1.in | 24 invalid lines | `test2_1.out` |
| 2.2 | Malformed points (`6e`, `19`) | test2_2.in | 26 invalid lines | `test2_2.out` |
| 2.3 | In check: the next move must escape | test2_3.in | 62 moves (34 legal, 28 illegal) | `test2_3.out` |
| 2.4 | Checkmate | test2_4.in | 11 blocks, each built with `setup` | `test2_4.out` |
| 2.5 | Draw by stalemate | test2_5.in | 4 games | `test2_5.out` |
| 2.6 | Resign | test2_6.in | 8 games | `test2_6.out` |
| 2.7 | `move` for a player vs a bot | test2_7.in | 7 games | skipped: bot moves are random |
| 2.8 | Controlling the other colour's pieces | test2_8.in | 42 moves (17 legal, 25 rejected) | `test2_8.out` |
| 2.9 | Castling | test2_9.in | 18 blocks, 656 lines | `test2_9.out` |
| 2.10 | En passant | test2_10.in | 22 blocks, 861 lines | `test2_10.out` |
| 2.11 | Promotion letters and promotion edge cases | test2_11.in | 929 lines | `test2_11.out` |
| 2.12 | Mate and stalemate reached by special moves | test2_12.in | 12 blocks, 525 lines | `test2_12.out` |
| 2.13 | Setups where a side has no (or only one) legal move | test2_13.in | 10 blocks, 437 lines | `test2_13.out` |
| 2.14 | Degenerate moves (same square, adjacent kings, pins) | test2_14.in | 284 lines | `test2_14.out` |
| 2.15 | Other draw rules (material, repetition, fifty-move) | test2_15.in | 434 lines | `test2_15.out` |
| 2.16 | Bots where the reply is forced | test2_16.in | 16 blocks, 665 lines | skipped: bot moves are random |
| 2.17 | Bot level 2 and 3 behaviour | test2_17.in | 16 blocks, 706 lines | skipped: bot moves are random |

### Test 2.1: Out of bounds

| Case | Examples |
|---|---|
| Only `to` off the board | `a1 a9`, `g8 i7`, `a8 a0`, `h2 h10`, `a8 a-1`, `h8 k8` |
| Only `from` off the board | `i1 h1`, `j9 a1` |
| Both off the board | `j1 j2`, `a9 a10`, `z0 z0`, `i9 j10` |
| Bad columns / rows | columns i, j, k, z; rows 0, 9, 10, `-1` |
| Edge and corner points | `a0`, `a9`, `h0`, `h9`, `i1`, `i8` |
| Retry that is also invalid | `g8 i7` then `i9 j9` then `b8 c6`; `i1 h1` then `h0 h2` then `b1 c3` |

Off-board moves come from rooks, knights, bishops, kings and queens, for both colours. There is no pawn on row 7 heading to row 9.

### Test 2.2: Malformed points

| Kind | Examples |
|---|---|
| Row before column | `6e`, `4e`, `2a`, `5h` |
| Digits only | `19`, `44`, `11`, `77` |
| Letters only | `ab`, `ee` |
| A single character | `e`, `4` |
| Symbols | `#3`, `e#`, `e-`, `**` |
| Uppercase column | `E2`, `C8`, `H6` |

Each kind appears as `from`, as `to` and as both, for both colours. Two retries are themselves invalid (`d1 E2`, `h7 6h`).

### Test 2.3: Escaping check

One continuous game of 62 moves.

| Case | Expected | Count |
|---|---|---|
| Ignore the check with an unrelated move | Illegal | 8 |
| Block, capture the checker, king step | Legal | about 11 |
| King onto an attacked square | Illegal | 7 |
| King captures a protected checker | Illegal | 1 |
| A move that doesn't block | Illegal | 6 |
| Capture a different piece instead of the checker | Illegal | 4 |
| Pinned piece moves off the pin line | Illegal | 3 pins; one legal move along the line |
| Double check: only a king move is legal | Legal (`Ke2`) / 5 illegal replies | 1 |
| King moves into check with no check present | Illegal | 2 (white only) |

Checks come from a bishop (3), pawn (1), knight (2), queen (2) and rook (1).

### Test 2.4: Checkmate

Each block is `setup` (a small position), `done`, `game human human`, then 1 to 3 moves ending in the mating move. No full games are played. The black king is never in check at `done`. Final score: White 7, Black 4.

| Block | Winner | Mating move | Feature | Score after (W:B) |
|---|---|---|---|---|
| 1 | White | Queen `c1 c8` | Queen + king vs king, corner mate | 1:0 |
| 2 | White | Rook `a1 a8` | Back-rank mate behind f7/g7/h7 pawns | 2:0 |
| 3 | Black | Queen `c8 c1` | Black moves first (`= black`) | 2:1 |
| 4 | Black | Rook `b8 b1` | Near-miss: `a8 a1+` is captured by the bishop on d4, then the second rook mates | 2:2 |
| 5 | White | Bishop `e3 d4` | Supported by a knight on h6, king boxed in by its own h7 pawn | 3:2 |
| 6 | Black | Knight `e4 f2` | Smothered mate: king boxed in by `Rg1`, g2, h2 | 3:3 |
| 7 | White | Pawn `g6 g7` | King not in check at the start and has no escape squares | 4:3 |
| 8 | White | `e7 e8 Q` | Promotion mate, with `e7 f8 Q` rejected just before it | 5:3 |
| 9 | White | Queen `h1 a8` | Near-miss: `Qh1+` is not mate, the king steps `h8 g8` | 6:3 |
| 10 | Black | Rook `a1 c1` | Near-miss: `Ra8-a1+` is blocked by a knight on c1, then the rook takes the blocker with mate | 6:4 |
| 11 | White | Knight `e4 d6` | Double-check mate, the knight discovers the rook on e1 | 7:4 |

### Test 2.5: Stalemate

| Game | Who is stalemated | Notes |
|---|---|---|
| 1 | Black | Sam Loyd's 10-move stalemate, 19 plies |
| 2 | White | A mirrored Loyd game with tempo moves so Black moves last, 22 plies |
| 3 | Black | Loyd's game with three rejected White moves right before the last move |
| 4 | Black | A near-miss: a check leaves Black exactly one legal move, and the game ends in stalemate on a later move |

### Test 2.6: Resign

| Game | Who resigns | Situation | Score after (W:B) |
|---|---|---|---|
| 1 | White | First move of the game | 0:1 |
| 2 | Black | Right after White's first move | 1:1 |
| 3 | White | After four moves, on a fresh board | 1:2 |
| 4 | Black | Mid-game | 2:2 |
| 5 | White | Right after a rejected move | 2:3 |
| 6 | Black | Right after a rejected move | 3:3 |
| 7 | Black | While in check | 4:3 |
| 8 | White | After four moves, on a fresh board | 4:4 |

The file ends with a `resign` while no game is running (expected behaviour unspecified).

### Test 2.7: `move` for a player vs a bot

| Game | Setup | What it checks |
|---|---|---|
| A | human vs computer [1] | human `move e2 e4`, bare `move` for the bot |
| B | human vs computer [1] | `move e7 e5` typed on the bot's turn: the bot should disregard the points |
| C | human vs computer [1] | a bare `move` on a human's turn, points on the next line |
| C2 | human vs human | the same bare `move` for both colours |
| D | computer [1] vs human | the bot moves first; points on a bot's turn are disregarded |
| E | computer [2] vs computer [1] | four bare `move` lines |
| F | human vs computer [3] | two exchanges |

Every game has at most 3 bot moves (4 in E) and ends with `resign //end game`.

### Test 2.8: Controlling the other colour

| Case | Examples |
|---|---|
| White tries to move a black piece, all six types | `e7 e5`, `d7 d5`, `g8 f6`, `a8 b8`, `f6 f3`, `e8 f8` |
| Black tries to move a white piece, all six types | `e2 e4`, `d2 d4`, `g1 f3`, `a1 b1`, `d1 d2`, `e1 d1` |
| Capture that would be legal for the other side | `c6 e4`, `c5 f2`, `f3 e5`, `c4 f7` |
| Move from an empty square | `e7 e6` (white's turn), `e2 e3` (black's turn) |
| First move of the game by a black piece | `e7 e5`, `d7 d5`, `g8 f6` |
| Black piece moved right after white moved | `e2 e3`, `d2 d4`, `g1 f3` |

Every rejected attempt is followed by a legal move from the side that was to play, so the turn does not change.

### Test 2.9: Castling

18 blocks: 4 reached by real moves, 14 built with `setup` (king and rook on their home squares). Every rejected castle is followed by a legal move by the same colour, and every block ends with `resign`.

| Lines | What it tests |
|---|---|
| 1 to 60 | Real games: both colours castle on both sides, the castled rook then moves; king already moved (rejected); king in check (rejected) |
| 61 to 184 | Queen-side castle with `b1` attacked (legal); a castle that gives check; king-side castle with the rook attacked (legal) |
| 185 to 274 | A two-square king move that isn't castling (rejected); rook moved away and back (king side rejected, queen side still legal) |
| 275 to 361 | A piece between king and rook (own bishop, enemy knight); king in check (rejected) |
| 362 to 484 | King passes through an attacked square; king lands on an attacked square |
| 485 to 565 | Rook captured, rook missing |
| 566 to 656 | King not on `e1` trying `d1 f1`/`d1 b1`; a king that has moved loses its rights; no second castle after castling |

### Test 2.10: En passant

22 blocks, mostly built with `setup`. A setup board has no history, so the double step is played after `done`.

| Lines | What it tests |
|---|---|
| 1 to 215 | Legal: white toward the a-side, the h-side and the a-file edge; black on the kingside and the h-file edge. A rook slide afterwards proves the captured pawn is gone. |
| 216 to 301 | Illegal: one move too late; after two single steps instead of a double step |
| 302 to 427 | Illegal: pawn not on its capturing rank; a pawn that never double-stepped (and a sideways capture); a non-adjacent double step |
| 428 to 563 | The capture would expose the king (rejected); it is the only escape from check (legal); it gives a discovered check |
| 564 to 651 | Two pawns could capture, taken with either |
| 652 to 679 | Three real games from the standard start, including losing the chance after an extra move pair |
| 680 to 765 | White en passant on the h-file edge; black en passant on the a-file edge |
| 766 to 806 | The black version of "wrong rank": a black pawn on its 3rd rank tries the diagonal after a white double step (rejected) |
| 807 to 861 | Black en passant as the only escape from check: eight alternatives rejected, only `e4 f3` legal |

### Test 2.11: Promotion letters and edge cases

Every block is built with `setup`. A letter always follows the move on the same line, or on a later line after blank lines.

| Lines | What it tests |
|---|---|
| 1 to 208 | All four letters for both colours, by plain promotion and by capture on both wings |
| 209 to 366 | Bad letters then a retry on the next line: `X`, `K`, `P`, `Z`, lowercase `q` and `n`, `5`, `#`, two or more in a row |
| 367 to 463 | The letter on a later line after blank or whitespace-only lines |
| 464 to 551 | Promotion that captures the checking rook to escape check, for each colour |
| 552 to 679 | Promotion that gives check (a-file, h-file, a knight check a queen wouldn't give) |
| 680 to 786 | Pawn moves that don't reach the last row never ask for a letter; promoted pieces then move as their new type |
| 787 to 838 | Rejected promotions: onto a friendly piece, blocked forward, diagonal to an empty square |
| 839 to 929 | A pinned pawn promoting is rejected, both colours |

### Test 2.12: Endings reached by special moves

12 blocks, each built with `setup`; each block's last move ends the game.

| Lines | Ending | Result |
|---|---|---|
| 1 to 44 | White castles queen side, the rook gives mate | White wins |
| 45 to 88 | Black castles king side, the rook gives mate | Black wins |
| 89 to 135 | En passant gives a discovered-check mate | White wins |
| 136 to 178 | Promotion to a rook, back-rank mate | White wins |
| 179 to 222 | Capture-promotion to a rook, mate | White wins |
| 223 to 262 | Promotion to a queen stalemates | Draw |
| 263 to 308 | Same position, under-promotion to a rook avoids stalemate; ends with `resign` | no result |
| 309 to 351 | Stalemate with a pinned knight and a blocked pawn | Draw |
| 352 to 392 | Stalemate by a capture | Draw |
| 393 to 435 | Check with one escape (`b4 d4`, reply `a1 a2`), then stalemate (`d4 c3`) | Draw |
| 436 to 480 | Mate where the mated side's en passant doesn't help | White wins |
| 481 to 525 | Smothered mate by a promoted knight | White wins |

### Test 2.13: Setups where a side has no (or only one) legal move

`done` is rejected with `Current setup is invalid` (kings, back-rank pawns or a king in check) or, if that passes, with `Stalemate` when EITHER colour has no legal move and is not in check, whoever is to move. After a rejected `done` the program stays in setup mode, so each such block repairs the board and issues `done` again.

| Lines | Block | What it tests | Rejected `done` |
|---|---|---|---|
| 1 to 44 | 1 | White stalemated (K h1 vs black K g3 + Q f2), then repaired and played | line 38: `Stalemate` |
| 45 to 88 | 2 | The mirror with black stalemated (`= black`) | line 82: `Stalemate` |
| 89 to 135 | 3 | White stalemated by a pinned knight and a blocked pawn | line 130: `Stalemate` |
| 136 to 179 | 4 | The non-moving side (black) stalemated while white has many moves | line 173: `Stalemate` |
| 180 to 228 | 5 | A king in check and the other side stalemated: invalid first, then `Stalemate`, then accepted | line 219: `Current setup is invalid`; line 222: `Stalemate` |
| 229 to 271 | 6 | The side to move has exactly one legal move (`h1 h2`); played, game continues | none |
| 272 to 311 | 7 | The first move stalemates black (`f1 f7`) | none; `Draw` |
| 312 to 351 | 8 | Mate in one (`a1 a8`) | none; White wins |
| 352 to 394 | 9 | Only legal moves are the four a7 promotions (`move a7 a8 N`) | none |
| 395 to 437 | 10 | Black mate in one (`e4 f2`, smothered, `= black`) | none; Black wins |

### Test 2.14: Degenerate moves

| Lines | What it tests |
|---|---|
| 2 to 17 | Same-square moves for a pawn, knight, bishop, rook, queen, king and an empty square, both colours |
| 18 to 45 | Standard game: pinned knights moving off the pin (rejected), pawn rejects (straight capture, diagonal without capture, backward, two squares from a non-start rank) |
| 46 to 109 | Setup 1: queen, rook and bishop jumping over a friendly and over an enemy piece, both colours |
| 110 to 164 | Setup 2: the king capturing a protected piece; the x-ray case (stepping along a rook's line to a square it still attacks) |
| 165 to 230 | Setup 3: the three squares between two kings rejected for each colour; every piece type moving onto its own king rejected |
| 231 to 284 | Setup 4: pinned rook and bishop, moves off the pin line rejected and along it legal; capturing the pinner |

### Test 2.15: Other draw rules

These rules may not be implemented, so every game continues past the draw point and ends with `resign`. The test is there to observe the behaviour.

| Lines | What | Where the condition is reached |
|---|---|---|
| 1 to 41 | K vs k only | already insufficient at `done` |
| 42 to 83 | K+B vs k, reached by `Rxd8+` and `Kxd8` | after line 80 |
| 84 to 125 | K+N vs k, same pattern | after line 122 |
| 126 to 166 | K+B vs k+b, both bishops on dark squares | after line 163 |
| 167 to 210 | Opposite-coloured bishops (not a draw by this rule) | game continues |
| 211 to 251 | K+N+N vs k (not a forced draw) | never |
| 252 to 269 | Threefold repetition from the start (knight shuffles) | third occurrence by line 260 |
| 270 to 293 | Repetition reached by different move orders | third occurrence at line 290 |
| 294 to 434 | Fifty-move rule, K+N vs k+n, no repeated position | halfmove clock reaches 100 after line 429 |

### Test 2.16: Bots where the reply is forced

Bot replies are forced by the position, or every human follow-up is legal against all possible replies.

| Lines | Block (level) | What it covers |
|---|---|---|
| 1 to 93 | 1, 1b (level 1 black, level 3 white) | Bot in check with exactly one reply (a king capture, a single block); the human's next move proves where its piece went |
| 94 to 186 | 2a, 2b (levels 2, 3) | Bot in check with two replies; the follow-up is legal after both |
| 187 to 231 | 3 (level 3) | Exactly one legal move although not in check; then a stalemate by capture |
| 232 to 398 | 4a, 4b, 5a, 5b | Human mates the bot (never asked to move); human stalemates the bot (Draw) |
| 399 to 484 | 6a, 6b | The bot's only moves are four promotions, both colours; ends with `resign` |
| 485 to 531 | 7 (level 2) | The bot's only legal move is a capture |
| 532 to 617 | 9a, 9b | The bot's king has exactly one safe square, then mate |
| 618 to 660 | 10 (level 2) | Three legal king moves, all losing to the same mate |
| 661 to 665 | 8 | `resign` on the bot's turn (bot as white at the start; bot as black after one human move) |

### Test 2.17: Bot level behaviour

Levels follow the course spec: level 2 prefers captures and checks; level 3 also prefers avoiding capture. Each block has a preferred set with one or two moves, and the human follow-up is legal after all of them.

| Lines | Level / colour | Case | Expected bot reply |
|---|---|---|---|
| 1 to 44 | 2 black | One capture, no checks | `Ne3xg4` |
| 45 to 88 | 2 white | One capture, no checks | `Bh5xf3` |
| 89 to 132 | 2 white | One check, no captures | `Re3-e8+` |
| 133 to 175 | 2 black | One check, no captures | `Bb5-d7+` |
| 176 to 219 | 2 white | A capture that is also a check | `Re8xe4+` |
| 220 to 263 | 2 black | A capture that is also a check | `Ra1xe1+` |
| 264 to 351 | 2 white, 2 black | One capture plus one separate check | either of the two |
| 352 to 440 | 3 white, 3 black | A knight attacked by a pawn with one safe square | `Nh1-g3`, `Na8-c7` |
| 441 to 485 | 3 black | A capture while another piece is attacked (the king has two legal moves) | `Kc5xc4` |
| 486 to 528 | 3 black | The only check also rescues the attacked knight | `Nd1-c3+` |
| 529 to 618 | 3 white, 3 black | Two captures available | either capture |
| 619 to 706 | 1 black, 1 white | The bot has exactly two quiet legal moves | either |

The level 3 blocks (352 to 528) are deterministic only under one reading of "avoids capture": that after the move none of the bot's pieces is attackable. Under a weaker reading, quiet king moves also qualify.

## Test 3: Sustainability

Checks that the program keeps working across several games in one run. Scores accumulate across the run (win or resign-win = 1, draw = 0.5 each); the expected running score is in a comment after each game. Games are `game human human` except in 3.8 and 3.9, which also use bots and setups.

| Test | What it checks | File | Size | Expected output |
|---|---|---|---|---|
| 3.1 | A second and third game after the first, with the score display | test3_1.in | 3 games | `test3_1.out` |
| 3.2 | Five normal games | test3_2.in | 5 games | `test3_2.out` |
| 3.3 | A normal game, then a valid setup, then a second game | test3_3.in | 9 games | `test3_3.out` |
| 3.4 | A game after a valid setup, then a normal game | test3_4.in | 6 games | `test3_4.out` |
| 3.5 | Games after a win, a loss, a draw and a resign | test3_5.in | 6 games | `test3_5.out` |
| 3.6 | Final score when the program is quit | test3_6.in | 53 lines | `test3_6.out` |
| 3.7 | Final score when the input ends (EOF) | test3_7.in | 42 lines | `test3_7.out` |
| 3.8 | Full bot-vs-bot games run to the end | test3_8.in | 2420 lines | skipped: bot moves are random |
| 3.9 | 61 games in one run | test3_9.in | 673 lines | skipped: bot moves are random |

### Test 3.1: Second game and score display

| Game | Result | Expected score (W:B) | Probe |
|---|---|---|---|
| 1 | Fool's mate, Black wins | 0:1 | |
| 2 | Scholar's mate, White wins | 1:1 | `e7 e5` rejected first (weak probe: game 1 ended on a black move) |
| 3 | Scholar's mate, White wins | 2:1 | `e7 e5` rejected first (strong probe: game 2 ended on a white move) |

### Test 3.2: Five normal games

| Game | Mating pattern | Winner | Score after (W:B) |
|---|---|---|---|
| 1 | Scholar's mate (queen) | White | 1:0 |
| 2 | Fool's mate (queen) | Black | 1:1 |
| 3 | Legal's mate (knight) | White | 2:1 |
| 4 | Caro-Kann knight trap (knight) | White | 3:1 |
| 5 | Blackburne-Shilling smothered mate (knight) | Black | 3:2 |

### Test 3.3: Setup after a game

| Game | What it is | Score after (W:B) |
|---|---|---|
| A1 | Fool's mate | 0:1 |
| A2 | Custom board K+R vs k+r, white first, black resigns | 1:1 |
| A3 | Short normal game, white resigns | 1:2 |
| B1 | Short normal game, white resigns | 1:3 |
| B2 | Custom board with `= black`, black mates with `d8 d1#` | 1:4 |
| B3 | Short normal game | 1:5 |
| C1 | Scholar's mate | 2:5 |
| C2 | Setup first rejected twice (two white kings, then a pawn on row 8), then a valid board, black resigns | 3:5 |
| C3 | Short normal game | 3:6 |

### Test 3.4: Game after a setup, then a normal game

| Game | What it is | Score after (W:B) |
|---|---|---|
| A1 | Setup K+Q vs k, white resigns | 0:1 |
| A2 | Normal game, Scholar's mate | 1:1 |
| B1 | Setup with `= black`, white's first move rejected, white resigns | 1:2 |
| B2 | Normal game: black's first move rejected, then a White mate | 2:2 |
| C1 | Setup with a pawn about to promote: `a7 a8 Q`, then the queen moves, black resigns | 3:2 |
| C2 | Normal game: the promoted queen is gone, Fool's mate | 3:3 |

### Test 3.5: Games after each kind of ending

| Game | Ends by | Score after (W:B) | Next game's probe |
|---|---|---|---|
| 1 | White checkmate (Scholar's mate) | 1:0 | `e7 e5` rejected, then `e2 e4` |
| 2 | Black checkmate (Fool's mate) | 1:1 | same |
| 3 | Stalemate (Loyd, Black stalemated) | 1.5:1.5 | same |
| 4 | White resigns | 1.5:2.5 | same |
| 5 | Black resigns | 2.5:2.5 | same |
| 6 | Black checkmate (Fool's mate) | 2.5:3.5 | |

### Test 3.6: Final score when the program is quit

The unfinished last game does not count. The three lines after `quit` must never be read.

| Lines | Game | Running score (W:B) |
|---|---|---|
| 1 to 8 | Scholar's mate, White wins | 1:0 |
| 9 to 13 | Fool's mate, Black wins | 1:1 |
| 14 to 33 | Sam Loyd's stalemate | 1.5:1.5 |
| 34 to 37 | `e2 e4`, `e7 e5`, White resigns | 1.5:2.5 |
| 38 to 40 | `e2 e4`, Black resigns | 2.5:2.5 |
| 41 to 43 | `e2 e4`, Black resigns | 3.5:2.5 |
| 44 to 49 | Unfinished game (`d2 d4`, `d7 d5`, `c1 f4`, `g8 f6`, `e2 e3`) | 3.5:2.5 |
| 50 | `quit` while that game is running | final 3.5:2.5 |
| 51 to 53 | `move e2 e4`, `game human human`, `quit`: never read | |

### Test 3.7: Final score at end of input

No `quit`: the input ends in the middle of a running game, which must not count.

| Lines | Game | Running score (W:B) |
|---|---|---|
| 1 to 8 | Scholar's mate, White wins | 1:0 |
| 9 to 12 | `e2 e4`, `e7 e5`, White resigns | 1:1 |
| 13 to 32 | Sam Loyd's stalemate | 1.5:1.5 |
| 33 to 35 | `d2 d4`, Black resigns | 2.5:1.5 |
| 36 to 42 | Unfinished game (six moves, White to move), then end of input | final 2.5:1.5 |

### Test 3.8: Bot-vs-bot games run to the end

Eight games, each `game computer [A] computer [B]` followed by 300 bare `move` lines and a `resign`. A game that ends early ignores the leftover lines. A game still running after 300 moves is ended by the `resign`. Bot results are random, so the `.out` must come from a real run.

| Lines | Game |
|---|---|
| 1 to 302 | [1] vs [1] |
| 303 to 604 | [2] vs [2] |
| 605 to 906 | [3] vs [3] |
| 907 to 1208 | [2] vs [3] |
| 1209 to 1510 | [3] vs [1] |
| 1511 to 1812 | [1] vs [2] |
| 1813 to 2114 | [3] vs [2] |
| 2115 to 2416 | [1] vs [1] |
| 2417 to 2420 | `game human human`, `e2 e4`, `e7 e5`, `resign` (a health check) |

### Test 3.9: 61 games in one run

Used to look for memory growth and state leaking across games (for example under `valgrind`). Expected final score: White 35.5, Black 25.5.

| Game numbers | What |
|---|---|
| Every 12th (12, 24, 36, 48, 60) | Sam Loyd's stalemate (0.5 each) |
| 6, 18, 30, 42, 54 | A setup game ended by `resign` (6, 30, 54: Black resigns; 18, 42: White resigns) |
| 5, 10, 15, 20, 25, 35, 40, 45, 50, 55 | A bot game, at most 3 bot moves, ended by `resign` (human vs bot [1], bot [2] vs human, bot [3] vs bot [1]) |
| 10, 20, 30, 40, 50, 60 | A failed `game computer [9] human` line right before the real game |
| All others | Cycle of Fool's mate, Scholar's mate, Legal's mate, White resigns after 4 plies, Black resigns after 3 plies |
| 61 | `e2 e4`, `e7 e5`, White resigns, then `quit` |
