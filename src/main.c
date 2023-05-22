#include <windows.h>   // The big one!
#include <commdlg.h>   // For common dialog functions
#include <stdio.h>     // For fopen, fwrite, fclose, etc.
#include "callbacks.h" // I dumped most of the ugly code in here to make main.c look nice and clean!

// Big bad main function!
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // Registering the window class
    const char CLASS_NAME[] = "BloatNote";
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    RegisterClass(&wc);

    // Create the window. Ngl this is straight from the microsoft's api docs lol
    // I'm not that smart - I'd never remember all the params!
    HWND hwnd = CreateWindowEx(
        0,                   // Optional window styles
        CLASS_NAME,          // Window class
        "BloatNote",         // Window text
        WS_OVERLAPPEDWINDOW, // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

        NULL,      // Parent window
        NULL,      // Menu
        hInstance, // Instance handle
        NULL       // Additional application data
    );
    // Okey, back to my code!
    //  cue fuck up here :(
    if (hwnd == NULL)
    {
        return 0;
    } // Hope this never happens!

    // What a horrid little function!
    // How does anyone at microsoft sleep at night :(
    HWND hEdit = CreateWindowEx(
        0, "EDIT", NULL,
        WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL,
        0, 0, 0, 0, hwnd, NULL, hInstance, NULL);

    if (hEdit == NULL)
    {
        return 0;
    } // Same as before

    // Set font for the text input area. Arial on top!!!
    HFONT hFont = CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
                             OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                             DEFAULT_PITCH | FF_DONTCARE, "Arial");

    if (hFont != NULL)
    {
        SendMessage(hEdit, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE, 0));
    }
    // end of probable fuck up!
    // nvm, it worked!

    // Pretty self explanatory don't you think?
    ShowWindow(hwnd, nCmdShow);

    // Run the message loop
    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // This used to be "return msg.wParam;" but that made the compiler angry!
    return (int)msg.wParam;
}
