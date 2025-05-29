#include <UI/GAWindow.hpp>
// #include "UI/GraphWindow.hpp"

#include <Core/CNF.hpp>
#include <Core/Candidates.hpp>

#include <tchar.h>
#include <locale.h>

#include <commctrl.h>
#include <stdexcept>
#include <string>

namespace ui
{
    
bool GAWindow::RegisterGAClass(HINSTANCE hInstance) 
{
    WNDCLASSEX wc = { sizeof(WNDCLASSEX) };
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = WINDOW_CLASS;

    return RegisterClassEx(&wc) != 0;
}

HWND GAWindow::Create(HWND parentWindow, HINSTANCE hInstance) 
{
    if (!RegisterGAClass(hInstance))
    {
        // Более информативное сообщение об ошибке
        DWORD err = GetLastError();
        std::wstring errMsg = L"Error code: " + std::to_wstring(err);
        MessageBox(NULL, errMsg.c_str(), L"Registration Error", MB_ICONERROR);
        return NULL;
    }

    HWND hGAWnd = CreateWindowEx(
        WS_EX_APPWINDOW, // Делаем окно независимым
        WINDOW_CLASS,
        L"Генетический алгоритм",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, 600, 400,
        parentWindow, 
        NULL, 
        hInstance, 
        NULL
    );

    if (!hGAWnd)
    {
        DWORD err = GetLastError();
        std::wstring errMsg = L"CreateWindowEx failed, error code: " + std::to_wstring(err);
        MessageBox(NULL, errMsg.c_str(), L"Window Creation Error", MB_ICONERROR);
        return NULL;
    }

    ShowWindow(hGAWnd, SW_SHOW);
    UpdateWindow(hGAWnd);

    return hGAWnd;
}

LRESULT CALLBACK GAWindow::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
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
            return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
}

void GAWindow::OnCreate(HWND hWnd) 
{
    CreateControls(hWnd);
}

void GAWindow::CreateControls(HWND hWnd) 
{
    static HWND hSelectionCombo;
    static HFONT hFont;

    hFont = CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                           DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                           CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                           DEFAULT_PITCH | FF_DONTCARE, L"Arial");

    CreateWindowW(L"STATIC", L"Настройки генетического алгоритма для задачи выполнимости КНФ",
                    WS_VISIBLE | WS_CHILD,
                    20, 20, 500, 25, hWnd, NULL, NULL, NULL);

    CreateWindowW(L"STATIC", L"Размер функции (переменных):",
                    WS_VISIBLE | WS_CHILD,
                    20, 60, 200, 20, hWnd, NULL, NULL, NULL);
    CreateWindowW(L"EDIT", L"10",
                    WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER,
                    220, 60, 100, 20, hWnd, (HMENU)IDC_VARIABLES_EDIT, NULL, NULL);

    CreateWindowW(L"STATIC", L"Количество итераций:",
                    WS_VISIBLE | WS_CHILD,
                    20, 90, 200, 20, hWnd, NULL, NULL, NULL);
    CreateWindowW(L"EDIT", L"1000",
                    WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER,
                    220, 90, 100, 20, hWnd, (HMENU)IDC_ITERATIONS_EDIT, NULL, NULL);
    CreateWindowW(L"STATIC", L"Размер популяции:",
                    WS_VISIBLE | WS_CHILD,
                    20, 120, 200, 20, hWnd, NULL, NULL, NULL);
    CreateWindowW(L"EDIT", L"100",
                    WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER,
                    220, 120, 100, 20, hWnd, (HMENU)IDC_POPULATION_EDIT, NULL, NULL);
    CreateWindowW(L"STATIC", L"Количество скрещиваний:",
                    WS_VISIBLE | WS_CHILD,
                    20, 150, 200, 20, hWnd, NULL, NULL, NULL);
    CreateWindowW(L"EDIT", L"50",
                    WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER,
                    220, 150, 100, 20, hWnd, (HMENU)IDC_CROSSOVERS_EDIT, NULL, NULL);
    CreateWindowW(L"STATIC", L"Количество мутаций:",
                    WS_VISIBLE | WS_CHILD,
                    20, 180, 200, 20, hWnd, NULL, NULL, NULL);
    CreateWindowW(L"EDIT", L"20",
                    WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER,
                    220, 180, 100, 20, hWnd, (HMENU)IDC_MUTATIONS_EDIT, NULL, NULL);
    CreateWindowW(L"STATIC", L"Количество генов при мутации:",
                    WS_VISIBLE | WS_CHILD,
                    20, 210, 200, 20, hWnd, NULL, NULL, NULL);
    CreateWindowW(L"EDIT", L"3",
                    WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER,
                    220, 210, 100, 20, hWnd, (HMENU)IDC_GENES_EDIT, NULL, NULL);
    CreateWindowW(L"STATIC", L"Функция выбора:",
                    WS_VISIBLE | WS_CHILD,
                    20, 240, 200, 20, hWnd, NULL, NULL, NULL);
    hSelectionCombo = CreateWindowW(L"COMBOBOX", NULL,
                    WS_VISIBLE | WS_CHILD | WS_BORDER | CBS_DROPDOWNLIST,
                    220, 240, 200, 100, hWnd, (HMENU)IDC_SELECTION_COMBO, NULL, NULL);

    SendMessageW(hSelectionCombo, CB_ADDSTRING, 0, (LPARAM)L"Случайная");
    SendMessageW(hSelectionCombo, CB_ADDSTRING, 0, (LPARAM)L"Линейная");
    SendMessageW(hSelectionCombo, CB_ADDSTRING, 0, (LPARAM)L"Экспоненциальная");
    SendMessageW(hSelectionCombo, CB_SETCURSEL, 0, 0);

    CreateWindowW(L"BUTTON", L"Сгенерировать КНФ",
                    WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                    20, 280, 180, 30, hWnd, (HMENU)IDC_GENERATE_CNF_BTN, NULL, NULL);

    CreateWindowW(L"BUTTON", L"Сгенерировать кандидатов",
                                                WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                                                220, 280, 180, 30, hWnd, (HMENU)IDC_GENERATE_CANDIDATES_BTN, NULL, NULL);

    CreateWindowW(L"BUTTON", L"Выполнить алгоритм",
                    WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                    20, 320, 380, 30, hWnd, (HMENU)IDC_RUN_BTN, NULL, NULL);

    EnumChildWindows(
        hWnd, 
        [](HWND hwnd, LPARAM lParam) -> BOOL
        {
            SendMessage(hwnd, WM_SETFONT, lParam, TRUE);
            return TRUE; 
        }, 
        (LPARAM)hFont
    );
}

