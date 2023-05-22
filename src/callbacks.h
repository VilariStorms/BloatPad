// Macros for the hotkeys, nice and simple!
#define ID_HOTKEY 1
#define ID_HOTKEY_SAVE 2
#define ID_HOTKEY_OPEN 3
#define IDM_SAVE 1001

/*
Function definitions!

These tell the magic rock we injected lightning into and tricked into
thinking what these functions will look like when we declare them in the
not so distant future!
*/
BOOL SaveToFile(HWND hwnd, const char *content, DWORD contentLength);
BOOL OpenFileToEdit(HWND hwnd, HWND hEdit);

// Here's out actual callback for the big bad main function!
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    // This is the message that gets sent when the window is created!
    case WM_CREATE:

        // register hotkey for ctrl+ L
        RegisterHotKey(hwnd, ID_HOTKEY, MOD_CONTROL, 0x4C);
        // Register hotkey for ctrl+s
        RegisterHotKey(hwnd, ID_HOTKEY_SAVE, MOD_CONTROL, 0x53);
        // Register hotkey for ctrl+o
        RegisterHotKey(hwnd, ID_HOTKEY_OPEN, MOD_CONTROL, 0x4F);
        return 0;
    case WM_SIZE:
    {
        // Don't worry about this, it's just to make the text input area fill the window!
        RECT rcClient;
        GetClientRect(hwnd, &rcClient);
        HWND hEdit = GetDlgItem(hwnd, 0);
        SetWindowPos(hEdit, NULL, 0, 0, rcClient.right, rcClient.bottom, SWP_NOZORDER);
    }
        return 0;
    case WM_DESTROY:
        // Unregister the hotkey
        UnregisterHotKey(hwnd, ID_HOTKEY);
        UnregisterHotKey(hwnd, ID_HOTKEY_SAVE);
        UnregisterHotKey(hwnd, ID_HOTKEY_OPEN);
        // Exit the program
        // Goodbye cruel world!
        PostQuitMessage(0);
        return 0;
    // This part is for the hotkeys!
    // This is very ugly. I should probably make a function for this!
    case WM_HOTKEY:
        if (wParam == ID_HOTKEY)
        {
            ShowWindow(hwnd, SW_RESTORE);
            MessageBox(NULL, "Hotkey [CTRL] + L pressed. Hello there!", "BloatNote", MB_OK);
            break;
        }
        if (wParam == ID_HOTKEY_SAVE)
        {
            ShowWindow(hwnd, SW_RESTORE);
            //MessageBox(NULL, "Save pressed", "BloatNote", MB_OK);
            // Save the content of the text input area to a file
            HWND hEdit = GetDlgItem(hwnd, 0);
            DWORD textLength = GetWindowTextLength(hEdit);

            // Allocate memory to store the text
            char *content = (char *)malloc((textLength + 1) * sizeof(char));
            if (content == NULL)
            {
                MessageBox(hwnd, "Failed to allocate memory.", "Error", MB_OK | MB_ICONERROR);
                return 0;
            }
            // Retrieve the text from the edit control
            GetWindowText(hEdit, content, textLength + 1);
            // Save the content to a file using the good ole file dialog!
            if (!SaveToFile(hwnd, content, textLength))
            {
                MessageBox(hwnd, "Failed to save the file.", "Error", MB_OK | MB_ICONERROR);
                return 0;
            }
            // Free the allocated memory
            free(content);
            break;
        }
        if (wParam == ID_HOTKEY_OPEN)
        {
            ShowWindow(hwnd, SW_RESTORE);
            // Open a file to edit
            HWND hEdit = GetDlgItem(hwnd, 0);
            int result = MessageBox(hwnd, "Unsaved work will be lost. Do you want to continue?", "Warning", MB_YESNO | MB_ICONWARNING);
            if (result != IDYES)
            {
                return 0;
            }
            // Clear the text input area
            SetWindowText(hEdit, "");
            // Open the file to edit
            if (!OpenFileToEdit(hwnd, hEdit))
            {
                MessageBox(hwnd, "Failed to open the file.", "Error", MB_OK | MB_ICONERROR);
                return 0;
            }
            break;
        }
        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

