#pragma once

#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
#include <vector>
#include <string>

#include <Utils/Utils.hpp>

namespace ui 
{

class GraphWindow 
{
public:
    static bool RegisterGWClass(HINSTANCE hInstance);
    static HWND Create(
        HWND parentWindow, 
        HINSTANCE hInstance, 
        const std::vector<double>& data, 
        const std::wstring& title
    );

    static inline const wchar_t* WindowClass = L"CNF_Solver_GraphWindow";

private:
    static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    static void OnPaint(HWND hWnd, const std::vector<double>& data, const std::wstring& title);
    
    static inline utils::GraphData* currentGraphData = nullptr;
};

} // namespace ui