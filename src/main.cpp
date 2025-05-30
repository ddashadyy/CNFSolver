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


// testing

#include <Algorithms/SimulatedAnnealing.hpp>

int main() 
{
    model::Candidate candidate(10);
    model::CNF cnf(10);

    algorithm::SimulatedAnnealing sa(cnf, candidate);

    const auto result = sa.Execute(
        100,
        100.0f,
        1.0f,
        0.5f,
        utils::cooling_type::kExponential
    );

    std::cout << "temperatures:\n";
    for (const auto& temp : result.temperatures_)
        std::cout << temp << ' ';

    std::cout << "\nenergies:\n";
    for (const auto& energy : result.best_energies_)
        std::cout << energy << ' ';

    std::cout << '\n' << result.iterations_ << " " << result.duration_ << " " << result.solution_ << '\n';

    return wWinMain(
        GetModuleHandle(nullptr),
        nullptr,
        GetCommandLineW(),
        SW_SHOWDEFAULT
    );
}

