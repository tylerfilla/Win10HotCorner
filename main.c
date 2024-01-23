/*
 * Win10HotCorner
 *
 * This project is released to the public domain per the CC0 1.0 waiver. See the
 * accompanying LICENSE file for details.
 */

#define UNICODE
#define _UNICODE
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>

#pragma comment(linker, "/SUBSYSTEM:WINDOWS")

/** Cooldown period between toggles. */
const ULONGLONG COOLDOWN_TOGGLE = 500;

/** Last time the task view was toggled. */
static ULONGLONG lastToggleTime;

static void ToggleTaskView() {
  // Skip toggle if the cooldown hasn't elapsed yet
  if (GetTickCount64() - lastToggleTime < COOLDOWN_TOGGLE) {
    return;
  }

  // The input events for toggling task view
  // We want to emulate a Win+Tab key chord
  INPUT events[] = {
      {
          .type = INPUT_KEYBOARD,
          .ki.wVk = VK_LWIN, // Press left Windows key
      },
      {
          .type = INPUT_KEYBOARD,
          .ki.wVk = VK_TAB, // Press tab key
      },
      {
          .type = INPUT_KEYBOARD,
          .ki.dwFlags = KEYEVENTF_KEYUP,
          .ki.wVk = VK_TAB, // Release tab key
      },
      {
          .type = INPUT_KEYBOARD,
          .ki.dwFlags = KEYEVENTF_KEYUP,
          .ki.wVk = VK_LWIN, // Release left Windows key
      },
  };

  // Fire off the input events
  SendInput(4, events, sizeof(INPUT));

  // Remember tick time of last toggle
  lastToggleTime = GetTickCount64();
}

static int IsOverHotCorner(const LONG x, const LONG y) {
  // Feel free to rewrite this function with your own logic
  // With a single monitor, negative values appear to translate into needing more activiation force
  // I have a hunch that this will fail with multi-monitor configurations, however
  return x < -15 && y < -15;
}

static LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
  // If the mouse moved
  if (wParam == WM_MOUSEMOVE) {
    // Get coordinates of mouse cursor
    const MSLLHOOKSTRUCT* event = (MSLLHOOKSTRUCT*) lParam;
    const LONG mx = event->pt.x;
    const LONG my = event->pt.y;

    // If the mouse was over the hot corner
    static int mouse_was_hot = -1;

    // If the mouse is over the hot corner
    const int mouse_is_hot = IsOverHotCorner(mx, my);

    // Initialize past state if the program just started up
    if (mouse_was_hot == -1) {
      mouse_was_hot = mouse_is_hot;
    }

    // If the mouse just went over the hot corner
    if (mouse_is_hot && !mouse_was_hot) {
      // Toggle the task view
      ToggleTaskView();
    }

    mouse_was_hot = mouse_is_hot;
  }

  // Call the next hook
  return CallNextHookEx(NULL, nCode, wParam, lParam);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
  // Set up a low-level mouse hook
  const HHOOK hook = SetWindowsHookEx(WH_MOUSE_LL, &LowLevelMouseProc, NULL, 0);

  // The message loop
  MSG msg;
  while (GetMessage(&msg, NULL, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  // Remove the mouse hook
  UnhookWindowsHookEx(hook);

  return 0;
}
