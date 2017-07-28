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

// Uncomment the line below to enable debug prints
// #define DEBUG

#ifdef DEBUG
#define Log(x) OutputDebugString(x)
#else
#define Log(x)
#endif

#pragma comment(linker, "/SUBSYSTEM:WINDOWS")

static void ToggleOverview()
{
    // Windows 10 has a "Task View" that resembles GNOME 3 Overview
    // This can be toggled using the Win+Tab key combo

    INPUT event {};
    event.type = INPUT_KEYBOARD;

    // Press left Windows key
    event.ki.wVk = VK_LWIN;
    SendInput(1, &event, sizeof(INPUT));

    // Press tab key
    event.ki.wVk = VK_TAB;
    SendInput(1, &event, sizeof(INPUT));

    // Release tab key
    event.ki.dwFlags = KEYEVENTF_KEYUP;
    event.ki.wVk = VK_TAB;
    SendInput(1, &event, sizeof(INPUT));

    // Release left Windows key
    event.ki.dwFlags = KEYEVENTF_KEYUP;
    event.ki.wVk = VK_LWIN;
    SendInput(1, &event, sizeof(INPUT));

    Log(L"Overview toggled\n");
}

static bool IsOverTriggerArea(LONG x, LONG y)
{
    // Require the user to exert some force into the corner of the screen
    // I don't know how this will play with multiple monitors
    return x < -20 && y < -20;
}

static void HandleMouseMove(LONG x, LONG y)
{
    // Whether cursor is currently over trigger area
    bool is_over = IsOverTriggerArea(x, y);

    // Whether cursor was previously over trigger area
    // This initializes to the current state to avoid false enter/leave detection on start
    static bool was_over = is_over;

    Log(L"Cursor moved (");
    Log(is_over ? L"is over trigger" : L"not over trigger");
    Log(is_over == was_over ? L" and " : L" but ");
    Log(was_over ? L"was over trigger" : L"was not over trigger");
    Log(L")\n");

    if (is_over && !was_over)
    {
        Log(L"Cursor entered trigger area\n");

        // Toggle the overview
        ToggleOverview();
    }
    else if (!is_over && was_over)
    {
        Log(L"Cursor left trigger area\n");
    }

    was_over = is_over;
}

LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    auto event = reinterpret_cast<MSLLHOOKSTRUCT*>(lParam);

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
    Log(L"GNOMEishOverview 0.1.0\n");

    // Set up low-level mouse hook
    HHOOK hook = SetWindowsHookEx(WH_MOUSE_LL, LowLevelMouseProc, nullptr, 0);

    // Operate a message loop on this thread
    MSG message {};
    while (GetMessage(&message, nullptr, 0, 0))
    {
        DispatchMessage(&message);
    }

    // Remove low-level mouse hook
    UnhookWindowsHookEx(hook);

    return 0;
}
