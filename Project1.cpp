#include <windows.h> 
#include <thread>
#include <tchar.h>
#include <random>
#include <iostream>
#include <strsafe.h>
#include <stdio.h>
#include "Project1.h"
#include "Resource.h"
#include "helperlib.h"

#pragma comment(lib, "Winmm.lib")

#define LAG_KEY_TEXT "Set Lag Key"
#define PRESS_ANY_KEY_TEXT "PRESS ANY KEY"

// looks nice, ok...
#define sleep(_time) (std::this_thread::sleep_for(std::chrono::milliseconds(_time))) 

TCHAR currentLagTimeLabel[30] = _T("Current Lag Time (ms): ");
TCHAR currentLagKeyText[50] = _T("Current Lag Key: ");

// defined keys
INT lag_key = VK_MBUTTON;
INT lagTime = 1800;
HHOOK _hook;
KBDLLHOOKSTRUCT kdbStruct;
MSLLHOOKSTRUCT msllStruct;
HBITMAP hBitmap;
char lagTimeStr[6];
INT btnPressed = 0;

// UI ELEMENTS
HWND MainWindow;
HWND Label, TextBox, SaveButton, lagKeyComboBox, lagKeyLabel, currentLagKeyLabel;

unsigned __stdcall lagLoop(void* data) {
    HINSTANCE hInstance = GetModuleHandle(NULL);
    INT fwnamelength = 10;
    // Create a random string for the Firewall name. Prevents the same name being added/removed... Good/Bad??
    std::string mystr = random_string(fwnamelength);

    printf("[Middle Mouse Button Pressed]\n");

    // Do our initial beep for start
    //MessageBeep(MB_ICONERROR);
    PlaySound("SoundName", hInstance, SND_RESOURCE | SND_ASYNC);

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
    PlaySound("SoundName", hInstance, SND_RESOURCE | SND_ASYNC);
    return 0;
}

// HOOK STUFF TO LISTEN FOR KEY PRESSES WHEN WINDOW NOT IN FOCUS
LRESULT __stdcall HookCallBack(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode >= 0) {
        if (wParam == WM_MBUTTONDOWN && lag_key == VK_MBUTTON) {
            HANDLE hThread;
            hThread = (HANDLE)_beginthreadex(NULL, 0, lagLoop, NULL, 0, NULL);
        }
        if (wParam == WM_KEYDOWN) {
            kdbStruct = *((KBDLLHOOKSTRUCT*)lParam);
            if (kdbStruct.vkCode == lag_key) {
                HANDLE hThread;
                hThread = (HANDLE)_beginthreadex(NULL, 0, lagLoop, NULL, 0, NULL);
            }
        }
    }
    return CallNextHookEx(_hook, nCode, wParam, lParam);
}

