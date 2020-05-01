#include "helperlib.h"
#pragma comment(lib, "Wininet.lib")

#define ACCESS_GRANTED "{\"msg\": \"access_granted\"}\n"
#define INVALID_KEY "{\"msg\": \"INVALID KEY\"}\n"
#define INVALID_HWID "{\"msg\": \"INVALID HWID\"}\n"
#define KEY_EXPIRED "{\"msg\": \"KEY EXPIRED\"}\n"
#define IP_ADDR "167.71.152.13"
#define ROUTE "api/verify"
#define PORT 80

STRSAFE_LPSTR vkCodeToString(INT vkCode) {
    switch (vkCode)
    {
    case VK_MBUTTON:
        return (STRSAFE_LPSTR)"M Mouse";
    case VK_ESCAPE:
        return (STRSAFE_LPSTR)"Escape";
    case VK_BACK:
        return (STRSAFE_LPSTR)"Back Spc";
    case VK_TAB:
        return (STRSAFE_LPSTR)"Tab";
    case VK_CLEAR:
        return (STRSAFE_LPSTR)"Clear";
    case VK_RETURN:
        return (STRSAFE_LPSTR)"Enter";
    case VK_SHIFT:
        return (STRSAFE_LPSTR)"Shift-INVLD";
    case VK_CONTROL:
        return (STRSAFE_LPSTR)"CTRL-INVLD";
    case VK_MENU:
        return (STRSAFE_LPSTR)"ALT-INVLD";
    case VK_PAUSE:
        return (STRSAFE_LPSTR)"Pause";
    case VK_CAPITAL:
        return (STRSAFE_LPSTR)"Caps Lk";
    case VK_KANA:
        return (STRSAFE_LPSTR)"Kana";
    case VK_SPACE:
        return (STRSAFE_LPSTR)"Space";
    case VK_PRIOR:
        return (STRSAFE_LPSTR)"Pg up";
    case VK_NEXT:
        return (STRSAFE_LPSTR)"Pg Dwn";
    case VK_END:
        return (STRSAFE_LPSTR)"End";
    case VK_HOME:
        return (STRSAFE_LPSTR)"Home";
    case VK_LEFT:
        return (STRSAFE_LPSTR)"Left ARRW";
    case VK_UP:
        return (STRSAFE_LPSTR)"Up ARRW";
    case VK_RIGHT:
        return (STRSAFE_LPSTR)"Right ARRW";
    case VK_DOWN:
        return (STRSAFE_LPSTR)"Down ARRW";
    case VK_SELECT:
        return (STRSAFE_LPSTR)"Select";
    case VK_PRINT:
        return (STRSAFE_LPSTR)"Print";
    case VK_EXECUTE:
        return (STRSAFE_LPSTR)"Execute";
    case VK_SNAPSHOT:
        return (STRSAFE_LPSTR)"Prnt Scrn";
    case VK_INSERT:
        return (STRSAFE_LPSTR)"Insert";
    case VK_DELETE:
        return (STRSAFE_LPSTR)"Delete";
    case VK_HELP:
        return (STRSAFE_LPSTR)"Help";
    case 0x30:
        return (STRSAFE_LPSTR)"0";
    case 0x31:
        return (STRSAFE_LPSTR)"1";
    case 0x32:
        return (STRSAFE_LPSTR)"2";
    case 0x33:
        return (STRSAFE_LPSTR)"3";
    case 0x34:
        return (STRSAFE_LPSTR)"4";
    case 0x35:
        return (STRSAFE_LPSTR)"5";
    case 0x36:
        return (STRSAFE_LPSTR)"6";
    case 0x37:
        return (STRSAFE_LPSTR)"7";
    case 0x38:
        return (STRSAFE_LPSTR)"8";
    case 0x39:
        return (STRSAFE_LPSTR)"9";
    case 0x41:
        return (STRSAFE_LPSTR)"A";
    case 0x42:
        return (STRSAFE_LPSTR)"B";
    case 0x43:
        return (STRSAFE_LPSTR)"C";
    case 0x44:
        return (STRSAFE_LPSTR)"D";
    case 0x45:
        return (STRSAFE_LPSTR)"E";
    case 0x46:
        return (STRSAFE_LPSTR)"F";
    case 0x47:
        return (STRSAFE_LPSTR)"G";
    case 0x48:
        return (STRSAFE_LPSTR)"H";
    case 0x49:
        return (STRSAFE_LPSTR)"I";
    case 0x4A:
        return (STRSAFE_LPSTR)"J";
    case 0x4B:
        return (STRSAFE_LPSTR)"K";
    case 0x4C:
        return (STRSAFE_LPSTR)"L";
    case 0x4D:
        return (STRSAFE_LPSTR)"M";
    case 0x4E:
        return (STRSAFE_LPSTR)"N";
    case 0x4F:
        return (STRSAFE_LPSTR)"O";
    case 0x50:
        return (STRSAFE_LPSTR)"P";
    case 0x51:
        return (STRSAFE_LPSTR)"Q";
    case 0x52:
        return (STRSAFE_LPSTR)"R";
    case 0x53:
        return (STRSAFE_LPSTR)"S";
    case 0x54:
        return (STRSAFE_LPSTR)"T";
    case 0x55:
        return (STRSAFE_LPSTR)"U";
    case 0x56:
        return (STRSAFE_LPSTR)"V";
    case 0x57:
        return (STRSAFE_LPSTR)"W";
    case 0x58:
        return (STRSAFE_LPSTR)"X";
    case 0x59:
        return (STRSAFE_LPSTR)"Y";
    case 0x5A:
        return (STRSAFE_LPSTR)"Z";
    case VK_LWIN:
        return (STRSAFE_LPSTR)"LEFT WIN";
    case VK_RWIN:
        return (STRSAFE_LPSTR)"RIGHT WIN";
    case VK_APPS:
        return (STRSAFE_LPSTR)"APPS";
    case VK_SLEEP:
        return (STRSAFE_LPSTR)"SLEEP";
    case VK_NUMPAD0:
        return (STRSAFE_LPSTR)"NUMPAD 0";
    case VK_NUMPAD1:
        return (STRSAFE_LPSTR)"NUMPAD 1";
    case VK_NUMPAD2:
        return (STRSAFE_LPSTR)"NUMPAD 2";
    case VK_NUMPAD3:
        return (STRSAFE_LPSTR)"NUMPD 3";
    case VK_NUMPAD4:
        return (STRSAFE_LPSTR)"NUMPAD 4";
    case VK_NUMPAD5:
        return (STRSAFE_LPSTR)"NUMPAD 5";
    case VK_NUMPAD6:
        return (STRSAFE_LPSTR)"NUMPAD 6";
    case VK_NUMPAD7:
        return (STRSAFE_LPSTR)"NUMPAD 7";
    case VK_NUMPAD8:
        return (STRSAFE_LPSTR)"NUMPAD 8";
    case VK_NUMPAD9:
        return (STRSAFE_LPSTR)"NUMPAD 9";
    case VK_MULTIPLY:
        return (STRSAFE_LPSTR)"*";
    case VK_ADD:
        return (STRSAFE_LPSTR)"+";
    case VK_SEPARATOR:
        return (STRSAFE_LPSTR)"SEPRTR";
    case VK_SUBTRACT:
        return (STRSAFE_LPSTR)"-";
    case VK_DECIMAL:
        return (STRSAFE_LPSTR)".";
    case VK_DIVIDE:
        return (STRSAFE_LPSTR)"/";
    case VK_F1:
        return (STRSAFE_LPSTR)"F1";
    case VK_F2:
        return (STRSAFE_LPSTR)"F2";
    case VK_F3:
        return (STRSAFE_LPSTR)"F3";
    case VK_F4:
        return (STRSAFE_LPSTR)"F4";
    case VK_F5:
        return (STRSAFE_LPSTR)"F5";
    case VK_F6:
        return (STRSAFE_LPSTR)"F6";
    case VK_F7:
        return (STRSAFE_LPSTR)"F7";
    case VK_F8:
        return (STRSAFE_LPSTR)"F8";
    case VK_F9:
        return (STRSAFE_LPSTR)"F9";
    case VK_F10:
        return (STRSAFE_LPSTR)"F10";
    case VK_F11:
        return (STRSAFE_LPSTR)"F11";
    case VK_F12:
        return (STRSAFE_LPSTR)"F12";
    case VK_F13:
        return (STRSAFE_LPSTR)"F13";
    case VK_F14:
        return (STRSAFE_LPSTR)"F14";
    case VK_F15:
        return (STRSAFE_LPSTR)"F15";
    case VK_F16:
        return (STRSAFE_LPSTR)"F16";
    case VK_F17:
        return (STRSAFE_LPSTR)"F17";
    case VK_F18:
        return (STRSAFE_LPSTR)"F18";
    case VK_F19:
        return (STRSAFE_LPSTR)"F19";
    case VK_F20:
        return (STRSAFE_LPSTR)"F20";
    case VK_F21:
        return (STRSAFE_LPSTR)"F21";
    case VK_F22:
        return (STRSAFE_LPSTR)"F22";
    case VK_F23:
        return (STRSAFE_LPSTR)"F23";
    case VK_F24:
        return (STRSAFE_LPSTR)"F24";
    case VK_NUMLOCK:
        return (STRSAFE_LPSTR)"NUMLCK";
    case VK_SCROLL:
        return (STRSAFE_LPSTR)"SCRLL LCK";
    case VK_LSHIFT:
        return (STRSAFE_LPSTR)"LFT SHFT";
    case VK_RSHIFT:
        return (STRSAFE_LPSTR)"RGT SHFT";
    case VK_LCONTROL:
        return (STRSAFE_LPSTR)"LFT CTRL";
    case VK_RCONTROL:
        return (STRSAFE_LPSTR)"RGT CTRL";
    case VK_LMENU:
        return (STRSAFE_LPSTR)"LFT MENU";
    case VK_RMENU:
        return (STRSAFE_LPSTR)"RGT MENU";
    case VK_BROWSER_BACK:
        return (STRSAFE_LPSTR)"BACK";
    case VK_BROWSER_FORWARD:
        return (STRSAFE_LPSTR)"FORWARD";
    case VK_BROWSER_REFRESH:
        return (STRSAFE_LPSTR)"REFRESH";
    case VK_BROWSER_STOP:
        return (STRSAFE_LPSTR)"STOP";
    case VK_VOLUME_MUTE:
        return (STRSAFE_LPSTR)"MUTE";
    case VK_VOLUME_DOWN:
        return (STRSAFE_LPSTR)"VOL UP";
    case VK_VOLUME_UP:
        return (STRSAFE_LPSTR)"VOL DWN";
    default:
        return (STRSAFE_LPSTR) "Invalid Key";
    }
}

