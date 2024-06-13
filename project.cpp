#include <windows.h>
#include <string>
#include <sstream> // Include for wstringstream
#include <iostream>
#include <conio.h>

using namespace std;

// Function to encrypt the message
wstring encrypt(wstring message, int shift, int& encryptedShift) {
    wstring result = L"";
    for (int i = 0; i < message.length(); ++i) {
        if (iswalpha(message[i])) {
            wchar_t shifted = (iswupper(message[i])) ? L'A' : L'a';
            result += wchar_t((int(message[i] + shift - shifted) % 26) + shifted);
        } else {
            result += message[i];
        }
    }
    encryptedShift = shift;
    return result;
}

// Function to decrypt the message
// Function to decrypt the message
wstring decrypt(wstring message, int shift) {
    wstring result = L"";
    for (int i = 0; i < message.length(); ++i) {
        if (iswalpha(message[i])) {
            wchar_t shifted = (iswupper(message[i])) ? L'A' : L'a';
            int decodedChar = int(message[i] - shift - shifted + 26) % 26;
            if (decodedChar < 0) // Adjusting for negative modulo
                decodedChar += 26;
            result += wchar_t(decodedChar + shifted);
        } else {
            result += message[i];
        }
    }
    return result;
}


// Window procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
    {
        // Create message label
        CreateWindowW(L"Static", L"Enter Message:", WS_VISIBLE | WS_CHILD, 50, 30, 100, 20, hwnd, NULL, NULL, NULL);

        // Create message edit box
        CreateWindowW(L"Edit", NULL, WS_VISIBLE | WS_CHILD | WS_BORDER, 200, 30, 150, 20, hwnd, (HMENU)2, NULL, NULL);

        // Create shift label
        CreateWindowW(L"Static", L"Shift Value:", WS_VISIBLE | WS_CHILD, 50, 60, 100, 20, hwnd, NULL, NULL, NULL);

        // Create shift edit box
        CreateWindowW(L"Edit", NULL, WS_VISIBLE | WS_CHILD | WS_BORDER, 200, 60, 50, 20, hwnd, (HMENU)3, NULL, NULL);

        // Create encrypt button
        CreateWindowW(L"Button", L"Encrypt", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 150, 100, 100, 30, hwnd, (HMENU)4, NULL, NULL);

        // Create decrypt button
        CreateWindowW(L"Button", L"Decrypt", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 270, 100, 100, 30, hwnd, (HMENU)5, NULL, NULL);

        // Create encrypted message label
        CreateWindowW(L"Static", L"Encrypted Message:", WS_VISIBLE | WS_CHILD, 50, 150, 150, 20, hwnd, NULL, NULL, NULL);

        // Create encrypted message display
        CreateWindowW(L"Edit", NULL, WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL, 50, 180, 400, 50, hwnd, (HMENU)6, NULL, NULL);

        // Create decrypted message label
        CreateWindowW(L"Static", L"Decrypted Message:", WS_VISIBLE | WS_CHILD, 50, 250, 150, 20, hwnd, NULL, NULL, NULL);

        // Create decrypted message display
        CreateWindowW(L"Edit", NULL, WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL, 50, 280, 400, 50, hwnd, (HMENU)7, NULL, NULL);
        break;
    }
    case WM_COMMAND:
    {
        if (LOWORD(wParam) == 4) // Encrypt button ID
        {
            // Get the message
            WCHAR message[256];
            GetWindowTextW(GetDlgItem(hwnd, 2), message, 256);

            // Get the shift value
            WCHAR shiftValue[256];
            GetWindowTextW(GetDlgItem(hwnd, 3), shiftValue, 256);
            int shift = _wtoi(shiftValue);

            // Encrypt the message
            int encryptedShift;
            wstring encrypted = encrypt(message, shift, encryptedShift);

            // Display encrypted message
            SetDlgItemTextW(hwnd, 6, encrypted.c_str());
        }
        else if (LOWORD(wParam) == 5) // Decrypt button ID
        {
            // Get the encrypted message
            WCHAR encryptedMessage[256];
            GetWindowTextW(GetDlgItem(hwnd, 6), encryptedMessage, 256);

            // Get the shift value
            WCHAR shiftValue[256];
            GetWindowTextW(GetDlgItem(hwnd, 3), shiftValue, 256);
            int shift = _wtoi(shiftValue);

            // Decrypt the message
            wstring decrypted = decrypt(encryptedMessage, shift);

            // Display decrypted message
            SetDlgItemTextW(hwnd, 7, decrypted.c_str());
        }
        break;
    }
    case WM_DESTROY:
    {
        PostQuitMessage(0);
        break;
    }
    default:
        return DefWindowProcW(hwnd, message, wParam, lParam);
    }
    return 0;
}

// Entry point
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // Register window class
    WNDCLASSW wc = {0};
    wc.lpszClassName = L"MyWindowClass";
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    RegisterClassW(&wc);

    // Create window
    HWND hwnd = CreateWindowW(L"MyWindowClass", L"Encryption/Decryption Tool", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 500, 400, NULL, NULL, hInstance, NULL);

    MSG msg;
    while (GetMessageW(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    return 0;
}
