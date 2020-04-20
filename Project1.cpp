#include <windows.h> 
#include <string>
#include <thread>
#include <tchar.h>
#include <random>
#include <iostream>
#include "Project1.h"
#include "resource1.h"

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
HBITMAP hBitmap;
char lagTimeStr[6];

// UI ELEMENTS
HWND Label, TextBox, SaveButton;

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

// HOOK STUFF TO LISTEN FOR KEY PRESSES WHEN WINDOW NOT IN FOCUS
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
    // HANDLE MESSAGES SUCH AS KEY PRESSES AND ADD GUI ELEMENTS
    switch (msg)
    {
    case WM_CREATE: {
        HINSTANCE hInstance = GetModuleHandle(NULL);

        // Set icon
        HICON hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
        SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);


        _itoa_s(lagTime, lagTimeStr, 10);
        Label = CreateWindowEx(SS_SIMPLE, "STATIC", "Lag Time (ms)", WS_CHILD | WS_VISIBLE, 10, 5, 100, 25, hwnd, NULL, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
        TextBox = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", lagTimeStr, WS_CHILD | WS_VISIBLE | WS_BORDER, 10, 35, 100, 20, hwnd, (HMENU)1, NULL, NULL);
        hBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP1));
        SaveButton = CreateWindowEx(0, "BUTTON", NULL, WS_VISIBLE | WS_CHILD | BS_BITMAP, 110, 35, 60, 20, hwnd, (HMENU)3, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
        SendMessage(SaveButton, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hBitmap);
        break;
    }
    case WM_COMMAND:
        if (LOWORD(wParam) == 3) {
            TCHAR buff[1024];
            GetWindowText(TextBox, buff, 1024);
            lagTime = atoi(buff);
        }
        break;
    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow) 
{
    // WIN MAIN - ENTRY POINT INTO THE PROGRAM
    WNDCLASSEX wc;
    HWND hwnd;
    MSG Msg;

    // SETTING WINDOW PARAMS
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

    // REGISTER Window
    if (!RegisterClassEx(&wc)) {
        MessageBox(NULL, _T("Window Registration Failed"), _T("ERROR"), MB_ICONEXCLAMATION|MB_OK);
    }

    hwnd = CreateWindowEx(
        0,
        g_szClassName,
        "Chrome Tester",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 300, 100,
        NULL, NULL, hInstance, NULL);

    if (hwnd == NULL) {
        MessageBox(NULL, _T("WINDOW CREATION FAILED"), _T("ERROR!"),
            MB_ICONEXCLAMATION | MB_OK);
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // SETTING HOOK TO LISTEN FOR MBUTTON PRESS
    SetHook();
    while (GetMessage(&Msg, NULL, 0, 0) > 0) {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
    return Msg.wParam;
}