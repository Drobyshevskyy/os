#include <windows.h>
#include <conio.h>
#include <cstdlib>
#include <time.h>
#include <string>
using namespace std;
int main()
{
    wchar_t lpszAppName[] = L"D:\\Microsoft VS Code Projects\\C++\\OS\\Lab2\\child.exe";
    
    STARTUPINFOW si;
    PROCESS_INFORMATION piCom;

    ZeroMemory(&si, sizeof(STARTUPINFO));
    si.cb = sizeof(STARTUPINFO);
    
    si.dwY = 200;
    srand(unsigned(time(0)));
    int size = rand() % 20 + 1;
    wstring temp = to_wstring(rand() % 100);
    for (int i = 1; i < size; i++)
    {
        temp += L" " + to_wstring(rand() % 100);
    }
    wchar_t* lpszCommandLine = new wchar_t[temp.length()];
    wcscpy_s(lpszCommandLine, temp.length() + 1, temp.c_str());
    CreateProcessW(lpszAppName, lpszCommandLine, NULL, NULL, FALSE,
        CREATE_NEW_CONSOLE, NULL, NULL, &si, &piCom);
    WaitForSingleObject(piCom.hProcess, INFINITE);
    CloseHandle(piCom.hThread);
    CloseHandle(piCom.hProcess);
    return 0;
}