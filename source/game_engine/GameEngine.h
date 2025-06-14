#define UNICODE
#define _UNICODE
#include <windows.h>
#include <gdiplus.h>
#include "VisitorVisualization.h"
#include "Board.h"
#include "Move.h"
#include "Position.h"
#include "Player.h"
#include "HumanPlayer.h"
#include "AIPlayer.h"
#pragma once

class GameEngine
{
private:

    VisitorVisualization* vizualizator = nullptr; // draws board and pieces
    bool fromFile = false; // set after Load Game
    Gdiplus::Image* sideImage = nullptr; // right-panel artwork

    Player* whitePlayer = nullptr; // owned pointers
    Player* blackPlayer = nullptr;
    int mode = 0; // 0 = PvP, 1 = vs Bot

    Gdiplus::Image* gif = nullptr; GUID dimID; // frame dimension ID
    UINT frameCount{}, currentFrame{}, * delays{}; // per-frame timing
    UINT timerID{}; // WM_TIMER handle

    HINSTANCE hInst{};
    HWND hMainWnd{}, hChessWnd{};

    bool dragging = false;
    int drag_from_row{}, drag_from_col{};
    int drag_mouse_x{}, drag_mouse_y{};
    Piece drag_piece = Piece::NONE;

    MyColor currentTurn = MyColor::WHITE; // side to move

    GameEngine(); // private–singleton
    ~GameEngine(); // cleanup and GDI+ shut

    static LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM); // lobby
    static LRESULT CALLBACK ChessWndProc(HWND, UINT, WPARAM, LPARAM); // board

    void initGifWindow(); // create lobby window

    void initChessWindow(); // create chess window

    void paintChess(HWND, HDC); // double-buffered redraw

    FigureType askPromotion(HWND parent); // modal choose-piece dialog

    ULONG_PTR gdipToken{ 0 };

public:

    static GameEngine& instance(); // Meyers singleton

    int  run(HINSTANCE hInst, int nShow); // WinMain loop

    bool saveGame(Writer&); // write .sav file

    bool loadGame(Reader&); // read .sav file

};
