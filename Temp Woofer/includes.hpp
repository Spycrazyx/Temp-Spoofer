#include <string>
#include <stdexcept>
#include <iostream>
#include "protection/httplib.h" // Ensure httplib.h is in the same directory
#include "html.hpp"
#include <filesystem>
#include <windows.h>
#include <thread>
#include <filesystem>
#include <TlHelp32.h>
#include <chrono>
#include <vector>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <cstdio>
#include <algorithm>
#include <fstream>

HWND my_wnd;
HWND game_wnd;

#ifdef _WIN32
#include <cstdlib>
#define OPEN_BROWSER "start"
#else
#include <cstdlib>
#define OPEN_BROWSER "xdg-open" // For Linux
// For macOS, use: #define OPEN_BROWSER "open"
#endif

#define C_PRIMARY   (FOREGROUND_GREEN | FOREGROUND_INTENSITY)
#define C_SECONDARY (FOREGROUND_INTENSITY)
#define C_HIGHLIGHT (FOREGROUND_BLUE | FOREGROUND_INTENSITY)
#define C_TITLE     (FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY)
#define C_ERROR     (FOREGROUND_RED | FOREGROUND_INTENSITY)
#define C_PINK      (FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY)
#define C_YELLOW    (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY)