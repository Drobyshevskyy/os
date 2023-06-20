#include <windows.h>
#include <conio.h>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <random>
#include <string>
#include <ctime>
#include <algorithm>

int main(int argc, char* argv[]) {

   
    HANDLE hReadPipe1 = (HANDLE)atoi(argv[1]);
    HANDLE hWritePipe2 = (HANDLE)atoi(argv[2]);
   

    HANDLE hEnableRead = OpenEventA(EVENT_ALL_ACCESS, FALSE, "EnableRead");

    int arraySize;

    DWORD dwBytesRead1;
    if (!ReadFile(hReadPipe1, &arraySize, sizeof(arraySize), &dwBytesRead1, NULL)) {
        _cputs("Read from the pipe failed.\n");
        _cputs("Press any key to finish.\n");
        _getch();
        return GetLastError();
    }

    _cprintf("The number %.d is read from the pipe.\n", arraySize);


    std::vector<__int8> vec;

    for (int i = 0; i < arraySize; i++) {
        __int8 nData;
        if (!ReadFile(hReadPipe1, &nData, sizeof(nData), &dwBytesRead1, NULL)) {
            _cputs("Read from the pipe failed.\n");
            std::cout << (GetLastError()) << "\n";
            _cputs("Press any key to finish.\n");
            _getch();
            return GetLastError();
        }
        _cprintf("The symbol %c is read from the pipe.\n", nData);
        vec.push_back(nData);
    }
    _cputs("The process finished reading from the pipe.\n");

    _cputs("Press any key to continue communication.\n");
    _getch();
   
    std::sort(vec.begin(), vec.end());

    std::cout << "\n";
    for (__int8 x : vec) {
        DWORD dwBytesWritten2;
        if (!WriteFile(hWritePipe2, &x, sizeof(x), &dwBytesWritten2, NULL)) {
            _cputs("Write to file failed.\n");
            _cputs("Press any key to finish.\n");
            _getch();
            return GetLastError();
        }
        _cprintf("The symbol %c is written to the pipe.\n", x);
    } 
    SetEvent(hEnableRead); 
    _cputs("The process finished writing to the pipe.\n");

    CloseHandle(hReadPipe1);
    CloseHandle(hWritePipe2);
    CloseHandle(hEnableRead);

    system("pause");
    return 0;
}