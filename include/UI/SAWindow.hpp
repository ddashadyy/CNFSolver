#pragma once


#include <windows.h>
#include <string>

#include <Core/CNF.hpp>
#include <Core/Candidate.hpp>
#include <Algorithms/SimulatedAnnealing.hpp>

namespace ui
{
    
class SAWindow final
{
public:
    static bool RegisterSAClass(HINSTANCE hInstance);
    static HWND Create(HWND parentWindow, HINSTANCE hInstance);

    static constexpr wchar_t* WINDOW_CLASS = L"CNF_Solver_SAWindow";

private:
    static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    enum
    {
        IDC_ITERATIONS_EDIT = 1001,
        IDC_VARIABLES_EDIT,
        IDC_INITIAL_TEMP_EDIT,
        IDC_MINIMAL_TEMP_EDIT,
        IDC_COOLING_RATE_EDIT,
        IDC_COOLING_COMBO,
        IDC_GENERATE_CNF_BTN,
        IDC_GENERATE_CANDIDATE_BTN,
        IDC_LOAD_CNF_BTN,
        IDC_LOAD_CANDIDATE_BTN,
        IDC_RUN_BTN
    };

    static void OnCreate(HWND hWnd);
    static void OnCommand(HWND hWnd, int controlId);
    static void OnDestroy(HWND hWnd);
    
    static void CreateControls(HWND hWnd);
    static void RunAlgorithm(HWND hWnd);

    static bool LoadCNFFromFile(HWND hWnd, const std::wstring& filePath);
    static bool LoadCandidateFromFile(HWND hWnd, const std::wstring& filePath);
    static std::wstring OpenFileDialog(HWND hWnd, const wchar_t* filter);

    static double GetDlgItemDouble(HWND hDlg, int nIDDlgItem, BOOL* lpSuccess = nullptr, BOOL bSigned = TRUE);

    static inline model::CNF* currentCNF = nullptr;
    static inline model::Candidate* currentCandidate = nullptr;
    static inline algorithm::SimulatedAnnealing* currentAlgorithm = nullptr;
};

} // namespace ui
