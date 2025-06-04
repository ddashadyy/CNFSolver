#include <UI/BHWindow.hpp>
#include <UI/GraphWindow.hpp>

#include <sstream>
#include <fstream>

#include <tchar.h>
#include <locale.h>
#include <commctrl.h>
#include <commdlg.h>


namespace ui
{

bool BHWindow::RegisterBHClass(HINSTANCE hInstance)
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
        WINDOW_CLASS,             
        nullptr    
    };



    if (!RegisterClassEx(&wc)) 
    {
        DWORD error = GetLastError();
        if (error != ERROR_CLASS_ALREADY_EXISTS) 
        {  
            MessageBox(nullptr, L"Failed to register BHWindow class!", L"Error", MB_ICONERROR);
            return false;
        }
    }
    return true;
}

HWND BHWindow::Create(HWND parentWindow, HINSTANCE hInstance) 
{
    if (!RegisterBHClass(hInstance))
    {
        DWORD err = GetLastError();
        std::wstring errMsg = L"Error code: " + std::to_wstring(err);
        MessageBox(nullptr, errMsg.c_str(), L"Registration Error", MB_ICONERROR);
        return nullptr;
    }

    HWND hGAWnd = CreateWindowEx(
        WS_EX_APPWINDOW, 
        WINDOW_CLASS,
        L"Алгоритм пчелиной колонии",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, 600, 500,
        parentWindow, 
        nullptr, 
        hInstance, 
        nullptr
    );

    if (!hGAWnd)
    {
        DWORD err = GetLastError();
        std::wstring errMsg = L"CreateWindowEx failed, error code: " + std::to_wstring(err);
        MessageBox(nullptr, errMsg.c_str(), L"Window Creation Error", MB_ICONERROR);
        return nullptr;
    }

    ShowWindow(hGAWnd, SW_SHOW);
    UpdateWindow(hGAWnd);

    return hGAWnd;
}


