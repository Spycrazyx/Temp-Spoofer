




// everything is inside
#include "includes.hpp"
#include "protection/prot.h"
#include <Windows.h>
#include "auth/auth.hpp"
#include <string>
#include <thread>
#include "auth/utils.hpp"
#include "auth/skStr.h"
#include <iostream>
#include <algorithm>
#include <sstream>
#include <vector>
#include <random>
#include <ctime>
#include <cstdlib>
#include <cstdio>
#include <functional>
#include <cctype>
#include "protection/httplib.h"


std::string tm_to_readable_time(tm ctx);
static std::time_t string_to_timet(std::string timestamp);
static std::tm timet_to_tm(time_t timestamp);

using namespace KeyAuth;

const std::string compilation_date = std::string(skCrypt(__DATE__));
const std::string compilation_time = std::string(skCrypt(__TIME__));

std::string name = std::string(skCrypt("Hezux Temp")); // App name
std::string ownerid = std::string(skCrypt("jQwtwVHDc9")); // Account ID
std::string version = std::string(skCrypt("1.0")); // Application version
std::string keyauth_url = std::string(skCrypt("https://keyauth.win/api/1.3/")); // KeyAuth URL
std::string path = std::string(skCrypt("")); // optional path

api KeyAuthApp(name, ownerid, version, keyauth_url, path);