void SetHook() {
    if (lag_key == 4) {
        if (!(_hook = SetWindowsHookEx(WH_MOUSE_LL, HookCallBack, NULL, 0)))
            MessageBox(NULL, "FAILED TO INSTALL HOOK", "ERROR", MB_ICONERROR);
    }
    else {
        if (!(_hook = SetWindowsHookEx(WH_KEYBOARD_LL, HookCallBack, NULL, 0)))
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
        if (_itoa_s(lag_key, lagTimeStr, 10) != 0)
            MessageBox(NULL, "FAILED TO CONVERT INPUT", "ERROR", MB_ICONERROR);
        
       if (StringCchCatA(currentLagKeyText, 50, vkCodeToString(lag_key)) != S_OK)
            MessageBox(NULL, "FAILED TO APPEND STRINGS", "ERROR", MB_ICONERROR);

        // HOT KEY AREA
        lagKeyLabel = CreateWindowEx(WS_EX_CONTEXTHELP, "BUTTON", LAG_KEY_TEXT, WS_CHILD | WS_VISIBLE, 50, 10, 180, 20, hwnd, (HMENU)18, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
        currentLagKeyLabel = CreateWindowEx(0, "STATIC", currentLagKeyText, WS_CHILD | WS_VISIBLE, 50, 33, 200, 25, hwnd, NULL, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
        
        // Convert input to string and append it to current lag time
        if (_itoa_s(lagTime, lagTimeStr, 10) != 0)
            MessageBox(NULL, "FAILED TO CONVERT INPUT", "ERROR", MB_ICONERROR);

        if (StringCchCatA(currentLagTimeLabel, 30, lagTimeStr) != S_OK)
            MessageBox(NULL, "FAILED TO APPEND STRINGS", "ERROR", MB_ICONERROR);

        // LAG TIME AREA
        Label = CreateWindowEx(WS_EX_CONTEXTHELP, "STATIC", currentLagTimeLabel, WS_CHILD | WS_VISIBLE , 50, 70, 200, 25, hwnd, NULL, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
        TextBox = CreateWindowEx(0, "EDIT", "", WS_CHILD | WS_VISIBLE | WS_BORDER, 50, 90, 120, 20, hwnd, (HMENU)1, NULL, NULL);
        SaveButton = CreateWindowEx(0, "BUTTON", NULL, WS_VISIBLE | WS_CHILD | BS_BITMAP, 175, 90, 60, 20, hwnd, (HMENU)3, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);

        // LOADING BITMAPS FOR BUTTONS
        hBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP1));
        SendMessage(SaveButton, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hBitmap);
        break;
    }
    case WM_COMMAND:
        // HANDLE SAVE BUTTON
        switch (LOWORD(wParam))
        {
        case (3):
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
            break;
        case (18):
            SetWindowText(lagKeyLabel, PRESS_ANY_KEY_TEXT);
            btnPressed = 1;
            SetFocus(MainWindow);
            break;
        case (23):
            memset(currentLagKeyText + 17, NULL, 33);

            if (_itoa_s(lag_key, lagTimeStr, 10) != 0)
                MessageBox(NULL, "FAILED TO CONVERT INPUT", "ERROR", MB_ICONERROR);

            if (StringCchCatA(currentLagKeyText, 50, vkCodeToString(lag_key)) != S_OK)
                MessageBox(NULL, "FAILED TO APPEND STRINGS", "ERROR", MB_ICONERROR);

            SetWindowText(currentLagKeyLabel, currentLagKeyText);
        default:
            break;
        }
    case WM_KEYDOWN:
        if (btnPressed == 1) {
            if (wParam == 18)
                break;
 
            // If prev key was mouse, reset hook to listen for kb events
            if (lag_key == VK_MBUTTON)
                lag_key = wParam;                       
                SetHook();
            lag_key = wParam;
            memset(currentLagKeyText + 17, NULL, 33);

            if (_itoa_s(lag_key, lagTimeStr, 10) != 0)
                MessageBox(NULL, "FAILED TO CONVERT INPUT", "ERROR", MB_ICONERROR);

            if (StringCchCatA(currentLagKeyText, 50, vkCodeToString(lag_key)) != S_OK)
                MessageBox(NULL, "FAILED TO APPEND STRINGS", "ERROR", MB_ICONERROR);

            SetWindowText(currentLagKeyLabel, currentLagKeyText);
            btnPressed = 0;
        }
        SetWindowText(lagKeyLabel, LAG_KEY_TEXT);
        break;
    case WM_MBUTTONDOWN:
        OutputDebugString("WM_MBUTTONDOWN\n");
        if (btnPressed == 1) {
            if (wParam == 18)
                break;

            // if prev key wasnt mouse, reset hook to listen for mouse events
            if (lag_key != VK_MBUTTON)
                lag_key = wParam;
                SetHook();
            lag_key = VK_MBUTTON;
            memset(currentLagKeyText + 17, NULL, 33);

            if (_itoa_s(lag_key, lagTimeStr, 10) != 0)
                MessageBox(NULL, "FAILED TO CONVERT INPUT", "ERROR", MB_ICONERROR);

            if (StringCchCatA(currentLagKeyText, 50, vkCodeToString(lag_key)) != S_OK)
                MessageBox(NULL, "FAILED TO APPEND STRINGS", "ERROR", MB_ICONERROR);

            SetWindowText(currentLagKeyLabel, currentLagKeyText);
            btnPressed = 0;
        }
        SetWindowText(lagKeyLabel, LAG_KEY_TEXT);
        break;
    case WM_MENUSELECT:
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
    CHAR hwid[1024];
    CHAR key[37];
    INT result;

    result = getHwid(hwid);
    if (result == 0) {
        MessageBox(NULL, "ERROR GETTING INFO 1", "ERROR", MB_ICONERROR);
        return 1;
    }

    result = getKeyFromFile(key);
    if (result == 1) {
        MessageBox(NULL, "ERROR READING KEY\nplace key.txt is same dir as laggr.exe", "ERROR", MB_ICONERROR);
        return 1;
    }
    
    result = postRequest(key, hwid);
    if (result == 1) {
        MessageBox(NULL, "INVALID KEY. TRY AGAIN", "ERROR", MB_ICONERROR);
        return 1;
    }
    else if (result == -1) {
        MessageBox(NULL, "INVALID HWID. TRY AGAIN", "ERROR", MB_ICONERROR);
        return 1;
    }
    else if (result == -2) {
        MessageBox(NULL, "UNKNOWN ERROR\nCheck key file and try again", "ERROR", MB_ICONERROR);
        return 1;
    }

    // WIN MAIN - ENTRY POINT INTO THE PROGRAM
    WNDCLASSEX wc;
    MSG Msg;
    LPCSTR mystr = random_string(10);

    // SETTING WINDOW PARAMS
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = 1;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, MAKEINTRESOURCE(IDI_ICON1));
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

    MainWindow = CreateWindowEx(
        0,
        g_szClassName,
        mystr,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 300, 175,
        NULL, NULL, hInstance, NULL);

    if (MainWindow == NULL) {
        MessageBox(NULL, _T("WINDOW CREATION FAILED"), _T("ERROR!"),
            MB_ICONEXCLAMATION | MB_OK);
    }

    ShowWindow(MainWindow, nCmdShow);
    UpdateWindow(MainWindow);

    // SETTING HOOK TO LISTEN FOR MBUTTON PRESS
    SetHook();
    while (GetMessage(&Msg, NULL, 0, 0) > 0) {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
    return Msg.wParam;
} 