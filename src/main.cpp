#include <Algorithms/GeneticAlgorithm.hpp>

#include <UI/MainWindow.hpp>
#include <UI/GAWindow.hpp>

#include <iostream>
#include <stdexcept>
#include <string>

#include <windows.h>
#include <commctrl.h> 

int WINAPI wWinMain(
    _In_ HINSTANCE hInstance,
    [[maybe_unused]] _In_opt_ HINSTANCE hPrevInstance,
    [[maybe_unused]] _In_ LPWSTR lpCmdLine,
    _In_ int nCmdShow
)
{
    try 
    {
        INITCOMMONCONTROLSEX icc = { 
            sizeof(INITCOMMONCONTROLSEX),
            ICC_STANDARD_CLASSES | ICC_WIN95_CLASSES
        };
        InitCommonControlsEx(&icc);

        if (!ui::MainWindow::RegisterMWClass(hInstance)) 
            throw std::runtime_error("Failed to register main window class");
        
        
        if (!ui::MainWindow::CreateAndShow(hInstance, nCmdShow)) 
            throw std::runtime_error("Failed to create main window");

        MSG msg = {};
        while (GetMessage(&msg, nullptr, 0, 0)) 
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        return static_cast<int>(msg.wParam);

    } 
    catch (const std::exception& e) 
    {
        MessageBoxA(nullptr, e.what(), "Error", MB_ICONERROR);
        return 1;
    } 
    catch (...) 
    {
        MessageBoxW(nullptr, L"Unknown error occurred", L"Error", MB_ICONERROR);
        return 1;
    }
}

int main() 
{
    return wWinMain(
        GetModuleHandle(nullptr),
        nullptr,
        GetCommandLineW(),
        SW_SHOWDEFAULT
    );
}