LRESULT CALLBACK BHWindow::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{
    switch (uMsg) 
    {
        case WM_CREATE:
            OnCreate(hWnd);
            return 0;
            
        case WM_COMMAND:
            OnCommand(hWnd, LOWORD(wParam));
            return 0;
            
        case WM_CLOSE:
            DestroyWindow(hWnd);
            return 0;

        case WM_DESTROY:
            OnDestroy(hWnd);
            return 0;
            
        default:
            return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
}

void BHWindow::OnCreate(HWND hWnd) 
{
    CreateControls(hWnd);
}

void BHWindow::CreateControls(HWND hWnd) 
{
    static HWND hSelectionCombo;
    static HFONT hFont;

    hFont = CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                           DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                           CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                           DEFAULT_PITCH | FF_DONTCARE, L"Arial");

    CreateWindowW(L"STATIC", L"Настройки алгоритма пчелиной колонии для задачи выполнимости КНФ",
                    WS_VISIBLE | WS_CHILD,
                    20, 20, 500, 25, hWnd, nullptr, nullptr, nullptr);

    CreateWindowW(L"STATIC", L"Размер функции (переменных):",
                    WS_VISIBLE | WS_CHILD,
                    20, 60, 200, 20, hWnd, nullptr, nullptr, nullptr);
    CreateWindowW(L"EDIT", L"10",
                    WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER,
                    220, 60, 100, 20, hWnd, reinterpret_cast<HMENU>(IDC_VARIABLES_EDIT), nullptr, nullptr);

    CreateWindowW(L"STATIC", L"Количество итераций:",
                    WS_VISIBLE | WS_CHILD,
                    20, 90, 200, 20, hWnd, nullptr, nullptr, nullptr);
    CreateWindowW(L"EDIT", L"1000",
                    WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER,
                    220, 90, 100, 20, hWnd, reinterpret_cast<HMENU>(IDC_ITERATIONS_EDIT), nullptr, nullptr);

    CreateWindowW(L"STATIC", L"Размер популяции:",
                    WS_VISIBLE | WS_CHILD,
                    20, 120, 200, 20, hWnd, nullptr, nullptr, nullptr);
    CreateWindowW(L"EDIT", L"10",
                    WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER,
                    220, 120, 100, 20, hWnd, reinterpret_cast<HMENU>(IDC_POPULATION_EDIT), nullptr, nullptr);
                    
    CreateWindowW(L"STATIC", L"Количество разведчиков:",
                    WS_VISIBLE | WS_CHILD,
                    20, 150, 200, 20, hWnd, nullptr, nullptr, nullptr);
    CreateWindowW(L"EDIT", L"5",
                    WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER,
                    220, 150, 100, 20, hWnd, reinterpret_cast<HMENU>(IDC_SCOUTS_EDIT), nullptr, nullptr);

    CreateWindowW(L"STATIC", L"Количество фуражиров:",
                    WS_VISIBLE | WS_CHILD,
                    20, 180, 200, 20, hWnd, nullptr, nullptr, nullptr);
    CreateWindowW(L"EDIT", L"5",
                    WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER,
                    220, 180, 100, 20, hWnd, reinterpret_cast<HMENU>(IDC_FORAGERS_EDIT), nullptr, nullptr);

    CreateWindowW(L"STATIC", L"Количество наблюдателей:",
                    WS_VISIBLE | WS_CHILD,
                    20, 210, 200, 20, hWnd, nullptr, nullptr, nullptr);
    CreateWindowW(L"EDIT", L"5",
                    WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER,
                    220, 210, 100, 20, hWnd, reinterpret_cast<HMENU>(IDC_ONLOOKERS_EDIT), nullptr, nullptr);

    CreateWindowW(L"STATIC", L"Функция выбора:",
                    WS_VISIBLE | WS_CHILD,
                    20, 240, 200, 20, hWnd, nullptr, nullptr, nullptr);
    hSelectionCombo = CreateWindowW(L"COMBOBOX", nullptr,
                    WS_VISIBLE | WS_CHILD | WS_BORDER | CBS_DROPDOWNLIST,
                    220, 240, 200, 100, hWnd, reinterpret_cast<HMENU>(IDC_SELECTION_COMBO), nullptr, nullptr);

    SendMessageW(hSelectionCombo, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(L"Случайная"));
    SendMessageW(hSelectionCombo, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(L"Линейная"));
    SendMessageW(hSelectionCombo, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(L"Экспоненциальная"));
    SendMessageW(hSelectionCombo, CB_SETCURSEL, 0, 0);

    CreateWindowW(L"BUTTON", L"Сгенерировать КНФ",
                    WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                    20, 280, 180, 30, hWnd, reinterpret_cast<HMENU>(IDC_GENERATE_CNF_BTN), nullptr, nullptr);

    CreateWindowW(L"BUTTON", L"Сгенерировать кандидатов",
                    WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                    220, 280, 180, 30, hWnd, reinterpret_cast<HMENU>(IDC_GENERATE_CANDIDATES_BTN), nullptr, nullptr);

    CreateWindowW(L"BUTTON", L"Загрузить КНФ из файла",
                    WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                    20, 320, 180, 30, hWnd, reinterpret_cast<HMENU>(IDC_LOAD_CNF_BTN), nullptr, nullptr);

    CreateWindowW(L"BUTTON", L"Загрузить кандидатов из файла",
                    WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                    220, 320, 180, 30, hWnd, reinterpret_cast<HMENU>(IDC_LOAD_CANDIDATES_BTN), nullptr, nullptr);

    CreateWindowW(L"BUTTON", L"Выполнить алгоритм",
                    WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                    20, 360, 380, 30, hWnd, reinterpret_cast<HMENU>(IDC_RUN_BTN), nullptr, nullptr);

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

void BHWindow::OnCommand(HWND hWnd, int controlId) 
{
    switch (controlId) 
    {
        case IDC_GENERATE_CNF_BTN: 
        {
            const std::uint32_t kVarCount = GetDlgItemInt(hWnd, IDC_VARIABLES_EDIT, nullptr, FALSE);

            if (currentCNF != nullptr) 
            {
                delete currentCNF;
                currentCNF = nullptr;
            }
            
            currentCNF = new model::CNF(kVarCount);

            MessageBoxW(hWnd, L"CNF generated successfully", L"Success", MB_OK);
            break;
        }
        
        case IDC_GENERATE_CANDIDATES_BTN: 
        {
            const std::uint32_t kPopSize = GetDlgItemInt(hWnd, IDC_POPULATION_EDIT, nullptr, FALSE);
            const std::uint32_t kVarCount = GetDlgItemInt(hWnd, IDC_VARIABLES_EDIT, nullptr, FALSE);

            if (currentCandidates != nullptr) 
            {
                delete currentCandidates;
                currentCandidates = nullptr;
            }

            currentCandidates = new model::Candidates(kPopSize, kVarCount);

            MessageBoxW(hWnd, L"Candidates generated successfully", L"Success", MB_OK);
            break;
        }

        case IDC_LOAD_CNF_BTN: 
        {
            std::wstring filePath = OpenFileDialog(hWnd, L"Text document\0*.txt\0All Files\0*.*\0");
            if (!filePath.empty()) 
                LoadCNFFromFile(hWnd, filePath);
            
            break;
        }

        case IDC_LOAD_CANDIDATES_BTN: 
        {
            std::wstring filePath = OpenFileDialog(hWnd, L"Text document\0*.txt\0All Files\0*.*\0");
            if (!filePath.empty()) 
                LoadCandidatesFromFile(hWnd, filePath);
            
            break;
        }
        
        case IDC_RUN_BTN:
            RunAlgorithm(hWnd);
            break;
    }
}

void BHWindow::RunAlgorithm(HWND hWnd) 
{
    if (!currentCNF || !currentCandidates) 
    {
        MessageBoxW(hWnd, L"Please generate CNF and candidates first", L"Error", MB_ICONERROR);
        return;
    }

    try 
    {
        const std::uint32_t kIterations = GetDlgItemInt(hWnd, IDC_ITERATIONS_EDIT, nullptr, FALSE);
        const std::uint32_t kPopulation = GetDlgItemInt(hWnd, IDC_POPULATION_EDIT, nullptr, FALSE);
        const std::uint32_t kScouts = GetDlgItemInt(hWnd, IDC_SCOUTS_EDIT, nullptr, FALSE);
        const std::uint32_t kForagers = GetDlgItemInt(hWnd, IDC_FORAGERS_EDIT, nullptr, FALSE);
        const std::uint32_t kOnLookers = GetDlgItemInt(hWnd, IDC_ONLOOKERS_EDIT, nullptr, FALSE);
        
        HWND hCombo = GetDlgItem(hWnd, IDC_SELECTION_COMBO);
        int selectionMethod = SendMessage(hCombo, CB_GETCURSEL, 0, 0);
        
        currentAlgorithm = new algorithm::BeeHive(*currentCNF, *currentCandidates);
        
        const auto result = currentAlgorithm->Execute(
            kIterations, 
            kPopulation, 
            kScouts, 
            kForagers, 
            kOnLookers, 
            static_cast<utils::selection_function>(selectionMethod)
        );
        
        std::wstring message = L"Algorithm completed!\n";
        message += L"Iterations: " + std::to_wstring(result.iterations) + L"\n";
        message += L"Solution: " + std::wstring(result.solution.begin(), result.solution.end()) + L"\n";
        message += L"Execution time in milliseconds: " + std::to_wstring(result.duration);
        
        MessageBoxW(hWnd, message.c_str(), L"Results", MB_OK);


        HINSTANCE hInstance = reinterpret_cast<HINSTANCE>(GetWindowLongPtr(hWnd, GWLP_HINSTANCE));
        if (!hInstance) 
        {
            MessageBoxW(hWnd, L"Invalid hInstance", L"Error", MB_ICONERROR);
            return;
        }

        HWND hGraphWnd = GraphWindow::Create(
            hWnd, 
            hInstance,
            result.best_qualities,
            L"Bee Hive Algorithm Quality Progress"
        );

        if (!hGraphWnd) 
            MessageBoxW(hWnd, L"Failed to create graph window", L"Error", MB_ICONERROR);
    
    } 

    catch (const std::exception& e) 
    {
        MessageBoxA(hWnd, e.what(), "Error", MB_ICONERROR);
    }
}

void BHWindow::OnDestroy(HWND hWnd) 
{
    delete currentCNF;
    currentCNF = nullptr;
    
    delete currentCandidates;
    currentCandidates = nullptr;

    delete currentAlgorithm;
    currentAlgorithm = nullptr;

    DestroyWindow(hWnd);
}

bool BHWindow::LoadCNFFromFile(HWND hWnd, const std::wstring& filePath) 
{
    try 
    {
        std::ifstream file(filePath.c_str());
        if (!file.is_open()) 
        {
            MessageBoxW(hWnd, L"Не удалось открыть файл", L"Ошибка", MB_ICONERROR);
            return false;
        }

        if (currentCNF != nullptr) 
        {
            delete currentCNF;
            currentCNF = nullptr;
        }

        std::string fileCNF{};
        std::getline(file, fileCNF);
        currentCNF = new model::CNF(std::move(fileCNF));
        
        file.close();

        MessageBoxW(hWnd, L"КНФ успешно загружена", L"Успех", MB_OK);
        return true;
    }
    catch (const std::exception& e) 
    {
        MessageBoxA(hWnd, e.what(), "Ошибка", MB_ICONERROR);
        return false;
    }
}

bool BHWindow::LoadCandidatesFromFile(HWND hWnd, const std::wstring& filePath) 
{
    try 
    {
        std::ifstream file(filePath.c_str());
        if (!file.is_open()) 
        {
            MessageBoxW(hWnd, L"Не удалось открыть файл", L"Ошибка", MB_ICONERROR);
            return false;
        }

        if (currentCandidates != nullptr) 
        {
            delete currentCandidates;
            currentCandidates = nullptr;
        }

        std::vector<model::Candidate> fileCandidates{};
        std::stringstream buffer;

        buffer << file.rdbuf();
        std::string line;

        while (std::getline(buffer, line)) 
        {
            fileCandidates.emplace_back(line);
        }

        currentCandidates = new model::Candidates(std::move(fileCandidates));
        
        file.close();

        MessageBoxW(hWnd, L"Кандидаты успешно загружены", L"Успех", MB_OK);
        return true;
    }
    catch (const std::exception& e) {
        MessageBoxA(hWnd, e.what(), "Ошибка", MB_ICONERROR);
        return false;
    }
}

std::wstring BHWindow::OpenFileDialog(HWND hWnd, const wchar_t* filter) 
{
    OPENFILENAMEW ofn;
    wchar_t fileName[MAX_PATH] = {0};

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFile = fileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrFilter = filter;
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileNameW(&ofn)) 
        return fileName;
    
    return L"";
}


} // namespace ui
