#include <UI/MainWindow.hpp>

#include <UI/GAWindow.hpp>
#include <UI/SAWindow.hpp>
#include <UI/BHWindow.hpp>

#include <UI/GraphWindow.hpp>

#include <Benchmarks/BenchmarkGA.hpp>
#include <Benchmarks/BenchmarkBH.hpp>
#include <Benchmarks/BenchmarkSA.hpp>

#include <stdexcept>
#include <thread>

namespace ui
{


bool MainWindow::RegisterMWClass(HINSTANCE hInstance) 
{
    WNDCLASSEXW wc = { 
        sizeof(WNDCLASSEXW),
        CS_HREDRAW | CS_VREDRAW,              
        WindowProc,          
        0,                   
        0,                   
        hInstance,          
        nullptr,        
        LoadCursor(nullptr, IDC_ARROW),          
        reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1),                
        nullptr,               
        CLASS_NAME,             
        nullptr    
    };

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
        400, 400,
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

    CreateWindowW(
        L"BUTTON", L"Алгоритм пчелиной колонии",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        100, 190, 200, 50,
        hWnd,
        reinterpret_cast<HMENU>(ID_BUTTON_BH),
        nullptr,
        nullptr
    );

    CreateWindowW(
        L"BUTTON", L"Бенчмарки",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        100, 260, 200, 30,
        hWnd,
        reinterpret_cast<HMENU>(ID_BENCHMARKS),
        nullptr,
        nullptr
    );

    CreateWindowW(
        L"BUTTON", L"Справка",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        100, 330, 200, 30,
        hWnd,
        reinterpret_cast<HMENU>(ID_BUTTON_HELP),
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

    case ID_BUTTON_SA:
    {
        HWND hGAWnd = SAWindow::Create(hWnd, hInstance);
        if (!hGAWnd) 
            MessageBoxW(hWnd, L"Failed to create BH window", L"Error", MB_ICONERROR);
        else 
        {
            ShowWindow(hGAWnd, SW_SHOW);
            UpdateWindow(hGAWnd);
        }
        break;
    }    

    case ID_BUTTON_BH:
    {
        HWND hGAWnd = BHWindow::Create(hWnd, hInstance);
        if (!hGAWnd) 
            MessageBoxW(hWnd, L"Failed to create BH window", L"Error", MB_ICONERROR);
        else 
        {
            ShowWindow(hGAWnd, SW_SHOW);
            UpdateWindow(hGAWnd);
        }
        break;
    }

    case ID_BENCHMARKS:
    {
        MessageBoxW(hWnd, L"Запуск бенчмарков...", L"Бенчмарки", MB_OK | MB_ICONINFORMATION);
        
        auto benchGA = std::make_unique<benchmark::BenchmarkGA>();
        auto resultGA = benchGA->Run();

        HWND hGAResultGraphWnd = GraphWindow::Create(
            hWnd, 
            hInstance,
            resultGA.best_progressive_qualities,
            static_cast<double>(resultGA.duration),
            L"Genetic Algorithm Best Progressive Qualities"
        );

        if (!hGAResultGraphWnd) 
            MessageBoxW(hWnd, L"Failed to create graph window", L"Error", MB_ICONERROR);
        
        auto benchSA = std::make_unique<benchmark::BenchmarkSA>();
        auto resultSA = benchSA->Run();

        HWND hSAResultGraphWnd = GraphWindow::Create(
            hWnd, 
            hInstance,
            resultSA.best_progressive_qualities,
            static_cast<double>(resultSA.duration),
            L"Simulated Annealing Algorithm Best Progressive Qualities"
        );

        if (!hSAResultGraphWnd) 
            MessageBoxW(hWnd, L"Failed to create graph window", L"Error", MB_ICONERROR);
        
        auto benchBH = std::make_unique<benchmark::BenchmarkBH>();
        auto resultBH = benchBH->Run();

        HWND hBHResultGraphWnd = GraphWindow::Create(
            hWnd, 
            hInstance,
            resultBH.best_progressive_qualities,
            static_cast<double>(resultBH.duration),
            L"Bee Hive Algorithm Best Progressive Qualities"
        );

        if (!hBHResultGraphWnd) 
            MessageBoxW(hWnd, L"Failed to create graph window", L"Error", MB_ICONERROR);
            
        MessageBoxW(hWnd, L"Бенчмарки записаны в файлы", L"Бенчмарки", MB_OK | MB_ICONINFORMATION);
                
        break;
    }

    case ID_BUTTON_HELP:
    {
        MessageBoxW(hWnd,
            L"CNF Solver - программа для решения задачи выполнимости КНФ.\n\n"
            L"Функции:\n"
            L"1. Генетический алгоритм - поиск решения с использованием эволюционных методов\n"
            L"2. Алгоритм имитации отжига - поиск решения методом имитации отжига\n"
            L"3. Алгоритм пчелиной колонии - поиск решения с использованием эвристических методов\n\n"
            L"Инструкция:\n\n"
            L"Если хотите использовать случайные данные:\n"
            L"1. Выберите алгоритм\n"
            L"2. Настройте параметры\n"
            L"3. Запустите расчет\n\n"
            L"Если хотите использовать свои данные:\n"
            L"1. Запиши свои данные в соответсвующие файлы и формате\n"
            L"Например, КНФ: (x1 | x2 | x3) & (x2 | x1 | x3) & (x3 | x1 | x2)\n"
            L"Каждый кандидат на решение должен быть записан с новой строки\n"
            L"2. После загрузки данных заполните параметры алгоритма строго в соответсвии ваших КНФ и кандидатов\n"
            L"3. Запустите расчет",
            L"Справка", 
            MB_OK | MB_ICONINFORMATION
        );
        break;
    }

    }
}

void MainWindow::OnDestroy([[maybe_unused]] HWND hWnd) 
{
    PostQuitMessage(0);
}

} // namespace ui
