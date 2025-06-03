#pragma once


#include <windows.h>
#include <string>

#include <Core/CNF.hpp>
#include <Core/Candidates.hpp>
#include <Algorithms/BeeHive.hpp>

namespace ui
{
    
class BHWindow final 
{
public:
    static bool RegisterBHClass(HINSTANCE hInstance);
    static HWND Create(HWND parentWindow, HINSTANCE hInstance);
    
    static constexpr wchar_t* WINDOW_CLASS = L"CNF_Solver_BHWindow";

private:
    static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    
    enum
    {
        IDC_ITERATIONS_EDIT = 1001,
        IDC_VARIABLES_EDIT,
        IDC_POPULATION_EDIT,
        IDC_SCOUTS_EDIT,
        IDC_FORAGERS_EDIT,
        IDC_ONLOOKERS_EDIT,
        IDC_SELECTION_COMBO,
        IDC_GENERATE_CNF_BTN,
        IDC_GENERATE_CANDIDATES_BTN,
        IDC_LOAD_CNF_BTN,
        IDC_LOAD_CANDIDATES_BTN,
        IDC_RUN_BTN
    };

    static void OnCreate(HWND hWnd);
    static void OnCommand(HWND hWnd, int controlId);
    static void OnDestroy(HWND hWnd);
    
    static void CreateControls(HWND hWnd);
    static void RunAlgorithm(HWND hWnd);

    static bool LoadCNFFromFile(HWND hWnd, const std::wstring& filePath);
    static bool LoadCandidatesFromFile(HWND hWnd, const std::wstring& filePath);
    static std::wstring OpenFileDialog(HWND hWnd, const wchar_t* filter);
    
    static inline model::CNF* currentCNF = nullptr;
    static inline model::Candidates* currentCandidates = nullptr;
    static inline algorithm::BeeHive* currentAlgorithm = nullptr;
};

} // namespace ui