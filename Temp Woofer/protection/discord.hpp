#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <winhttp.h>
#include <string>
#include <sstream>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <vector>
#include <thread>
#include <atlimage.h>
#include <fstream>
#include "screenshot.hpp"
#include "../auth/skStr.h"  // skCrypt

#pragma comment(lib, "winhttp.lib")

using namespace std;

// Forward declarations
std::string get_current_timestamp();
void send_webhook(const std::string& url, const std::string& json_data);
void send_webhook_curl_fallback(const std::string& json_data);
bool test_internet_connection();
std::string get_user_ip();
std::string get_system_uuid();
std::string get_process_name();
std::string get_discord_username();

namespace discord {

    void send_discord_alert(const std::string& reason, const std::string& details = "") {
        try {
            stringstream json;
            json << "{";
            json << "\"content\": null,";
            json << "\"embeds\": [{";
            json << "\"title\": \"Protection Alert\",";
            json << "\"description\": \"Security violation detected!\",";
            json << "\"color\": 16711680,";
            json << "\"fields\": [";
            json << "{\"name\": \"User IP:\", \"value\": \"" << get_user_ip() << "\", \"inline\": false},";
            json << "{\"name\": \"UUID:\", \"value\": \"" << get_system_uuid() << "\", \"inline\": false},";
            json << "{\"name\": \"Debugging Tool:\", \"value\": \"" << (details.empty() ? reason : details) << "\", \"inline\": false}";
            json << "],";
            json << "\"footer\": {\"text\": \"Hezux Temp Protection System\"},";
            json << "\"timestamp\": \"" << get_current_timestamp() << "\"";
            json << "}]";
            json << "}";

            string json_str = json.str();

            // Webhook URL
            string url = skCrypt("https://discord.com/api/webhooks/1429185359933214824/6B3dc3TYYTP9MGlfjSXPfwSi7OQCZlQQnJHHOI4-d6XEjkaL0d5hrlnz6m-HPvZWXXlY").decrypt();
            send_webhook(url, json_str);

            // Curl fallback
            this_thread::sleep_for(chrono::milliseconds(500));
            send_webhook_curl_fallback(json_str);
        }
        catch (...) {}
    }

}

// Get current timestamp in ISO format
string get_current_timestamp() {
    auto now = chrono::system_clock::now();
    auto time_t_now = chrono::system_clock::to_time_t(now);
    auto ms = chrono::duration_cast<chrono::milliseconds>(now.time_since_epoch()) % 1000;

    stringstream ss;
    struct tm gmt;
    if (gmtime_s(&gmt, &time_t_now) == 0) {
        ss << put_time(&gmt, "%Y-%m-%dT%H:%M:%S");
        ss << "." << setfill('0') << setw(3) << static_cast<int>(ms.count()) << "Z";
    }
    return ss.str();
}

// WinHTTP helper
void send_webhook(const string& url, const string& json_data) {
    HINTERNET hSession = nullptr;
    HINTERNET hConnect = nullptr;
    HINTERNET hRequest = nullptr;

    try {
        wstring wUserAgent = L"HezuxTemp/1.0";
        hSession = WinHttpOpen(wUserAgent.c_str(), WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
        if (!hSession) return;

        DWORD tls = 0x00000800 | 0x00000080 | 0x00000020;
        WinHttpSetOption(hSession, WINHTTP_OPTION_SECURE_PROTOCOLS, &tls, sizeof(tls));

        URL_COMPONENTSW urlComp;
        ZeroMemory(&urlComp, sizeof(urlComp));
        urlComp.dwStructSize = sizeof(urlComp);
        urlComp.dwSchemeLength = -1; urlComp.dwHostNameLength = -1; urlComp.dwUrlPathLength = -1;

        wstring wurl(url.begin(), url.end());
        if (!WinHttpCrackUrl(wurl.c_str(), (DWORD)wurl.length(), 0, &urlComp)) return;

        hConnect = WinHttpConnect(hSession, urlComp.lpszHostName, urlComp.nPort, 0);
        if (!hConnect) return;

        wstring fullPath = urlComp.lpszUrlPath ? urlComp.lpszUrlPath : L"/";
        if (urlComp.lpszExtraInfo && *urlComp.lpszExtraInfo) fullPath += urlComp.lpszExtraInfo;

        hRequest = WinHttpOpenRequest(hConnect, L"POST", fullPath.c_str(), nullptr, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, (urlComp.nScheme == INTERNET_SCHEME_HTTPS) ? WINHTTP_FLAG_SECURE : 0);
        if (!hRequest) return;

        wstring headers = L"Content-Type: application/json\r\n";
        if (!WinHttpAddRequestHeaders(hRequest, headers.c_str(), (DWORD)headers.length(), WINHTTP_ADDREQ_FLAG_ADD)) return;

        if (!WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, (LPVOID)json_data.c_str(), (DWORD)json_data.size(), (DWORD)json_data.size(), 0)) return;
        if (!WinHttpReceiveResponse(hRequest, nullptr)) return;
    }
    catch (...) {}

    if (hRequest) WinHttpCloseHandle(hRequest);
    if (hConnect) WinHttpCloseHandle(hConnect);
    if (hSession) WinHttpCloseHandle(hSession);
}

// Fallback using system curl
void send_webhook_curl_fallback(const string& json_data) {
    try {
        ofstream temp_file("webhook_temp.json");
        if (temp_file.is_open()) {
            temp_file << json_data;
            temp_file.close();

            string cmd = string("curl -X POST -H \"Content-Type: application/json\" -d @webhook_temp.json \"") +
                skCrypt("https://discord.com/api/webhooks/1429185359933214824/6B3dc3TYYTP9MGlfjSXPfwSi7OQCZlQQnJHHOI4-d6XEjkaL0d5hrlnz6m-HPvZWXXlY").decrypt() +
                "\" >nul 2>&1";
            system(cmd.c_str());

            DeleteFileA("webhook_temp.json");
        }
    }
    catch (...) {}
}

// Internet check
bool test_internet_connection() {
    HINTERNET hSession = WinHttpOpen(L"Test/1.0", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
    if (!hSession) return false;
    HINTERNET hConnect = WinHttpConnect(hSession, L"google.com", INTERNET_DEFAULT_HTTP_PORT, 0);
    bool connected = (hConnect != nullptr);
    if (hConnect) WinHttpCloseHandle(hConnect);
    WinHttpCloseHandle(hSession);
    return connected;
}

// Other helper functions
string get_user_ip() { /* ... implement with skCrypt if needed ... */ return "Unknown"; }
string get_system_uuid() { /* ... */ return "Unknown"; }
string get_process_name() { /* ... */ return "Unknown"; }
string get_discord_username() { /* ... */ return "Unknown"; }

void image() {
    const std::string imagePath = "C:\\Windows\\image.png";

    // Pass const char* to CaptureScreenshot
    CaptureScreenshot(imagePath.c_str());

    // Webhook URL
    const std::string webhook = skCrypt("https://discord.com/api/webhooks/1429185359933214824/6B3dc3TYYTP9MGlfjSXPfwSi7OQCZlQQnJHHOI4-d6XEjkaL0d5hrlnz6m-HPvZWXXlY").decrypt();

    // Curl command
    std::string cmd = std::string("curl -F \"file1=@") + imagePath + "\" \"" + webhook + "\"";

    system(cmd.c_str());

    // Delete file
    std::remove(imagePath.c_str());
}

// Global alias
void send_discord_alert(const string& reason, const string& details = "") {
    discord::send_discord_alert(reason, details);
}
