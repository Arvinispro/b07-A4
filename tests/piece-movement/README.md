# Piece-movement tests

Each line of `testSuite.txt` names a pair of files. The `.in` file contains the chess commands. The `.out` file shows the last board, any check message after the attempted move, the resignation result, and the final score.

Run a case from the repository root once `chess` builds. For example, in Bash:

```sh
./chess < tests/piece-movement/rook_blocked_by_a_friendly_pawn.in
```

Compare the last board and score to the matching `.out`. The game can also print boards during setup and after earlier moves. Those lines, prompts, and move rejection messages are outside this comparison. Spaces at the ends of board rows represent empty light squares.

Each input ends with `resign` to verify whose turn it is. In the blocked-rook case, the rook stays at a1. White still has the turn and resigns, so Black wins.

## Rook

| Case | Attempt | Expected |
| --- | --- | --- |
| [Rook captures on an open file](rook_captures_on_an_open_file.in) | `a1 to a6` | Accept |
| [Rook blocked by a friendly pawn](rook_blocked_by_a_friendly_pawn.in) | `a1 to a6` | Reject |
| [Rook cannot jump an enemy pawn](rook_cannot_jump_an_enemy_pawn.in) | `a1 to a6` | Reject |

## Bishop

| Case | Attempt | Expected |
| --- | --- | --- |
| [Bishop captures on an open diagonal](bishop_captures_on_an_open_diagonal.in) | `c1 to h6` | Accept |
| [Bishop blocked on the diagonal](bishop_blocked_on_the_diagonal.in) | `c1 to h6` | Reject |

## Queen

| Case | Attempt | Expected |
| --- | --- | --- |
| [Queen travels along a diagonal](queen_travels_along_a_diagonal.in) | `d4 to a7` | Accept |
| [Queen travels along a file](queen_travels_along_a_file.in) | `d4 to d7` | Accept |
| [Queen cannot move as a knight](queen_cannot_move_as_a_knight.in) | `d4 to e6` | Reject |
| [Queen cannot capture a friendly piece](queen_cannot_capture_a_friendly_piece.in) | `d4 to d7` | Reject |

## Knight

| Case | Attempt | Expected |
| --- | --- | --- |
| [Knight jumps over the opening pawn line](knight_jumps_over_the_opening_pawn_line.in) | `b1 to c3` | Accept |
| [Knight cannot move in a straight line](knight_cannot_move_in_a_straight_line.in) | `b1 to b3` | Reject |
| [Knight cannot land on a teammate](knight_cannot_land_on_a_teammate.in) | `b1 to d2` | Reject |

## King

| Case | Attempt | Expected |
| --- | --- | --- |
| [King steps to a safe square](king_steps_to_a_safe_square.in) | `d1 to c1` | Accept |
| [King cannot enter an attacked file](king_cannot_enter_an_attacked_file.in) | `d1 to e1` | Reject |
| [Kings cannot become adjacent](kings_cannot_become_adjacent.in) | `e1 to e2` | Reject |

## Pawn

| Case | Attempt | Expected |
| --- | --- | --- |
| [White pawn initial double move](white_pawn_initial_double_move.in) | `e2 to e4` | Accept |
| [Black pawn initial double move](black_pawn_initial_double_move.in) | `d7 to d5` | Accept |
| [Pawn cannot jump a blocker](pawn_cannot_jump_a_blocker.in) | `e2 to e4` | Reject |
| [Pawn cannot capture straight ahead](pawn_cannot_capture_straight_ahead.in) | `e2 to e3` | Reject |
| [Pawn captures diagonally](pawn_captures_diagonally.in) | `e4 to d5` | Accept |
| [Pawn cannot capture on an empty diagonal](pawn_cannot_capture_on_an_empty_diagonal.in) | `e2 to d3` | Reject |
| [Pawn cannot move backward](pawn_cannot_move_backward.in) | `e4 to e3` | Reject |
| [Pawn cannot double-step a second time](pawn_cannot_double_step_a_second_time.in) | `e4 to e6` | Reject |

## King safety

| Case | Attempt | Expected |
| --- | --- | --- |
| [Pinned rook cannot expose its own king](pinned_rook_cannot_expose_its_own_king.in) | `e2 to d2` | Reject |
| [Pinned rook can stay on the same file](pinned_rook_can_stay_on_the_same_file.in) | `e2 to e3` | Accept |

## Castling

| Case | Attempt | Expected |
| --- | --- | --- |
| [White castles kingside and rook follows](white_castles_kingside_and_rook_follows.in) | `e1 to g1` | Accept |
| [White castles queenside and rook follows](white_castles_queenside_and_rook_follows.in) | `e1 to c1` | Accept |
| [Black castles kingside and rook follows](black_castles_kingside_and_rook_follows.in) | `e8 to g8` | Accept |
| [Castling with pieces in the way](castling_with_pieces_in_the_way.in) | `e1 to g1` | Reject |
| [Castling out of check](castling_out_of_check.in) | `e1 to g1` | Reject |
| [Castling through an attacked square](castling_through_an_attacked_square.in) | `e1 to g1` | Reject |
| [Castling onto an attacked square](castling_onto_an_attacked_square.in) | `e1 to g1` | Reject |
| [Castling after the king returns home](castling_after_the_king_returns_home.in) | `e1 to g1` | Reject |
| [Castling after the rook returns home](castling_after_the_rook_returns_home.in) | `e1 to g1` | Reject |

## En passant

| Case | Attempt | Expected |
| --- | --- | --- |
| [Immediate en passant removes the bypassed pawn](immediate_en_passant_removes_the_bypassed_pawn.in) | `e5 to d6` | Accept |
| [En passant expires after one reply](en_passant_expires_after_one_reply.in) | `e5 to d6` | Reject |
| [En passant cannot uncover check](en_passant_cannot_uncover_check.in) | `e5 to d6` | Reject |
| [Black captures en passant](black_captures_en_passant.in) | `e4 to d3` | Accept |

## Promotion

| Case | Attempt | Expected |
| --- | --- | --- |
| [White pawn promotes to queen](white_pawn_promotes_to_queen.in) | `e7 to e8 Q` | Accept |
| [White pawn promotes to rook](white_pawn_promotes_to_rook.in) | `e7 to e8 R` | Accept |
| [White pawn promotes to bishop](white_pawn_promotes_to_bishop.in) | `e7 to e8 B` | Accept |
| [White pawn promotes to knight](white_pawn_promotes_to_knight.in) | `e7 to e8 N` | Accept |
| [Black promotes while capturing](black_promotes_while_capturing.in) | `e2 to d1 n` | Accept |
