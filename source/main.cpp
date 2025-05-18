#include "GameEngine.h"

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int nShow)
{

    return GameEngine::instance().run(hInst, nShow);

}
