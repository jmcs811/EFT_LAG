#pragma once

#include <string>
#include <thread>
#include <tchar.h>
#include <random>
#include <iostream>
#include <strsafe.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h> 
#include <wininet.h>
#include <wbemidl.h>
#include <tchar.h>
#include <string.h>
#include <intrin.h>
#include "cryptlib.h"
#include "sha.h"
#include "filters.h"
#include "hex.h"

#define sleep(_time) (std::this_thread::sleep_for(std::chrono::milliseconds(_time))) 

// PROTOS
LPCSTR random_string(size_t length);
STRSAFE_LPSTR vkCodeToString(INT vkCode);
DWORD postRequest(CHAR *buffer, CHAR *hwid);
DWORD getKeyFromFile(CHAR *buffer);
DWORD getHwid(CHAR *hwid);