void GAWindow::OnCommand(HWND hWnd, int controlId) 
{
    switch (controlId) {
        case IDC_GENERATE_CNF_BTN: 
        {
            int varCount = GetDlgItemInt(hWnd, IDC_VARIABLES_EDIT, nullptr, FALSE);

            if (currentCNF != nullptr) 
            {
                delete currentCNF;
                currentCNF = nullptr;
            }
            
            currentCNF = new model::CNF(varCount);

            MessageBoxW(hWnd, L"CNF generated successfully", L"Success", MB_OK);
            break;
        }
        
        case IDC_GENERATE_CANDIDATES_BTN: 
        {
            int popSize = GetDlgItemInt(hWnd, IDC_POPULATION_EDIT, nullptr, FALSE);
            int varCount = GetDlgItemInt(hWnd, IDC_VARIABLES_EDIT, nullptr, FALSE);

            if (currentCandidates != nullptr) 
            {
                delete currentCandidates;
                currentCandidates = nullptr;
            }

            currentCandidates = new model::Candidates(popSize, varCount);

            MessageBoxW(hWnd, L"Candidates generated successfully", L"Success", MB_OK);
            break;
        }
        
        case IDC_RUN_BTN:
            RunAlgorithm(hWnd);
            break;
    }
}

void GAWindow::RunAlgorithm(HWND hWnd) 
{
    if (!currentCNF || !currentCandidates) 
    {
        MessageBoxW(hWnd, L"Please generate CNF and candidates first", L"Error", MB_ICONERROR);
        return;
    }

    try 
    {
        int iterations = GetDlgItemInt(hWnd, IDC_ITERATIONS_EDIT, nullptr, FALSE);
        int population = GetDlgItemInt(hWnd, IDC_POPULATION_EDIT, nullptr, FALSE);
        int crossovers = GetDlgItemInt(hWnd, IDC_CROSSOVERS_EDIT, nullptr, FALSE);
        int mutations = GetDlgItemInt(hWnd, IDC_MUTATIONS_EDIT, nullptr, FALSE);
        int genesToMutate = GetDlgItemInt(hWnd, IDC_GENES_EDIT, nullptr, FALSE);
        
        HWND hCombo = GetDlgItem(hWnd, IDC_SELECTION_COMBO);
        int selectionMethod = SendMessage(hCombo, CB_GETCURSEL, 0, 0);
        
        currentAlgorithm = new algorithm::GeneticAlgorithm(*currentCNF, *currentCandidates);
        
        auto result = currentAlgorithm->Execute(
            iterations, 
            population, 
            crossovers, 
            mutations, 
            genesToMutate, 
            static_cast<utils::selection_function>(selectionMethod)
        );
        
        // Show results
        std::wstring message = L"Algorithm completed!\n";
        message += L"Iterations: " + std::to_wstring(result.iterations_) + L"\n";
        message += L"Solution: " + std::wstring(result.solution_.begin(), result.solution_.end()) + L"\n";
        message += L"Execution time in milliseconds: " + std::to_wstring(result.duration_);
        
        MessageBoxW(hWnd, message.c_str(), L"Results", MB_OK);
    } 

    catch (const std::exception& e) 
    {
        MessageBoxA(hWnd, e.what(), "Error", MB_ICONERROR);
    }
}

void GAWindow::OnDestroy(HWND hWnd) 
{
    delete currentCNF;
    currentCNF = nullptr;
    
    delete currentCandidates;
    currentCandidates = nullptr;

    delete currentAlgorithm;
    currentAlgorithm = nullptr;
}

} // namespace ui
