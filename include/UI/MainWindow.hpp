#pragma once

#include <windows.h>

namespace ui
{

constexpr int ID_BUTTON_GA = 1001;
constexpr int ID_BUTTON_SA = 1002;

class MainWindow 
{
public:
    static bool RegisterMWClass(HINSTANCE hInstance);
    static bool CreateAndShow(HINSTANCE hInstance, int nCmdShow);
    
private:
    static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, 
                                     WPARAM wParam, LPARAM lParam);
    
    static void OnCreate(HWND hWnd);
    static void OnCommand(HWND hWnd, int controlId);
    static void OnDestroy(HWND hWnd);
    
    static inline const wchar_t* CLASS_NAME = L"CNF_Solver_MainWindow";
};

} // namespace ui


