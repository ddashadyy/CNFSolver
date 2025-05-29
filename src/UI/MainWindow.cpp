#include <UI/MainWindow.hpp>
#include <UI/GAWindow.hpp>
// #include "UI/SAWindow.hpp"

#include <stdexcept>


namespace ui
{


bool MainWindow::RegisterMWClass(HINSTANCE hInstance) 
{
    WNDCLASSEXW wc = { sizeof(WNDCLASSEXW) };
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
    wc.lpszClassName = CLASS_NAME;

    return RegisterClassExW(&wc) != 0;
}

bool MainWindow::CreateAndShow(HINSTANCE hInstance, int nCmdShow) 
{
    HWND hWnd = CreateWindowExW(
        0,
        CLASS_NAME,
        L"CNF Solver",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        400, 300,
        nullptr,
        nullptr,
        hInstance,
        nullptr
    );

    if (!hWnd) return false;

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return true;
}

LRESULT CALLBACK MainWindow::WindowProc(
    HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam
) 
{
    switch (uMsg) 
    {
    case WM_CREATE:
        OnCreate(hWnd);
        return 0;

    case WM_COMMAND:
        OnCommand(hWnd, LOWORD(wParam));
        return 0;

    case WM_DESTROY:
        OnDestroy(hWnd);
        return 0;

    default:
        return DefWindowProcW(hWnd, uMsg, wParam, lParam);
    }
}

void MainWindow::OnCreate(HWND hWnd) 
{
    CreateWindowW(
        L"BUTTON", L"Генетический алгоритм",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        100, 50, 200, 50, 
        hWnd, 
        reinterpret_cast<HMENU>(ID_BUTTON_GA), 
        nullptr, 
        nullptr
    );

    CreateWindowW(
        L"BUTTON", L"Алгоритм имитации отжига",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        100, 120, 200, 50,
        hWnd,
        reinterpret_cast<HMENU>(ID_BUTTON_SA),
        nullptr,
        nullptr
    );
}

void MainWindow::OnCommand(HWND hWnd, int controlId) 
{
    HINSTANCE hInstance = reinterpret_cast<HINSTANCE>(
        GetWindowLongPtrW(hWnd, GWLP_HINSTANCE)
    );

    switch (controlId) 
    {
    case ID_BUTTON_GA:
    {
        HWND hGAWnd = GAWindow::Create(hWnd, hInstance);
        if (!hGAWnd) 
            MessageBoxW(hWnd, L"Failed to create GA window", L"Error", MB_ICONERROR);
        else 
        {
            ShowWindow(hGAWnd, SW_SHOW);
            UpdateWindow(hGAWnd);
        }
        break;
    }

    // case ID_BUTTON_SA:
    //     if (!SAWindow::Create(hWnd, hInstance)) {
    //         MessageBoxW(hWnd, L"Failed to create SA window", L"Error", MB_ICONERROR);
    //     }
    //     break;
    }
}

void MainWindow::OnDestroy(HWND hWnd) 
{
    PostQuitMessage(0);
}

} // namespace ui
