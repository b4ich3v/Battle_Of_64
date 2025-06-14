# Battle Of 64

## **Description**
"Battle Of 64" is a didactic chess engine and graphical client written in C++.  The only external code is GDI+ for rendering sprites and a handful of Win32 calls for the window loop. It supports:

* human vs AI and human vs human
* adjustable difficulty (depth, search noise, randomised move pool)
* basic save/load of the current game state (`.dsq`)

The goal is to demonstrate that a full chess stack—move generation, alpha–beta search, GUI and persistence—can live on pure, low-level C++.

---

## Key Features
| Area        | Details |
|-------------|---------|
| **Engine**  | α–β search (depth 1 – 5), centipawn evaluation, optional noise and reservoir-sampling for *human-like* mistakes |
| **Rules**   | Full FIDE legality: castling, en passant, promotion, stalemate, threefold prevention |
| **GUI**     | GDI+ double-buffered board (80 px squares), drag-and-drop moves, animated lobby GIF |
| **Persistence** | Binary writer/reader save the whole *Board* plus metadata, version header guards incompatible files |
| **No STL**  | Custom `MyVector`, `MyPair`, manual memory—fits environments where STL is disallowed |
| **Extensibility**  | Visitor pattern separates engine from rendering; swap GUI (SDL, console) or plug new evaluation without touching engine code |
---


