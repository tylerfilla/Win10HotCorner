/*
 *
 * GNOMEishOverview
 * A GNOME 3-like overview function for the homesick on Windows 10
 *
 */

// TODO: Prove some sort of exit functionality (just kill it for now)

#define UNICODE
#define WIN32_LEAN_AND_MEAN

#include <windows.h>

// TODO: Create a real resources thing
#define VERSION_STR L"0.1.0"

LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    // Move on to next hook in chain
    CallNextHookEx(nullptr, nCode, wParam, lParam);
}

int main(int argc, char* argv[])
{
    OutputDebugString(L"GNOMEishOverview " VERSION_STR L"\n");

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