LPCSTR random_string(size_t length)
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
    return str.c_str();
}

DWORD postRequest(CHAR *key, CHAR *hwid) {
    char headers[] = "Content-Type: application/json\r\n";
    char data[1024] = {};
    snprintf(data, sizeof(data), "{\"secret\":\"4pUAauwutDFvTr9J\",\"key\":\"%s\", \"hwid\": \"%s\"}", key, hwid);
    HINTERNET hSession = InternetOpen(
        "Mozilla/5.0",
        INTERNET_OPEN_TYPE_PRECONFIG,
        NULL,
        NULL,
        0
    );

    HINTERNET hConnect = InternetConnect(
        hSession,
        IP_ADDR,
        PORT,
        0,
        0,
        INTERNET_SERVICE_HTTP,
        0,
        0
    );

    HINTERNET hHttpFile = HttpOpenRequest(
        hConnect,
        "POST",
        ROUTE,
        NULL,
        NULL,
        NULL,
        0,
        0
    );
 
    HttpAddRequestHeaders(hHttpFile, headers, strlen(headers), HTTP_ADDREQ_FLAG_ADD);

    while (!HttpSendRequestA(hHttpFile, NULL, 0, (char*) data, strlen(data) + 1)) {
        INT error = GetLastError();
        char temp[20];
        snprintf(temp, 20, "Error: %ld\n", error);
        OutputDebugString(temp);

        InternetErrorDlg(
            GetDesktopWindow(),
            hHttpFile,
            ERROR_INTERNET_CLIENT_AUTH_CERT_NEEDED,
            FLAGS_ERROR_UI_FILTER_FOR_ERRORS
            | FLAGS_ERROR_UI_FLAGS_GENERATE_DATA
            | FLAGS_ERROR_UI_FLAGS_CHANGE_OPTIONS,
            NULL
        );
        break;
    }

    DWORD dwFileSize;
    dwFileSize = 50;

    char* buffer= {};
    buffer = new char[dwFileSize + 1];
    DWORD dwBytesRead;
    while (true) {
        
        BOOL bRead;

        bRead = InternetReadFile(
            hHttpFile,
            buffer,
            dwFileSize + 1,
            &dwBytesRead
        );

        if (dwBytesRead == 0) break;

        if (!bRead)
            OutputDebugString("Read error");
        else
            buffer[dwBytesRead] = 0;
            OutputDebugString(buffer);
           
    }

    InternetCloseHandle(hHttpFile);
    InternetCloseHandle(hConnect);
    InternetCloseHandle(hSession);
    if (strcmp(buffer, ACCESS_GRANTED) == 0)
        return 0;
    else if (strcmp(buffer, INVALID_KEY) == 0)
        return 1;
    else if (strcmp(buffer, INVALID_HWID) == 0)
        return -1;
    else if (strcmp(buffer, KEY_EXPIRED) == 0)
        return -2;
    else
        return -3;
}

