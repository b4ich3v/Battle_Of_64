#include "GameEngine.h"
#include "Figure.h"
#include "BinaryReader.h"
#include "BinaryWriter.h"
#include <cstdlib>
#include <commdlg.h>                 
#pragma comment(lib, "Comdlg32.lib") 

using namespace Gdiplus;

#ifndef GET_X_LPARAM
#define GET_X_LPARAM(lp) ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp) ((int)(short)HIWORD(lp))
#endif

GameEngine& GameEngine::instance()
{

    static GameEngine inst;
    return inst;

}

GameEngine::GameEngine() :
    gif(nullptr), delays(nullptr),
    whitePlayer(nullptr), blackPlayer(nullptr),
    mode(0), vizualizator(nullptr), fromFile(false)
{

    GdiplusStartupInput in{};

    if (GdiplusStartup(&gdipToken, &in, nullptr) != Ok)
        throw std::runtime_error("Cannot start GDI+");

    vizualizator = new VisitorVisualization(80);

}

GameEngine::~GameEngine()
{

    delete whitePlayer;
    delete blackPlayer;
    delete vizualizator;
    delete gif;
    delete[] delays;
    GdiplusShutdown(gdipToken);

}

int GameEngine::run(HINSTANCE inst, int nShow)
{

    hInst = inst;


    initGifWindow();
    ShowWindow(hMainWnd, nShow);

    MSG message;

    while (GetMessage(&message, nullptr, 0, 0))
    {

        TranslateMessage(&message);
        DispatchMessage(&message);

    }

    return static_cast<int>(message.wParam);

}