void SetConsoleColor(WORD color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void ShowQuestionBox()
{
    // skCrypt(...) converts to char* via operator T*, so build std::string from it for convenience
    std::string q = std::string(skCrypt("Do you want to continue?"));
    std::wstring qw(q.begin(), q.end());
    std::string t = std::string(skCrypt("Confirmation"));
    std::wstring tw(t.begin(), t.end());

    int result = MessageBoxW(
        nullptr,
        qw.c_str(),
        tw.c_str(),
        MB_YESNO | MB_ICONQUESTION
    );

    if (result == IDYES)
    {
        MessageBeep(MB_ICONINFORMATION);
    }
    else if (result == IDNO)
    {
        ExitProcess(0);
    }
}

void speakSpoofMessage() {
    system("powershell -Command \"Add-Type –AssemblyName System.Speech; "
        "$speak = New-Object System.Speech.Synthesis.SpeechSynthesizer; "
        "$speak.Speak('Spoofing Succesfully Compleated');\"");
}

void LoadDriver() {
    Beep(750, 1000);

    // system accepts const char*. skCrypt returns char*, but wrap in std::string for consistency

    Sleep(3000);
    system(std::string(skCrypt("cd C:\\Windows\\ && vredistx64.exe ud.sys >nul 2>&1")).c_str());
    Sleep(1000);
    system(std::string(skCrypt("cd C:\\Windows\\ && vredistx32.exe >nul 2>&1")).c_str());
    Sleep(1000);
    system(std::string(skCrypt("cd C:\\Windows\\ && driverdisable.bat >nul 2>&1")).c_str());

    // Delete the downloaded files
    system(std::string(skCrypt("del C:\\Windows\\ud.sys >nul 2>&1")).c_str());
    system(std::string(skCrypt("del C:\\Windows\\vredistx64.exe >nul 2>&1")).c_str());
    system(std::string(skCrypt("del C:\\Windows\\vredistx32.exe >nul 2>&1")).c_str());
    system(std::string(skCrypt("del C:\\Windows\\driverdisable.bat >nul 2>&1")).c_str());



    system("net stop winmgmt");
    system("net start winmgmt");



    Beep(750, 1000);
    speakSpoofMessage();
}


void ShowSecurityNotice()
{
    std::string message = std::string(skCrypt(
        "Software Usage Agreement & Security Notice\n"
        "This software is protected by intellectual property laws and\n"
        "remains the sole property of its rightful owner.\n"
        "By using this software, you acknowledge and agree to comply\n"
        "with all applicable laws, regulations, and licensing terms.\n\n"
        "Prohibited Activities:\n"
        "- Reverse engineering, disassembly, decompilation, or\n"
        "  attempts to access the underlying code and algorithms.\n"
        "- Modification, tampering, or circumvention of security\n"
        "  measures.\n"
        "- Use in violation of licensing agreements, redistribution\n"
        "  without explicit permission, or unauthorized data extraction.\n\n"
        "Compliance & Monitoring:\n"
        "Usage of this software is continuously monitored to ensure\n"
        "compliance with security policies and intellectual property\n"
        "protections.\n"
        "Violations may result in restrictions, legal consequences, and\n"
        "irreversible data loss.\n\n"
        "Examples of Unauthorized Access:\n"
        "- Reverse Engineering: Attempting to analyze internal\n"
        "  functionality via debugging tools or unauthorized code\n"
        "  inspection.\n"
        "- Tampering: Modifying executable files, injecting\n"
        "  unauthorized code, or altering system behaviors.\n"
        "- Unauthorized Usage: Sharing, redistributing, or executing\n"
        "  the software outside of permitted environments.\n\n"
        "By proceeding, you acknowledge that violations of this\n"
        "agreement may result in immediate termination of access and\n"
        "potential legal action.\n\n"
        "Press 'OK' to continue."
    ));

    std::wstring wmessage(message.begin(), message.end());
    std::string title = std::string(skCrypt("SECURITY NOTICE"));
    std::wstring wtitle(title.begin(), title.end());

    MessageBoxW(NULL, wmessage.c_str(), wtitle.c_str(), MB_OK | MB_ICONWARNING);
}


void Loadshit() {

    // Use HTML content from header file
    const std::string html_content = HTML::CONTENT;

    // Create HTTP server
    httplib::Server svr;

    auto trim = [](std::string s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) { return !std::isspace(ch); }));
        s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) { return !std::isspace(ch); }).base(), s.end());
        return s;
        };

    auto runCommand = [&](const char* cmd) -> std::string {
        std::string result;
        FILE* pipe = _popen(cmd, "r");
        if (!pipe) return result;
        char buffer[4096];
        while (fgets(buffer, sizeof(buffer), pipe)) {
            result += buffer;
        }
        _pclose(pipe);
        return result;
        };

    // Serve the HTML content at root
    svr.Get("/", [&](const httplib::Request&, httplib::Response& res) {
        try {
            res.set_content(html_content, std::string(skCrypt("text/html")));
        }
        catch (const std::exception& e) {
            res.status = 500;
            std::string err = std::string(skCrypt("Error: ")) + e.what();
            res.set_content(err, std::string(skCrypt("text/plain")));
        }
        });


    svr.Get("/load-driver", [&](const httplib::Request&, httplib::Response& res) {
        try {
            std::thread ldThread(LoadDriver); // run in a separate thread to avoid blocking
            ldThread.detach();
            res.set_content("{\"status\":\"Spoofing started\"}", "application/json");
        }
        catch (const std::exception& e) {
            res.status = 500;
            std::string err = std::string("{\"error\":\"") + e.what() + "\"}";
            res.set_content(err, "application/json");
        }
        });

    // Endpoint to collect hardware serials and return JSON
    svr.Get("/serials", [&](const httplib::Request&, httplib::Response& res) {
        try {
            // Disks: model + serial
            std::string disksOut = runCommand(std::string(skCrypt("wmic diskdrive get Model,SerialNumber /value")).c_str());
            std::vector<std::pair<std::string, std::string>> disks;
            {
                std::string currentModel;
                std::string currentSerial;
                std::istringstream iss(disksOut);
                std::string line;
                while (std::getline(iss, line)) {
                    line = trim(line);
                    if (line.rfind(std::string(skCrypt("Model=")), 0) == 0) {
                        currentModel = trim(line.substr(6));
                    }
                    else if (line.rfind(std::string(skCrypt("SerialNumber=")), 0) == 0) {
                        currentSerial = trim(line.substr(13));
                    }
                    if (!currentModel.empty() && !currentSerial.empty()) {
                        disks.emplace_back(currentModel, currentSerial);
                        currentModel.clear();
                        currentSerial.clear();
                    }
                }
            }

            // CPU: use ProcessorId as a reliable identifier
            std::string cpuOut = runCommand(std::string(skCrypt("wmic cpu get ProcessorId /value")).c_str());
            std::string cpuSerial;
            {
                std::istringstream iss(cpuOut);
                std::string line;
                while (std::getline(iss, line)) {
                    line = trim(line);
                    if (line.rfind(std::string(skCrypt("ProcessorId=")), 0) == 0) {
                        cpuSerial = trim(line.substr(12));
                        break;
                    }
                }
            }

            // BIOS Serial
            std::string biosOut = runCommand(std::string(skCrypt("wmic bios get SerialNumber /value")).c_str());
            std::string biosSerial;
            {
                std::istringstream iss(biosOut);
                std::string line;
                while (std::getline(iss, line)) {
                    line = trim(line);
                    if (line.rfind(std::string(skCrypt("SerialNumber=")), 0) == 0) {
                        biosSerial = trim(line.substr(13));
                        break;
                    }
                }
            }

            // Motherboard Serial
            std::string mbOut = runCommand(std::string(skCrypt("wmic baseboard get SerialNumber /value")).c_str());
            std::string mbSerial;
            {
                std::istringstream iss(mbOut);
                std::string line;
                while (std::getline(iss, line)) {
                    line = trim(line);
                    if (line.rfind(std::string(skCrypt("SerialNumber=")), 0) == 0) {
                        mbSerial = trim(line.substr(13));
                        break;
                    }
                }
            }

            // SMBIOS UUID
            std::string uuidOut = runCommand(std::string(skCrypt("wmic csproduct get UUID /value")).c_str());
            std::string smbiosUUID;
            {
                std::istringstream iss(uuidOut);
                std::string line;
                while (std::getline(iss, line)) {
                    line = trim(line);
                    if (line.rfind(std::string(skCrypt("UUID=")), 0) == 0) {
                        smbiosUUID = trim(line.substr(5));
                        break;
                    }
                }
            }

            // Build JSON manually (avoid adding a JSON library dependency)
            std::ostringstream json;
            json << std::string(skCrypt("{\n"));
            json << std::string(skCrypt("  \"disks\": ["));
            for (size_t i = 0; i < disks.size(); ++i) {
                // Escape quotes in model/serial if present
                auto esc = [](const std::string& s) {
                    std::string r; r.reserve(s.size());
                    for (char c : s) { if (c == '\\' || c == '"') r.push_back('\\'); r.push_back(c); }
                    return r;
                    };
                json << std::string(skCrypt("{\"model\":\"")) << esc(disks[i].first) << std::string(skCrypt("\",\"serial\":\"")) << esc(disks[i].second) << std::string(skCrypt("\"}"));
                if (i + 1 < disks.size()) json << std::string(skCrypt(","));
            }
            json << std::string(skCrypt("],\n"));
            json << std::string(skCrypt("  \"cpuSerial\": \"")) << cpuSerial << std::string(skCrypt("\",\n"));
            json << std::string(skCrypt("  \"biosSerial\": \"")) << biosSerial << std::string(skCrypt("\",\n"));
            json << std::string(skCrypt("  \"motherboardSerial\": \"")) << mbSerial << std::string(skCrypt("\",\n"));
            json << std::string(skCrypt("  \"smbiosUUID\": \"")) << smbiosUUID << std::string(skCrypt("\"\n"));
            json << std::string(skCrypt("}\n"));

            res.set_content(json.str(), std::string(skCrypt("application/json")));
        }
        catch (const std::exception& e) {
            res.status = 500;
            std::string err = std::string(skCrypt("{\"error\":\"")) + e.what() + std::string(skCrypt("\"}"));
            res.set_content(err, std::string(skCrypt("application/json")));
        }
        });

    // Start server on localhost:8080
    std::string host = std::string(skCrypt("localhost"));
    int port = 8080;
    std::string localUrl = std::string(skCrypt("http://")) + host + std::string(skCrypt(":")) + std::to_string(port);

    // Open the default browser (OPEN_BROWSER macro assumed to exist)
    std::string command = std::string(OPEN_BROWSER) + std::string(skCrypt(" ")) + localUrl;
    system(command.c_str());

    // Run the server (blocking)
    svr.listen(host.c_str(), port);
}

