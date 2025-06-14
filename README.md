# Battle Of 64

## **Description**
**Battle Of 64** is a didactic chess engine and graphical client written C++ with zero reliance on the Standard Template Library (STL).  All memory is managed manually through bespoke containers (MyVector, MyPair) and smart, scope‑bound ownership rules, the only external code is GDI+ (bundled with Windows) for sprite blitting plus a thin layer of raw Win32 API calls for the message loop and window creation.

The project aims to prove that a complete chess stack-from bit-level move generation to polished drag‑and‑drop GUI - can be built on bare‑metal C++ while still offering modern conveniences such as adjustable AI strength and save/load support. It also serves as a teaching resource for:

* Search algorithms - a clean `α`‑`β` implementation with tunable depth, noise injection and reservoir sampling.
* Game‑rule enforcement - full FIDE legality including castling state, en passant, promotion choices, stalemate and threefold prevention.
* Design patterns in practice
  * Singleton (`Board`, `GameEngine`) centralise state and window management.
  * Visitor separates rendering from logic (`VisitorVisualization`), allows future SDL/Console views.
  * Factory Method (`FigureFactory`) instantiates pieces without switch‑sprawl.
* Separation of concerns - Visitor pattern decouples core logic from any rendering backend, swapping GUI or AI is trivial.
* Low‑level Win32 / GDI+ programming - double‑buffering, custom cursors, modal dialogs, GIF animation without frameworks.

The goal is to demonstrate that a full chess stack-move generation, alpha-beta search, GUI and persistence - can live on pure, low-level C++.

---

### **Key Features**
| Area        | Details |
|-------------|---------|
| **Engine**  | `α`-`β` search (depth 1 – 5), centipawn evaluation, optional noise and reservoir-sampling for human-like mistakes |
| **Rules**   | Full FIDE legality: castling, en passant, promotion, stalemate, threefold prevention |
| **GUI**     | GDI+ double-buffered board (80 px squares), drag-and-drop moves, animated lobby GIF |
| **Persistence** | Binary writer/reader save the whole *Board* plus metadata, version header guards incompatible files |
| **No STL**  | Custom `MyVector`, `MyPair`, manual memory-fits environments where STL is disallowed |
| **Extensibility**  | Visitor pattern separates engine from rendering; swap GUI (SDL, console) or plug new evaluation without touching engine code |

---

## **Game Rules and Modes**

### **Rule Enforcement**
* **Complete FIDE rule set** – castling, en passant, promotion, stalemate, checkmate, threefold‑repetition prevention (50‑move rule planned).
* **Legality guard** – every candidate passes `isLegalMove`; the king is never left in check.
* **Castling rights** tracking – flags updated on king/rook moves and restored on undo.
* **En passant squares** – generated only on double‑pawn pushes and valid for exactly one reply.
* **Promotion dialog** – modal window lets you pick Queen, Rook, Bishop or Knight.

### **Play Modes**
| Area        | Description | Default Side  |
|-------------|---------|---------|
| **PvP**  | Human vs Human (same PC) | White and Black = users |
| **Beginner Bot**   | Human vs AI (depth 1 + noise) | White = user |
| **Easy Bot**     | Human vs AI (depth 2) | White = user |
| **Medium Bot** | Human vs AI (depth 3) | White = user | 
| **Hard Bot**  | Human vs AI (depth 5, no noise) | White = user | 
| **AI Mirror**  | AI vs AI demo (toggle with F5) | N/A |

---

## **Limitations**
* **Material‑only evaluation** – no piece‑square tables, king safety or mobility, the AI compensates with depth and random noise but positional understanding is shallow.
* **No move ordering / quiescence / transposition table** - alpha‑beta explores many redundant nodes, depths beyond 5 become slow.
* **No opening book nor end‑game tablebases** - the engine must "discover" every move, early game looks naive, late end‑gam-es may miss forced mates.
* **Single‑threaded** - search runs on the GUI thread, large depths can freeze the interface for >1 s.
* **Windows‑only backend** - tight coupling to Win32 API and GDI+, porting requires replacing window / input / rendering code.
* **Raw pointers / manual memory** - smart‑pointer migration is planned once STL usage is approved.
* **Limited draw detection** - threefold repetition works, 50‑move rule is todo, insufficient‑material check is absent.
* **No pondering or background thinking** - AI idles during opponent time, search restarts every move.
* **Binary save only** - no PGN/FEN export yet, saves are engine-specific.
