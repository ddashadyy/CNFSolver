#include <UI/GraphWindow.hpp>

#include <stdexcept>
#include <algorithm>


namespace ui 
{

bool GraphWindow::RegisterGWClass(HINSTANCE hInstance) 
{
    static bool isRegistered = false; 
    if (isRegistered) return true;    

    WNDCLASSEX wc = { sizeof(WNDCLASSEX) };
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
    wc.lpszClassName = WindowClass;

    isRegistered = RegisterClassEx(&wc) != 0;
    return isRegistered;
}

HWND GraphWindow::Create(
    HWND parentWindow, 
    HINSTANCE hInstance, 
    const std::vector<double>& data, 
    const std::wstring& title
) 
{
    if (!RegisterGWClass(hInstance)) 
    {
        DWORD err = GetLastError();
        MessageBoxW(nullptr, L"Failed to register class", L"Error", MB_ICONERROR);
        return nullptr;
    }

    currentGraphData = new utils::GraphData{data, title};
    HWND hWnd = CreateWindowEx(
        0,
        WindowClass,
        title.c_str(),
        WS_OVERLAPPEDWINDOW,
        100, 100, 800, 600,
        parentWindow,  
        nullptr,
        hInstance,
        currentGraphData
    );

    if (!hWnd) 
    {
        delete currentGraphData;
        return nullptr;
    }

    ShowWindow(hWnd, SW_SHOW);
    UpdateWindow(hWnd);
    return hWnd;
}

LRESULT CALLBACK GraphWindow::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{
    if (uMsg == WM_CREATE) 
    {
        LPCREATESTRUCT pCreate = reinterpret_cast<LPCREATESTRUCT>(lParam);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pCreate->lpCreateParams));

        return 0;
    }

    currentGraphData = reinterpret_cast<utils::GraphData*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

    switch (uMsg) 
    {
        case WM_PAINT:
            if (currentGraphData) 
                OnPaint(hWnd, currentGraphData->data_, currentGraphData->title_);
            
            return 0;
            
        case WM_CLOSE:
            DestroyWindow(hWnd);
            return 0;
            
        case WM_DESTROY:
            delete currentGraphData;
            currentGraphData = nullptr;

            Gdiplus::GdiplusShutdown(0);
            return 0;
            
        default:
            return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
}

void GraphWindow::OnPaint(HWND hWnd, const std::vector<double>& data, const std::wstring& title) 
{
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hWnd, &ps);
    
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);
    
    {
        Gdiplus::Graphics graphics(hdc);
        
        RECT rect;
        GetClientRect(hWnd, &rect);
        int width = rect.right - rect.left;
        int height = rect.bottom - rect.top;
        
        const int margin = 50;
        Gdiplus::RectF plotArea(
            margin, margin, 
            width - 2 * margin, 
            height - 2 * margin
        );
        
        Gdiplus::SolidBrush bgBrush(Gdiplus::Color(255, 255, 255));
        graphics.FillRectangle(&bgBrush, 0, 0, width, height);
        
        Gdiplus::Font titleFont(L"Arial", 16);
        Gdiplus::SolidBrush textBrush(Gdiplus::Color(0, 0, 0));
        Gdiplus::StringFormat format;
        format.SetAlignment(Gdiplus::StringAlignmentCenter);
        graphics.DrawString(
            title.c_str(), -1, &titleFont,
            Gdiplus::PointF(width / 2.0f, 10), &format, &textBrush
        );
        
        if (data.empty()) 
        {
            Gdiplus::SolidBrush brush(Gdiplus::Color(255, 0, 0));
            graphics.DrawString(L"No data to display", -1, nullptr, Gdiplus::PointF(10, 10), &brush);
            return;
        }

        else 
        {
            double maxY = *std::max_element(data.begin(), data.end());
            double minY = *std::min_element(data.begin(), data.end());
            if (maxY == minY) maxY = minY + 1.0; 
            
            Gdiplus::Pen axisPen(Gdiplus::Color(0, 0, 0), 2);
            graphics.DrawLine(&axisPen, 
                plotArea.X, plotArea.GetBottom(), 
                plotArea.GetRight(), plotArea.GetBottom());
            graphics.DrawLine(&axisPen, 
                plotArea.X, plotArea.GetBottom(), 
                plotArea.X, plotArea.Y);
            
            
            Gdiplus::Font labelFont(L"Arial", 10);
            
            for (int i = 0; i <= 10; i++) 
            {
                float x = plotArea.X + (i / 10.0f) * plotArea.Width;
                int iteration = static_cast<int>((i / 10.0) * (data.size() - 1));
                
                graphics.DrawLine(&axisPen, x, plotArea.GetBottom(), x, plotArea.GetBottom() + 5);
                
                std::wstring label = std::to_wstring(iteration);
                graphics.DrawString(
                    label.c_str(), -1, &labelFont,
                    Gdiplus::PointF(x - 10, plotArea.GetBottom() + 10),
                    &textBrush
                );
            }
            
            for (int i = 0; i <= 10; i++) 
            {
                float y = plotArea.GetBottom() - (i / 10.0f) * plotArea.Height;
                double value = minY + (i / 10.0) * (maxY - minY);
                
                graphics.DrawLine(&axisPen, plotArea.X - 5, y, plotArea.X, y);
                
                std::wstring label = std::to_wstring(value).substr(0, 4);
                graphics.DrawString(
                    label.c_str(), -1, &labelFont,
                    Gdiplus::PointF(plotArea.X - 40, y - 8),
                    &textBrush
                );
            }
            
            Gdiplus::Pen dataPen(Gdiplus::Color(255, 0, 0), 2);
            for (size_t i = 1; i < data.size(); i++) 
            {
                float x1 = plotArea.X + ((i-1) / static_cast<float>(data.size()-1)) * plotArea.Width;
                float y1 = plotArea.GetBottom() - 
                          static_cast<float>((data[i-1] - minY) / (maxY - minY) * plotArea.Height);
                
                float x2 = plotArea.X + (i / static_cast<float>(data.size()-1)) * plotArea.Width;
                float y2 = plotArea.GetBottom() - 
                          static_cast<float>((data[i] - minY) / (maxY - minY) * plotArea.Height);
                
                graphics.DrawLine(&dataPen, x1, y1, x2, y2);
            }
        }
    }
    
    Gdiplus::GdiplusShutdown(gdiplusToken);
    
    EndPaint(hWnd, &ps);
}

} // namespace ui