#include <windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    MessageBox(NULL, L"这是一个嵌入的程序！二级闹钟已触发！", L"二级闹钟", MB_OK | MB_ICONINFORMATION);
    return 0;
}