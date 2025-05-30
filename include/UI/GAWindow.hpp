#pragma once

#include <windows.h>
#include <string>

#include <Core/CNF.hpp>
#include <Core/Candidates.hpp>
#include <Algorithms/GeneticAlgorithm.hpp>

namespace ui
{
    
class GAWindow 
{
public:
    static bool RegisterGAClass(HINSTANCE hInstance);
    static HWND Create(HWND parentWindow, HINSTANCE hInstance);
    
    static inline const wchar_t* WINDOW_CLASS = L"CNF_Solver_GAWindow";

private:
    static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    
    // Control IDs
    enum {
        IDC_VARIABLES_EDIT = 1001,
        IDC_POPULATION_EDIT,
        IDC_ITERATIONS_EDIT,
        IDC_CROSSOVERS_EDIT,
        IDC_MUTATIONS_EDIT,
        IDC_GENES_EDIT,
        IDC_SELECTION_COMBO,
        IDC_GENERATE_CNF_BTN,
        IDC_GENERATE_CANDIDATES_BTN,
        IDC_LOAD_CNF_BTN,          
        IDC_LOAD_CANDIDATES_BTN,   
        IDC_RUN_BTN
    };

    // Message handlers
    static void OnCreate(HWND hWnd);
    static void OnCommand(HWND hWnd, int controlId);
    static void OnDestroy(HWND hWnd);
    
    // Helper methods
    static void CreateControls(HWND hWnd);
    static void RunAlgorithm(HWND hWnd);

    // File operations
    static bool LoadCNFFromFile(HWND hWnd, const std::wstring& filePath);
    static bool LoadCandidatesFromFile(HWND hWnd, const std::wstring& filePath);
    static std::wstring OpenFileDialog(HWND hWnd, const wchar_t* filter);
    
    // Instance data
    static inline model::CNF* currentCNF = nullptr;
    static inline model::Candidates* currentCandidates = nullptr;
    static inline algorithm::GeneticAlgorithm* currentAlgorithm = nullptr;
};

} // namespace ui