void slowPrint(const std::string& text, int delayMs = 50) {
    std::cout << std::string(skCrypt("[+] "));
    for (char c : text) {
        std::cout << c << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
    }
    std::cout << std::endl;
}

void slowwPrint(const std::string& text, int delayMs = 50) {
    std::cout << std::string(skCrypt("[+] "));
    for (char c : text) {
        std::cout << c << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
    }
}

void fuckPrint(const std::string& text, int delayMs = 50) {
    // keep empty prefix intentionally (no visible prefix)
    for (char c : text) {
        std::cout << c << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
    }
}

std::string generateRandomString(int length) {
    const std::string chars =
        std::string(skCrypt("abcdefghijklmnopqrstuvwxyz")) +
        std::string(skCrypt("ABCDEFGHIJKLMNOPQRSTUVWXYZ")) +
        std::string(skCrypt("0123456789"));

    std::string result;
    result.reserve(length);

    std::mt19937 rng(static_cast<unsigned int>(time(nullptr))); // seed RNG
    std::uniform_int_distribution<int> dist(0, static_cast<int>(chars.size() - 1));

    for (int i = 0; i < length; ++i) {
        result += chars[dist(rng)];
    }

    return result;
}

void setRandomCmdTitle() {
    std::string randomTitle = generateRandomString(100);
    SetConsoleTitleA(randomTitle.c_str());
}

