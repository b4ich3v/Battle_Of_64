# Battle Of 64

## **Description**
"Battle Of 64" is a didactic chess engine and graphical client written C++ with zero reliance on the Standard Template Library (STL).  All memory is managed manually through bespoke containers (MyVector, MyPair) and smart, scope‑bound ownership rules, the only external code is GDI+ (bundled with Windows) for sprite blitting plus a thin layer of raw Win32 API calls for the message loop and window creation.

The project aims to prove that a complete chess stack-from bit-level move generation to polished drag‑and‑drop GUI - can be built on bare‑metal C++ while still offering modern conveniences such as adjustable AI strength and save/load support. It also serves as a teaching resource for:

* Search algorithms - a clean α‑β implementation with tunable depth, noise injection and reservoir sampling.
* Game‑rule enforcement - full FIDE legality including castling state, en passant, promotion choices, stalemate and threefold prevention.
* Design patterns in practice
  * Singleton (Board, GameEngine) centralise state and window management.
  * Visitor separates rendering from logic (VisitorVisualization), allows future SDL/Console views.
  * Factory Method (FigureFactory) instantiates pieces without switch‑sprawl.
* Separation of concerns - Visitor pattern decouples core logic from any rendering backend, swapping GUI or AI is trivial.
* Low‑level Win32 / GDI+ programming - double‑buffering, custom cursors, modal dialogs, GIF animation without frameworks.

The goal is to demonstrate that a full chess stack-move generation, alpha-beta search, GUI and persistence - can live on pure, low-level C++.

---

## Key Features
| Area        | Details |
|-------------|---------|
| **Engine**  | α-β search (depth 1 – 5), centipawn evaluation, optional noise and reservoir-sampling for *human-like* mistakes |
| **Rules**   | Full FIDE legality: castling, en passant, promotion, stalemate, threefold prevention |
| **GUI**     | GDI+ double-buffered board (80 px squares), drag-and-drop moves, animated lobby GIF |
| **Persistence** | Binary writer/reader save the whole *Board* plus metadata, version header guards incompatible files |
| **No STL**  | Custom `MyVector`, `MyPair`, manual memory-fits environments where STL is disallowed |
| **Extensibility**  | Visitor pattern separates engine from rendering; swap GUI (SDL, console) or plug new evaluation without touching engine code |
---


