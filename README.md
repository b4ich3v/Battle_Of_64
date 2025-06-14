# DeathSquare Chess Engine

> **Minimal-STL chess project for Windows**

---

## 1  Project Overview
DeathSquare is a didactic chess engine and graphical client written in C++ **without the Standard Template Library**.*¹*  The only external code is GDI+ for rendering sprites and a handful of Win32 calls for the window loop.  It supports:

* human vs AI & human vs human
* adjustable difficulty (depth, search noise, randomised move pool)
* basic save/load of the current game state (`.dsq`)

The goal is to demonstrate that a full chess stack—move generation, alpha–beta search, GUI and persistence—can live on pure, low-level C++.

---

## 2  Key Features
| Area        | Details |
|-------------|---------|
| **Engine**  | α–β search (depth 1 – 5), centipawn evaluation, optional noise & reservoir-sampling for *human-like* mistakes |
| **Rules**   | Full FIDE legality: castling, en passant, promotion, stalemate, threefold prevention |
| **GUI**     | GDI+ double-buffered board (80 px squares), drag-&-drop moves, animated lobby GIF |
| **Persistence** | Binary writer/reader save the whole *Board* + metadata; version header guards incompatible files |
| **No STL**  | Custom `MyVector`, `MyPair`, manual memory—fits environments where STL is disallowed |

---

## 3  Folder Structure

