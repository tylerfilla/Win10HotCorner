/*
 *
 * GNOMEishOverview
 * A GNOME 3-like overview function for the homesick on Windows 10
 *
 */

// TODO: Provide some sort of exit functionality (we'll just kill it for now)

#define UNICODE
#define WIN32_LEAN_AND_MEAN

#include <windows.h>

#pragma comment(linker, "/SUBSYSTEM:WINDOWS")

static void HandleMouseMove(LONG x, LONG y)
{
    // Whether cursor is currently over trigger area
    bool is_over = (x < 50) && (y < 50);

    // Whether cursor was previously over trigger area
    static bool was_over = is_over;
/*
    OutputDebugString(L"is: ");
    OutputDebugString(is_over ? L"yes" : L"no");
    OutputDebugString(L", was: ");
    OutputDebugString(was_over ? L"yes" : L"no");
    OutputDebugString(L"\n");
*/
    if (is_over && !was_over)
    {
        OutputDebugString(L"mouse entered trigger area\n");
    }
    else if (!is_over && was_over)
    {
        OutputDebugString(L"mouse left trigger area\n");
    }

    was_over = is_over;
}

LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    auto event = (MSLLHOOKSTRUCT*) lParam;

    switch (wParam)
    {
    case WM_MOUSEMOVE:
        HandleMouseMove(event->pt.x, event->pt.y);
        break;
    default:
        break;
    }

    // Move on to next hook in chain
    return CallNextHookEx(nullptr, nCode, wParam, lParam);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    OutputDebugString(L"GNOMEishOverview 0.1.0\n");

    // Set up low-level mouse hook
    HHOOK hook = SetWindowsHookEx(WH_MOUSE_LL, LowLevelMouseProc, nullptr, 0);

    // Operate a message loop on this thread
    MSG message{};
    while (GetMessage(&message, nullptr, 0, 0))
    {
        DispatchMessage(&message);
    }

    // Remove low-level mouse hook
    UnhookWindowsHookEx(hook);

    return 0;
}
