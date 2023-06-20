#include <iostream>
#include <windows.h>
#include <conio.h>
#include <vector>
#include <random>
#include <string>
#include <ctime>

int main() {
    int arraySize;
    int N, M;
    std::cout << "Enter array size: ";
    std::cin >> arraySize;
    
    std::vector<__int8> vec;
    srand(time(NULL));
    for (int i = 0; i < arraySize; i++) {
        __int8 num = rand() % 20 + 65; 
        vec.push_back(num);
    }

    HANDLE hWritePipe1, hReadPipe1;
    HANDLE hWritePipe2, hReadPipe2;
    HANDLE hEnableRead = CreateEventA(NULL, FALSE, FALSE, "EnableRead"); 
   
    STARTUPINFOW si;
    PROCESS_INFORMATION pi;

    SECURITY_ATTRIBUTES sa; 
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.lpSecurityDescriptor = NULL; 
    sa.bInheritHandle = TRUE; 

    if (!CreatePipe(&hReadPipe1, &hWritePipe1, &sa, 0)) {
        std::cout << "Create pipe failed.\n";
        _getch();
        return GetLastError();
    }

    if (!CreatePipe(&hReadPipe2, &hWritePipe2, &sa, 0)) {
        std::cout << "Create pipe failed.\n";
        _getch();
        return GetLastError();
    }

    ZeroMemory(&si, sizeof(STARTUPINFO));
    si.cb = sizeof(STARTUPINFO);

    std::wstring commandLine = L"Client.exe " + std::to_wstring(reinterpret_cast<intptr_t>(hReadPipe1)) + L" " + 
        std::to_wstring(reinterpret_cast<intptr_t>(hWritePipe2));
    wchar_t* commLine = new wchar_t[commandLine.length()];
    wcscpy_s(commLine, commandLine.length() + 1, commandLine.c_str());
    if (!CreateProcessW(NULL, commLine, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
        _cputs("Create process failed.\n");
        _cputs("Press any key to finish.\n");
        _getch();
        return GetLastError();
    }

    DWORD dwBytesWritten1;
    if (!WriteFile(hWritePipe1, &arraySize, sizeof(arraySize), &dwBytesWritten1, NULL)) {
        _cputs("Write to file failed.\n");
        _cputs("Press any key to finish.\n");
        _getch();
        return GetLastError();
    }

    for (__int8 x : vec) {
        if (!WriteFile(hWritePipe1, &x, sizeof(x), &dwBytesWritten1, NULL)) {
            _cputs("Write to file failed.\n");
            _cputs("Press any key to finish.\n");
            _getch();
            return GetLastError();
        }
        _cprintf("The number %.d is written to the pipe.\n", x);
    } 
    _cputs("The process finished writing to the pipe.\n");

    WaitForSingleObject(hEnableRead, INFINITE);

    std::vector<__int8> vecSorted;

    for (int i = 0; i < arraySize; i++) {
        __int8 nData;
        DWORD dwBytesRead2;
        if (!ReadFile(hReadPipe2, &nData, sizeof(nData), &dwBytesRead2, NULL)) {
            _cputs("Read from the pipe failed.\n");
            std::cout << (GetLastError()) << "\n";
            _cputs("Press any key to finish.\n");
            _getch();
            return GetLastError();
        }
        _cprintf("The symbol %c is read from the pipe.\n", nData);
        vecSorted.push_back(nData);
    }
    _cputs("The process finished reading from the pipe.\n");

    std::cout << "Sent array: ";
    std::cout << "[ ";
    for (__int8 x : vecSorted) {
        std::cout << x << " ";
    }
    std::cout << "]";

    _cputs("\nThe process finished reading from the pipe.\n");
    system("pause");

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    CloseHandle(hWritePipe1);
    CloseHandle(hReadPipe1);
    CloseHandle(hWritePipe2);
    CloseHandle(hReadPipe2);
    CloseHandle(hEnableRead);
}