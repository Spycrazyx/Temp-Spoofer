#include <windows.h>
#include <gdiplus.h>
#include <vector>
#include <stdexcept>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define _CRT_SECURE_NO_WARNINGS
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#define CURL_STATICLIB

// Function to capture a screenshot and save it as image.png in the specified directory
bool CaptureScreenshot(const char* outputPath) {
    // Get screen dimensions
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    // Create a device context for the entire screen
    HDC hScreenDC = GetDC(NULL); // NULL for entire screen
    HDC hMemoryDC = CreateCompatibleDC(hScreenDC);

    // Create a compatible bitmap
    HBITMAP hBitmap = CreateCompatibleBitmap(hScreenDC, screenWidth, screenHeight);
    HGDIOBJ hOldBitmap = SelectObject(hMemoryDC, hBitmap);

    // Copy screen content to the bitmap
    BitBlt(hMemoryDC, 0, 0, screenWidth, screenHeight, hScreenDC, 0, 0, SRCCOPY);

    // Get bitmap info
    BITMAPINFO bmi = { 0 };
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = screenWidth;
    bmi.bmiHeader.biHeight = -screenHeight; // Negative for top-down
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 24; // RGB
    bmi.bmiHeader.biCompression = BI_RGB;

    // Allocate buffer for pixel data
    std::vector<unsigned char> pixelData(screenWidth * screenHeight * 3);
    if (!GetDIBits(hMemoryDC, hBitmap, 0, screenHeight, pixelData.data(), &bmi, DIB_RGB_COLORS)) {
        throw std::runtime_error("Failed to get bitmap data");
    }

    // Save as PNG using stb_image_write
    int result = stbi_write_png(outputPath, screenWidth, screenHeight, 3, pixelData.data(), screenWidth * 3);
    if (!result) {
        throw std::runtime_error("Failed to save PNG file");
    }

    // Cleanup
    SelectObject(hMemoryDC, hOldBitmap);
    DeleteObject(hBitmap);
    DeleteDC(hMemoryDC);
    ReleaseDC(NULL, hScreenDC);

    return true;
}