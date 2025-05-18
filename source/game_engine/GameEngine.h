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

    VisitorVisualization* vizualizator = nullptr;

    Player* whitePlayer = nullptr;
    Player* blackPlayer = nullptr;
    int mode = 0;

    Gdiplus::Image* gif = nullptr;
    GUID dimID;
    UINT frameCount, currentFrame;
    UINT* delays = nullptr;
    UINT timerID;

    HINSTANCE hInst;
    HWND hMainWnd;
    HWND hChessWnd;

    bool dragging = 0;
    int drag_from_row, drag_from_col = 0;
    int drag_mouse_x, drag_mouse_y = 0;
    Piece drag_piece;
    MyColor currentTurn;

    GameEngine();

    ~GameEngine();

    ULONG_PTR gdipToken{ 0 };

    static LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);

    static LRESULT CALLBACK ChessWndProc(HWND, UINT, WPARAM, LPARAM);

    void initGifWindow();

    void initChessWindow();

    void paintChess(HWND hWnd, HDC hdc);

    FigureType askPromotion(HWND parent);

public:

    static GameEngine& instance();

    int run(HINSTANCE hInst, int nShow);

};