DWORD getKeyFromFile(CHAR *buffer) {
    DWORD bytesRead = 0;
    char currentPath[MAX_PATH];
    DWORD pathLength = GetCurrentDirectory(MAX_PATH, (LPSTR) currentPath);

    // APPEND key.txt to currentname
    strcat_s(currentPath, "\\key.txt");

    HANDLE hFile;
    hFile = CreateFile((LPCSTR) currentPath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
        return 1;

    do {
        if (!ReadFile(hFile, buffer, 36, &bytesRead, NULL));
        OutputDebugString("READ ERROR");
    } while (bytesRead > 0);
    buffer[36] = 0;
    CloseHandle(hFile);
    return bytesRead;
}

DWORD getHwid(CHAR *hwid) {
    // GET VOLUME INFO
    TCHAR volumeName[MAX_PATH + 1] = { 0 };
    TCHAR fileSystemName[MAX_PATH + 1] = { 0 };
    DWORD serialNumber = 0;
    DWORD maxComponentLen = 0;
    DWORD fileSystemFlags = 0;
    if (GetVolumeInformation(
        _T("C:\\"),
        volumeName,
        ARRAYSIZE(volumeName),
        &serialNumber,
        &maxComponentLen,
        &fileSystemFlags,
        fileSystemName,
        ARRAYSIZE(fileSystemName)
    ) == 0) {
        return 0;
    }

    // GET COMPUTER NAME
    TCHAR computerName[MAX_COMPUTERNAME_LENGTH + 1];
    DWORD size = sizeof(computerName) / sizeof(computerName[0]);
    if (GetComputerName(computerName, &size) == 0) {
        return 0;
    }

    // GET CPU INFO
    int cpuinfo[4] = { 0, 0, 0, 0 };
    __cpuid(cpuinfo, 0);
    char16_t hash = 0;
    char16_t* ptr = (char16_t*)(&cpuinfo[0]);
    for (char32_t i = 0; i < 8; i++)
        hash += ptr[i];

    
    snprintf(hwid, 1024, "%s%s%lu%lu%lu%s%lu%d", volumeName, fileSystemName, serialNumber, maxComponentLen, fileSystemFlags, computerName, size, hash);
    CryptoPP::SHA256 sha256;
    std::string digest;
    CryptoPP::StringSource foo(hwid, true, new CryptoPP::HashFilter(sha256, new CryptoPP::HexEncoder(new CryptoPP::StringSink(digest))));
    snprintf(hwid, 1024, "%s", digest.c_str());
    return 1;
}