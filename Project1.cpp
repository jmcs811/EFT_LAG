#include <windows.h> 
#include <string>
#include <thread>
#include <tchar.h>
#include <random>
#include <iostream>

//#pragma comment(lib, "winmm.lib")
//#pragma comment(lib, "User32.lib")

// looks nice, ok...
#define sleep(_time) (std::this_thread::sleep_for(std::chrono::milliseconds(_time))) 

// windows firewall rule name and game executable path
std::string rule_name = "Chrome Test";

// defined keys
CONST INT lag_key = VK_MBUTTON;
INT exit_key = VK_END;
INT fwnamelength = 10;
INT lagTime = 1800;
HHOOK _hook;
KBDLLHOOKSTRUCT kdbStruct;

std::string random_string(size_t length)
{
    auto randchar = []() -> char
    {
        const char charset[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";
        const size_t max_index = (sizeof(charset) - 1);
        return charset[rand() % max_index];
    };
    std::string str(length, 0);
    std::generate_n(str.begin(), length, randchar);
    return str;
}

void lagLoop() {
    // Create a random string for the Firewall name. Prevents the same name being added/removed... Good/Bad??
    std::string mystr = random_string(fwnamelength);

    printf("[Middle Mouse Button Pressed]\n");

    // Do our initial beep for start
    MessageBeep(MB_ICONERROR);

    // Create our netsh rule strings
    std::string inrule = "netsh advfirewall firewall add rule name=" + mystr + " dir=in action=block";
    std::string outrule = "netsh advfirewall firewall add rule name=" + mystr + " dir=out action=block";
    std::string delrule = "netsh advfirewall firewall delete rule name=" + mystr;

    // Add the rules to firewall
    WinExec(inrule.c_str(), SW_HIDE);
    WinExec(outrule.c_str(), SW_HIDE);

    // Sleep the lag time
    Sleep(lagTime);

    // Remove the rules
    WinExec(delrule.c_str(), SW_HIDE);

    // Beep for end of lag
    MessageBeep(MB_ICONERROR);
}

LRESULT __stdcall HookCallBack(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode >= 0) {
        if (wParam == WM_MBUTTONDOWN) {
            lagLoop();
        }
    }
    return CallNextHookEx(_hook, nCode, wParam, lParam);
}

void SetHook() {
    if (!(_hook = SetWindowsHookEx(WH_MOUSE_LL, HookCallBack, NULL, 0))) {
        MessageBox(NULL, "FAILED TO INSTALL HOOK", "ERROR", MB_ICONERROR);
    }
}   

void ReleaseHook() {
    UnhookWindowsHookEx(_hook);
}


const char g_szClassName[] = "windowClass";

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) 
{
    switch (msg)
    {
    case WM_CREATE: {
        HWND TextBox = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD | WS_VISIBLE | WS_BORDER, 10, 120, 390, 20, hwnd, (HMENU)1, NULL, NULL);
        HWND SendButton = CreateWindowEx(WS_EX_CLIENTEDGE, "SEND", "", WS_VISIBLE | WS_CHILD | WS_TABSTOP | BS_DEFPUSHBUTTON, 10, 50, 50, 20, hwnd, NULL, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
        break;
    }
    case WM_CLOSE:
       // DestroyWindow(hwnd);
        break;
    case WM_DESTROY:
        //PostQuitMessage(0);

        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow) 
{
    OutputDebugString("Handle it");
    WNDCLASSEX wc;
    HWND hwnd;
    MSG Msg;

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = 1;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = g_szClassName;
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wc)) {
        MessageBox(NULL, _T("Window Registration Failed"), _T("ERROR"), MB_ICONEXCLAMATION|MB_OK);
    }

    hwnd = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        g_szClassName,
        "Chrome Tester",
        WS_OVERLAPPED,
        CW_USEDEFAULT, CW_USEDEFAULT, 500, 200,
        NULL, NULL, hInstance, NULL);

    if (hwnd == NULL) {
        MessageBox(NULL, _T("WINDOW CREATION FAILED"), _T("ERROR!"),
            MB_ICONEXCLAMATION | MB_OK);
    }

    ShowWindow(GetConsoleWindow(), SW_HIDE);
    FreeConsole();
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);
    SetHook();
    while (GetMessage(&Msg, NULL, 0, 0) > 0) {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
    return Msg.wParam;
}