void resizeCMD(float widthScale = 0.5f, float heightScale = 0.5f) {
    HWND console = GetConsoleWindow();
    if (!console) return;
    RECT r;
    GetWindowRect(console, &r);
    int currentWidth = r.right - r.left;
    int currentHeight = r.bottom - r.top;
    int newWidth = static_cast<int>(currentWidth * widthScale);
    int newHeight = static_cast<int>(currentHeight * heightScale);
    MoveWindow(console, r.left, r.top, newWidth, newHeight, TRUE);
}

void resizeCMDNoScrollbars(int width = 80, int height = 25) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SMALL_RECT windowSize;
    windowSize.Left = 0;
    windowSize.Top = 0;
    windowSize.Right = width - 1;
    windowSize.Bottom = height - 1;
    SetConsoleWindowInfo(hConsole, TRUE, &windowSize);
    COORD bufferSize;
    bufferSize.X = width;
    bufferSize.Y = height;
    SetConsoleScreenBufferSize(hConsole, bufferSize);
}

void addFreeLine() {
    std::cout << std::endl;
}

void disable() {
    HWND hwnd = GetConsoleWindow(); // Get the handle to the console window
    if (!hwnd) return;

    // Get the current window style
    LONG style = GetWindowLong(hwnd, GWL_STYLE);

    // Remove maximize and minimize buttons
    style &= ~WS_MAXIMIZEBOX;
    style &= ~WS_MINIMIZEBOX;

    // Apply the new style
    SetWindowLong(hwnd, GWL_STYLE, style);

    // Update the window to reflect the changes
    SetWindowPos(hwnd, nullptr, 0, 0, 0, 0,
        SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
}


int main() {

    disable();

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    resizeCMD(0.7f, 0.5f);
    resizeCMDNoScrollbars(55, 15);
    setRandomCmdTitle();

    KeyAuthApp.init();
    security::init_protection();

    slowwPrint(std::string(skCrypt("Welcome to")), 40);
    SetConsoleTextAttribute(hConsole, 13);
    fuckPrint(std::string(skCrypt(" Hezux")), 40);
    SetConsoleTextAttribute(hConsole, 7);
    fuckPrint(std::string(skCrypt(" - Hardware Virtualization!")), 40);

    // show security notice
    ShowSecurityNotice();

    addFreeLine();
    addFreeLine();

    slowPrint(std::string(skCrypt("Succesfully Connected to Api Servers")), 4);
    slowwPrint(std::string(skCrypt("Supported System Verions")), 4);
    SetConsoleTextAttribute(hConsole, 8);
    fuckPrint(std::string(skCrypt(" [22h2,23h2,24h2]")), 4);
    SetConsoleTextAttribute(hConsole, 7);
    addFreeLine();
    slowwPrint(std::string(skCrypt("Current Loader Version is")), 4);
    SetConsoleTextAttribute(hConsole, 8);
    fuckPrint(std::string(skCrypt(" 2.0")), 4);
    SetConsoleTextAttribute(hConsole, 7);

    std::string key;
    std::cout << std::string(skCrypt("\n\nPlease Enter Your activation Key: "));
    std::cin >> key;

    KeyAuthApp.license(key);

    system(std::string(skCrypt("cls")).c_str());

    slowwPrint(std::string(skCrypt("Welcome to")), 40);
    SetConsoleTextAttribute(hConsole, 13);
    fuckPrint(std::string(skCrypt(" Hezux")), 40);
    SetConsoleTextAttribute(hConsole, 7);
    fuckPrint(std::string(skCrypt(" - Hardware Virtualization!")), 40);
    addFreeLine();
    addFreeLine();

    slowPrint(std::string(skCrypt("Checking Your license Key.")), 40);
    Sleep(3000);

    if (!KeyAuthApp.response.success)
    {
        addFreeLine();
        slowPrint(std::string(skCrypt("License Key Is invalid.")), 40);
        Sleep(3000);
        return 1;
    }

    addFreeLine();
    slowPrint(std::string(skCrypt("License Key Is Correct")), 40);
    addFreeLine();
    Sleep(1000);
    slowPrint(std::string(skCrypt("Loading Woofer Components Please Wait")), 40);
    Sleep(4000);

    // start any background checks (existing function from your project)
    std::thread run(checkAuthenticated, ownerid);
    run.detach();

    Loadshit();

    return 0;
}