// Now come the two functions we declared earlier!
// Let's define them now!
OpenFileToEdit(HWND hwnd, HWND hEdit)
{
    // Scary looking struct!
    OPENFILENAME ofn;
    char fileName[MAX_PATH] = "";
    ZeroMemory(&ofn, sizeof(OPENFILENAME));
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFilter = "Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
    ofn.lpstrFile = fileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_FILEMUSTEXIST;

    // Display the open file dialog
    if (GetOpenFileName(&ofn) == TRUE)
    {
        FILE *file;
        errno_t err = fopen_s(&file, ofn.lpstrFile, "r");
        if (err != 0)
        {
            MessageBox(hwnd, "Failed to open the file.", "Error", MB_OK | MB_ICONERROR);
            fclose(file);
            return FALSE;
        }

        // Now we read the file content and get the file size
        long fileSize = 0;
        fseek(file, 0, SEEK_END);
        fileSize = ftell(file);
        fseek(file, 0, SEEK_SET);
        // Allocate memory to store the file content. Note: I changed malloc to calloc because it automatically sets the allocated memory to 0. Did I explain why?
        char *content = (char *)calloc(fileSize + 1, sizeof(char));
        /*
        content[fileSize +1] = '\0'; // Add the null terminator at the end of the file content. 
        Because calloc already sets the allocated memory to 0, we don't need to do this!
        Now we can read the file content into the allocated memory!
        */

        fread(content, sizeof(char), fileSize, file);
        if (content == NULL)
        {

            MessageBox(hwnd, "Failed to allocate memory.", "Error", MB_OK | MB_ICONERROR);
            fclose(file);
            return FALSE;
        }

        // Now we have the file content stored in memory but we need to replace any newline "\n" characters with "\r\n" because windows is very strange :/
        for (int i = 0; i < fileSize; i++)
        {
            // If the current character is \n and the previous character is not \r
            if (content[i] == '\n' && content[i - 1] != '\r')
            {
                // Reallocate memory to make space for the \r character
                content = (char *)realloc(content, (fileSize + 2) * sizeof(char));
                // Move the content after the current character one character to the right
                memmove(content + i + 1, content + i, (fileSize - i) * sizeof(char));

                // Replace \n with \r\n (insert \r before \n)
                content[i] = '\r';
                content[i + 1] = '\n';

                // Increase the file size by 1
                fileSize++;
            }
        }
        // Now we have content in memory and we've fixed the newline, it's almost ready to be displayed!
        // But first we need to convert it to Unicode (UTF-8)
        // Here we allocate memory for the wide string.
        int output_size = MultiByteToWideChar(CP_UTF8, 0, content, -1, NULL, 0);
        wchar_t *wstring = malloc(output_size * sizeof(wchar_t));

        // convert the content to Unicode
        MultiByteToWideChar(CP_UTF8, 0, content, -1, wstring, output_size);
        // Set the file content in the edit control
        SetWindowTextW(hEdit, wstring);
        // Free the allocated memory
        free(content);
        free(wstring);

        fclose(file);
        return TRUE;
    }

    return FALSE;
}

SaveToFile(HWND hwnd, const char *content, DWORD contentLength)
{
    OPENFILENAME ofn;
    char fileName[MAX_PATH] = "";

    // Initialize the OPENFILENAME structure
    ZeroMemory(&ofn, sizeof(OPENFILENAME));
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFilter = "Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
    ofn.lpstrFile = fileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrDefExt = "txt";
    ofn.Flags = OFN_OVERWRITEPROMPT;

    // Display the save file dialog
    if (GetSaveFileName(&ofn) != TRUE)
    {
        return FALSE;
    }

    FILE *file;
    errno_t err = fopen_s(&file, ofn.lpstrFile, "w");
    if (err != 0)
    {
        MessageBox(hwnd, "Failed to open the save file.", "Error", MB_OK | MB_ICONERROR);
        fclose(file);
        return FALSE;
    }
    
    fwrite(content, sizeof(char), contentLength, file);
    fclose(file);
    return TRUE;

    return TRUE;
}