void GameEngine::initGifWindow()
{

    WNDCLASS wc{};
    wc.lpszClassName = L"GIFWnd";
    wc.hInstance = hInst;
    wc.lpfnWndProc = MainWndProc;
    wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
    RegisterClass(&wc);

    hMainWnd = CreateWindowW(L"GIFWnd", L"GDI+ Animated GIF", WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
        CW_USEDEFAULT, CW_USEDEFAULT, 1280, 740,
        nullptr, nullptr, hInst, nullptr);

    LONG style = GetWindowLong(hMainWnd, GWL_STYLE);

    style &= ~(WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_THICKFRAME);
    SetWindowLong(hMainWnd, GWL_STYLE, style);
    SetWindowPos(hMainWnd, nullptr, 0, 0, 0, 0,
        SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

}

void GameEngine::initChessWindow()
{

    WNDCLASS wc{};

    wc.lpszClassName = L"ChessWnd";
    wc.hInstance = hInst;
    wc.lpfnWndProc = ChessWndProc;
    wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    RegisterClass(&wc);

    hChessWnd = CreateWindowExW(WS_EX_COMPOSITED,
        L"ChessWnd", L"Battlefield", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 681,
        nullptr, nullptr, hInst, nullptr);

}

struct PromoData
{
public:

    FigureType result;

};

static LRESULT CALLBACK PromoProc(HWND wnd, UINT msg, WPARAM wp, LPARAM lp)
{

    PromoData* promoData = reinterpret_cast<PromoData*>(GetWindowLongPtr(wnd, GWLP_USERDATA));

    switch (msg)
    {

    case WM_CREATE:
    {

        promoData = reinterpret_cast<PromoData*>(reinterpret_cast<LPCREATESTRUCT>(lp)->lpCreateParams);
        SetWindowLongPtr(wnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(promoData));
        return 0;

    }
    case WM_COMMAND:
    {

        switch (LOWORD(wp))
        {

        case 100: promoData->result = FigureType::QUEEN; EndDialog(wnd, 1); break;
        case 101: promoData->result = FigureType::ROOK; EndDialog(wnd, 1); break;
        case 102: promoData->result = FigureType::BISHOP; EndDialog(wnd, 1); break;
        case 103: promoData->result = FigureType::KNIGHT; EndDialog(wnd, 1); break;

        }

        break;

    }
    case WM_CLOSE: EndDialog(wnd, 0); break;

    }

    return 0;

}

FigureType GameEngine::askPromotion(HWND parent)
{

    static bool registered = false;

    if (!registered)
    {

        WNDCLASS wc{}; wc.lpszClassName = L"PromoDlg"; wc.hInstance = hInst; wc.lpfnWndProc = PromoProc; wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
        RegisterClass(&wc); registered = true;

    }

    PromoData promoData{ FigureType::QUEEN };
    HWND dlg = CreateWindowEx(WS_EX_DLGMODALFRAME, L"PromoDlg", L"Promotion",
        WS_POPUP | WS_CAPTION | WS_SYSMENU,
        CW_USEDEFAULT, CW_USEDEFAULT, 340, 120,
        parent, nullptr, hInst, &promoData);

    CreateWindowW(L"STATIC", L"Choose piece:", WS_CHILD | WS_VISIBLE,
        10, 10, 320, 20, dlg, nullptr, hInst, nullptr);
    CreateWindowW(L"BUTTON", L"Queen", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        10, 40, 75, 35, dlg, reinterpret_cast<HMENU>(100), hInst, nullptr);
    CreateWindowW(L"BUTTON", L"Rook", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        90, 40, 75, 35, dlg, reinterpret_cast<HMENU>(101), hInst, nullptr);
    CreateWindowW(L"BUTTON", L"Bishop", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        170, 40, 75, 35, dlg, reinterpret_cast<HMENU>(102), hInst, nullptr);
    CreateWindowW(L"BUTTON", L"Knight", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        250, 40, 75, 35, dlg, reinterpret_cast<HMENU>(103), hInst, nullptr);

    RECT pr; GetWindowRect(parent, &pr);
    int px = pr.left + (pr.right - pr.left) / 2 - 170;
    int py = pr.top + (pr.bottom - pr.top) / 2 - 60;
    SetWindowPos(dlg, HWND_TOP, px, py, 340, 120, SWP_SHOWWINDOW);

    EnableWindow(parent, FALSE);
    MSG message;

    while (IsWindow(dlg) && GetMessage(&message, nullptr, 0, 0))
    {

        if (!IsDialogMessage(dlg, &message)) { TranslateMessage(&message); DispatchMessage(&message); }

    }

    EnableWindow(parent, TRUE);
    SetActiveWindow(parent);

    return promoData.result;

}

void GameEngine::paintChess(HWND wnd, HDC hdc)
{

    RECT rc; GetClientRect(wnd, &rc);
    int w = rc.right - rc.left;
    int h = rc.bottom - rc.top;

    HDC mem = CreateCompatibleDC(hdc);
    HBITMAP bm = CreateCompatibleBitmap(hdc, w, h);
    HBITMAP old = static_cast<HBITMAP>(SelectObject(mem, bm));

    Graphics g(mem);
    g.SetSmoothingMode(SmoothingModeHighQuality);
    g.SetInterpolationMode(InterpolationModeHighQualityBicubic);

    vizualizator->setGraphics(&g);
    Board::instance().accept(*vizualizator);

    if (dragging && drag_piece != Piece::NONE)
    {

        Image* img = vizualizator->getImage(drag_piece);
        if (img) g.DrawImage(img, drag_mouse_x - 40, drag_mouse_y - 40, 80, 80);

    }

    BitBlt(hdc, 0, 0, w, h, mem, 0, 0, SRCCOPY);
    SelectObject(mem, old); DeleteObject(bm); DeleteDC(mem);

}

LRESULT CALLBACK GameEngine::MainWndProc(HWND wnd, UINT msg, WPARAM wp, LPARAM lp)
{

    GameEngine& engine = GameEngine::instance();

    switch (msg)
    {

    case WM_CREATE:
    {

        engine.gif = Image::FromFile(L"background.gif");

        if (!engine.gif || engine.gif->GetLastStatus() != Ok)
        {

            MessageBoxW(wnd, L"Cannot load test.gif", L"Error", MB_ICONERROR);
            PostQuitMessage(-1);
            return 0;

        }

        UINT dimCnt = engine.gif->GetFrameDimensionsCount();
        GUID* dims = new GUID[dimCnt];
        engine.gif->GetFrameDimensionsList(dims, dimCnt);
        engine.dimID = dims[0];
        delete[] dims;

        engine.frameCount = engine.gif->GetFrameCount(&engine.dimID);

        UINT sz = engine.gif->GetPropertyItemSize(PropertyTagFrameDelay);
        PropertyItem* pi = static_cast<PropertyItem*>(std::malloc(sz));
        engine.gif->GetPropertyItem(PropertyTagFrameDelay, sz, pi);
        UINT n = pi->length / sizeof(UINT);
        engine.delays = new UINT[n];

        for (UINT i = 0; i < n; i++)
            engine.delays[i] = (reinterpret_cast<UINT*>(pi->value))[i] * 10;
        std::free(pi);

        SetTimer(wnd, engine.timerID, engine.delays[0], nullptr);

        CreateWindowW(L"BUTTON", L"Start new game",          
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            1050, 100, 200, 40,
            wnd, reinterpret_cast<HMENU>(1001), engine.hInst, nullptr);

        CreateWindowW(L"BUTTON", L"Load game",               
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            1050, 150, 200, 40,
            wnd, reinterpret_cast<HMENU>(1004), engine.hInst, nullptr);

        CreateWindowW(L"BUTTON", L"Options",                 
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            1050, 200, 200, 40,
            wnd, reinterpret_cast<HMENU>(1002), engine.hInst, nullptr);

        CreateWindowW(L"BUTTON", L"Exit",                    
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            1050, 250, 200, 40,
            wnd, reinterpret_cast<HMENU>(1003), engine.hInst, nullptr);


        return 0;

    }
    case WM_TIMER:
    {

        engine.currentFrame = (engine.currentFrame + 1) % engine.frameCount;
        engine.gif->SelectActiveFrame(&engine.dimID, engine.currentFrame);
        KillTimer(wnd, engine.timerID);
        SetTimer(wnd, engine.timerID, engine.delays[engine.currentFrame], nullptr);
        InvalidateRect(wnd, nullptr, FALSE);

        return 0;

    }
    case WM_ERASEBKGND:
    {

        return 1;

    }
    case WM_PAINT:
    {

        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(wnd, &ps);
        Graphics g(hdc);
        g.DrawImage(engine.gif, 0, 0, engine.gif->GetWidth(), engine.gif->GetHeight());
        EndPaint(wnd, &ps);

        return 0;

    }
    case WM_COMMAND:
    {

        switch (LOWORD(wp))
        {

        case 1001:
        {

            engine.mode = (MessageBoxW(wnd,
                L"Start vs Bot (Yes) or 2-Player local (No)?",
                L"Choose mode", MB_YESNO | MB_ICONQUESTION) == IDYES) ? 1 : 0;

            engine.initChessWindow();
            ShowWindow(engine.hChessWnd, SW_SHOW);
            UpdateWindow(engine.hChessWnd);
            ShowWindow(wnd, SW_HIDE);

            break;

        }
        case 1002:
        {

            MessageBoxW(wnd, L"Options pressed", L"Info", MB_OK);
            break;

        }
        case 1003:
        {

            PostQuitMessage(0);
            break;

        }
        case 1004:           
        {

            wchar_t pathW[MAX_PATH]{};
            OPENFILENAMEW ofn{ sizeof(ofn) };
            ofn.hwndOwner = wnd;
            ofn.lpstrFilter = L"Save files\0*.sav\0";
            ofn.lpstrFile = pathW;
            ofn.nMaxFile = MAX_PATH;

            if (GetOpenFileNameW(&ofn))
            {

                char pathA[MAX_PATH];
                WideCharToMultiByte(CP_UTF8, 0, pathW, -1,
                    pathA, MAX_PATH, nullptr, nullptr);

                BinaryReader br;

                if (br.open(pathA) && engine.loadGame(br))
                {

                    br.close();
                    engine.fromFile = true;
                    engine.initChessWindow();
                    ShowWindow(engine.hChessWnd, SW_SHOW);
                    UpdateWindow(engine.hChessWnd);
                    ShowWindow(wnd, SW_HIDE);

                }

            }

            break;

        }

        }

        return 0;

    }
    case WM_DESTROY:
    {

        KillTimer(wnd, engine.timerID);
        return 0;

    }

    }

    return DefWindowProc(wnd, msg, wp, lp);

}

LRESULT CALLBACK GameEngine::ChessWndProc(HWND wnd, UINT  msg, WPARAM wp, LPARAM lp)
{

    GameEngine& engine = GameEngine::instance();
    constexpr int SZ = 80;

    switch (msg)
    {

    case WM_CREATE:
    {

        if (!engine.fromFile)                 
            Board::instance().setupInitialPosition();
        engine.fromFile = false;

        delete engine.whitePlayer;
        delete engine.blackPlayer;

        engine.whitePlayer = new HumanPlayer;
        engine.blackPlayer = (engine.mode == 0)
            ? static_cast<Player*>(new HumanPlayer)
            : static_cast<Player*>(new AIPlayer);

        engine.dragging = false;
        engine.currentTurn = MyColor::WHITE;

        CreateWindowW(L"BUTTON", L"Save",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            657 + 10, 10, 100, 30,
            wnd, reinterpret_cast<HMENU>(2001), engine.hInst, nullptr);

        return 0;

    }
    case WM_PAINT:
    {

        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(wnd, &ps);
        engine.paintChess(wnd, hdc);
        EndPaint(wnd, &ps);

        return 0;

    }
    case WM_LBUTTONDOWN:
    {

        int x = GET_X_LPARAM(lp), y = GET_Y_LPARAM(lp);
        int col = x / SZ, row = y / SZ;
        if (row < 0 || row > 7 || col < 0 || col > 7) return 0;

        Figure* figure = nullptr;
        try { figure = Board::instance().at({ (int8_t)row,(int8_t)col }); }
        catch (...) {}

        if (figure && figure->getColor() == engine.currentTurn)
        {

            engine.drag_piece = engine.vizualizator->figureToPiece(*figure);

            if (engine.drag_piece != Piece::NONE)
            {

                engine.dragging = true;
                engine.drag_from_row = row;
                engine.drag_from_col = col;
                engine.drag_mouse_x = x;
                engine.drag_mouse_y = y;
                SetCapture(wnd);
                InvalidateRect(wnd, nullptr, FALSE);

            }

        }

        return 0;

    }
    case WM_MOUSEMOVE:
    {

        if (engine.dragging)
        {

            engine.drag_mouse_x = GET_X_LPARAM(lp);
            engine.drag_mouse_y = GET_Y_LPARAM(lp);
            InvalidateRect(wnd, nullptr, FALSE);

        }

        return 0;

    }
    case WM_LBUTTONUP:
    {

        if (!engine.dragging) return 0;

        int x = GET_X_LPARAM(lp), y = GET_Y_LPARAM(lp);
        int col = x / SZ, row = y / SZ;

        if (row >= 0 && row < 8 && col >= 0 && col < 8)
        {

            Position from{ (int8_t)engine.drag_from_row,(int8_t)engine.drag_from_col };
            Position to{ (int8_t)row,(int8_t)col };

            if (from.row != to.row || from.col != to.col)
            {

                Figure* figure = Board::instance().at(from);

                if (figure && figure->getColor() == engine.currentTurn)
                {

                    auto moves = figure->generateMoves(Board::instance(), from);
                    bool done = false;

                    for (size_t i = 0; i < moves.size() && !done; i++)
                    {

                        bool promoPawn =
                            figure->getType() == FigureType::PAWN &&
                            ((engine.currentTurn == MyColor::WHITE && to.row == 0) ||
                                (engine.currentTurn == MyColor::BLACK && to.row == 7)) &&
                            moves[i].to == to;

                        if (promoPawn)
                        {

                            FigureType pt = engine.askPromotion(wnd);

                            for (size_t j = 0; j < moves.size(); j++)
                            {

                                if (moves[j].to == to &&
                                    moves[j].promotionType == pt &&
                                    Board::instance().isLegalMove(moves[j], engine.currentTurn))
                                {

                                    Board::instance().applyMove(moves[j]);
                                    (engine.currentTurn == MyColor::WHITE ? engine.whitePlayer :
                                        engine.blackPlayer)->setPendingMove(moves[j]);
                                    engine.currentTurn = oppositeColor(engine.currentTurn);

                                    Player* bot = (engine.currentTurn == MyColor::WHITE ? engine.whitePlayer : engine.blackPlayer);

                                    if (dynamic_cast<AIPlayer*>(bot))
                                    {

                                        Move ai = bot->getMove(Board::instance(), engine.currentTurn);

                                        if (Board::instance().isLegalMove(ai, engine.currentTurn))
                                            Board::instance().applyMove(ai);

                                        engine.currentTurn = oppositeColor(engine.currentTurn);

                                    }

                                    done = true;
                                    break;

                                }

                            }

                        }
                        else if (moves[i].to == to && Board::instance().isLegalMove(moves[i], engine.currentTurn))
                        {

                            Board::instance().applyMove(moves[i]);
                            (engine.currentTurn == MyColor::WHITE ? engine.whitePlayer :
                                engine.blackPlayer)->setPendingMove(moves[i]);
                            engine.currentTurn = oppositeColor(engine.currentTurn);

                            Player* bot = (engine.currentTurn == MyColor::WHITE ? engine.whitePlayer : engine.blackPlayer);

                            if (dynamic_cast<AIPlayer*>(bot))
                            {

                                Move ai = bot->getMove(Board::instance(), engine.currentTurn);

                                if (Board::instance().isLegalMove(ai, engine.currentTurn))
                                    Board::instance().applyMove(ai);

                                engine.currentTurn = oppositeColor(engine.currentTurn);
                            }

                            done = true;

                        }

                    }

                }

            }

        }

        engine.dragging = false;
        engine.drag_piece = Piece::NONE;
        ReleaseCapture();
        InvalidateRect(wnd, nullptr, FALSE);

        return 0;

    }
    case WM_COMMAND:
    {

        switch (LOWORD(wp))
        {

        case 2001:               
        {

            wchar_t pathW[MAX_PATH]{};
            OPENFILENAMEW ofn{ sizeof(ofn) };
            ofn.hwndOwner = wnd;
            ofn.lpstrFilter = L"Save files\0*.sav\0";
            ofn.lpstrFile = pathW;
            ofn.nMaxFile = MAX_PATH;
            ofn.Flags = OFN_OVERWRITEPROMPT;

            if (GetSaveFileNameW(&ofn))
            {

                char pathA[MAX_PATH];
                WideCharToMultiByte(CP_UTF8, 0, pathW, -1,
                    pathA, MAX_PATH, nullptr, nullptr);

                BinaryWriter bw;

                if (bw.open(pathA))
                {

                    engine.saveGame(bw);
                    bw.close();

                }

            }

            return 0;

        }

        }

        break;     

    }
    case WM_DESTROY:
    {

        return 0;

    }

    }

    return DefWindowProc(wnd, msg, wp, lp);

}

bool GameEngine::saveGame(Writer& writer)
{

    const char magic[4] = { 'D', 'S', 'Q', '1' };          
    if (!writer.write(magic, 4)) return false;

    int32_t m = mode;                                      
    if (!writer.write(&m, sizeof(m))) return false;

    int8_t t = static_cast<int8_t>(currentTurn);           
    if (!writer.write(&t, sizeof(t))) return false;

    Board::instance().serialize(writer);                        
    return true;

}

bool GameEngine::loadGame(Reader& reader)
{

    char magic[4];

    if (!reader.read(magic, 4) || strncmp(magic, "DSQ1", 4) != 0) return false;
    if (!reader.read(&mode, sizeof(mode))) return false;

    int8_t t;
    if (!reader.read(&t, sizeof(t))) return false;
    currentTurn = static_cast<MyColor>(t);

    Board::instance().deserialize(reader);                      

    delete whitePlayer;
    delete blackPlayer;

    whitePlayer = new HumanPlayer;
    blackPlayer = (mode == 0)
        ? static_cast<Player*>(new HumanPlayer)
        : static_cast<Player*>(new AIPlayer);

    return true;

}

