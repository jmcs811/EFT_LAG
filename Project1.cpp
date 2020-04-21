#include <windows.h> 
#include <string>
#include <thread>
#include <tchar.h>
#include <random>
#include <iostream>
#include <strsafe.h>
#include "Project1.h"
#include "resource1.h"
#include "Resource.h"

//#pragma comment(lib, "winmm.lib")
//#pragma comment(lib, "User32.lib")

// looks nice, ok...
#define sleep(_time) (std::this_thread::sleep_for(std::chrono::milliseconds(_time))) 

// windows firewall rule name and game executable path
std::string rule_name = "Chrome Test";

TCHAR currentLagTimeLabel[30] = _T("Current Lag Time (ms): ");

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

unsigned __stdcall lagLoop(void *data) {
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
    return 0;
}

// HOOK STUFF TO LISTEN FOR KEY PRESSES WHEN WINDOW NOT IN FOCUS
LRESULT __stdcall HookCallBack(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode >= 0) {
        if (wParam == WM_MBUTTONDOWN) {
            HANDLE hThread;
            hThread = (HANDLE) _beginthreadex(NULL, 0, lagLoop, NULL, 0, NULL);
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
        HDC hdc = GetDC(hwnd);

        // CREATE MENU
        HMENU hMenu, hSubMenu;

        if (!(hMenu = CreateMenu()))
            MessageBox(NULL, "FAILED TO LOAD MENU", "ERROR", MB_ICONERROR);

        if (!(hSubMenu = CreatePopupMenu()))
            MessageBox(NULL, "FAILED TO LOAD POPUPMENU", "ERROR", MB_ICONERROR);

        AppendMenu(hSubMenu, MF_STRING, 5, "&About");

        // Convert input to string and append it to current lag time
        if (_itoa_s(lagTime, lagTimeStr, 10) != 0)
            MessageBox(NULL, "FAILED TO CONVERT INPUT", "ERROR", MB_ICONERROR);

        if (StringCchCatA(currentLagTimeLabel, 30, lagTimeStr) != S_OK)
            MessageBox(NULL, "FAILED TO APPEND STRINGS", "ERROR", MB_ICONERROR);

        Label = CreateWindowEx(WS_EX_CONTEXTHELP, "STATIC", currentLagTimeLabel, WS_CHILD | WS_VISIBLE , 50, 10, 200, 25, hwnd, NULL, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
        TextBox = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD | WS_VISIBLE | WS_BORDER, 50, 35, 120, 20, hwnd, (HMENU)1, NULL, NULL);
        hBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP1));
        SaveButton = CreateWindowEx(0, "BUTTON", NULL, WS_VISIBLE | WS_CHILD | BS_BITMAP, 175, 35, 60, 20, hwnd, (HMENU)3, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
        SendMessage(SaveButton, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hBitmap);
        break;
    }
    case WM_COMMAND:
        // HANDLE SAVE BUTTON
        if (LOWORD(wParam) == 3) {
            TCHAR buff[1024];
            GetWindowText(TextBox, buff, 1024);
            lagTime = atoi(buff);
            if (_itoa_s(lagTime, lagTimeStr, 10) != 0)
                MessageBox(NULL, "FAILED TO CONVERT INPUT", "ERROR", MB_ICONERROR);

            memset(currentLagTimeLabel + 23, NULL, 7);

            if (StringCchCatA(currentLagTimeLabel, 30, lagTimeStr) != S_OK)
                MessageBox(NULL, "FAILED TO APPEND STRINGS", "ERROR", MB_ICONERROR);

            if (SetWindowTextA(Label, currentLagTimeLabel) == 0)
                MessageBox(NULL, "FAILED TO SET TEXT", "ERROR", MB_ICONERROR);

            if (SetWindowTextA(TextBox, "") == 0)
                MessageBox(NULL, "FAILED TO SET TEXT", "ERROR", MB_ICONERROR);
        }
        break;
    case WM_MENUSELECT:
       // char s[1024];
       // sprintf_s(s, 1024, "wParam: %d", HIWORD(wParam));
        //OutputDebugString(s);
        if (HIWORD(wParam) == 32896) {
            MessageBox(NULL,
                "LAG SWITH MADE BY THE HACK BOIS\n\nUsage:\nMiddle mouse button to lag\n\nInfo:\nReccomend 1500 - 1900 ms\nMust run as Admin",
                "HACK BOIS",
                MB_OK
            );
        }
        break;
    case WM_CTLCOLORSTATIC:
        SetTextColor(GetWindowDC(Label), TRANSPARENT);
        SetBkColor(GetWindowDC(Label), RGB(95, 95, 95));
        return (INT_PTR)CreateSolidBrush(RGB(95, 95, 95));
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
    wc.hbrBackground = CreateSolidBrush(RGB(95, 95, 95));
    wc.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
    wc.lpszClassName = g_szClassName;
    wc.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));
    wc.hIconSm = (HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 16, 16, 0);

    // REGISTER Window
    if (!RegisterClassEx(&wc)) {
        MessageBox(NULL, _T("Window Registration Failed"), _T("ERROR"), MB_ICONEXCLAMATION|MB_OK);
    }

    hwnd = CreateWindowEx(
        0,
        g_szClassName,
        "Chrome Tester",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 300, 125,
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