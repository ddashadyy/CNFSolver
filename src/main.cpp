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
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR lpCmdLine,
    _In_ int nCmdShow
)
{
    (void)hPrevInstance;
    (void)lpCmdLine;

    try 
    {
        INITCOMMONCONTROLSEX icc = { sizeof(INITCOMMONCONTROLSEX) };
        icc.dwICC = ICC_STANDARD_CLASSES | ICC_WIN95_CLASSES;
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

#include <Algorithms/BeeHive.hpp>

int main() 
{
    model::CNF* cnf = new model::CNF(10);
    model::Candidates* candidates = new model::Candidates(10, 10);

    algorithm::BeeHive* bh = new algorithm::BeeHive(*cnf, *candidates);

    const auto result = bh->Execute(
        1000,
        10,
        5,
        5,
        5,
        utils::selection_function::kRandom
    );

    std::cout << "Iterations: " << result.iterations_ 
              << " duration: " << result.duration_ 
              << " solution: " << result.solution_ << '\n';

    std::cout << "best qualities\n";
    for (const auto& bq : result.best_qualities_)
        std::cout << bq << " ";
    std::cout << '\n';

    return wWinMain(
        GetModuleHandle(nullptr),
        nullptr,
        GetCommandLineW(),
        SW_SHOWDEFAULT
    );
}

