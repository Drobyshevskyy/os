#include <iostream>
#include <windows.h>
#include <synchapi.h>
using namespace std;

CRITICAL_SECTION cs;
HANDLE hEvent;
int X, countEqual = 0;

const int P = INT_MIN;

DWORD WINAPI work(LPVOID array)
{
    EnterCriticalSection(&cs);
    int sleepTime;
    cout << "Enter the time to sleep between elements: ";
    cin >> sleepTime;
    int count = 0;
    for (int i = 0; ((int*)array)[i] != P; i++) {
        int a = ((int*)array)[i];
        if (a < 0 && a % 3 == 0) {
            ((int*)array)[i] = ((int*)array)[count];
            ((int*)array)[count++] = a;
        } 
    }
    for (int i = 0; ((int*)array)[i] != P; i++) {
        cout << ((int*)array)[i] << " ";
        Sleep(sleepTime);
    }
    cout << '\n';
    LeaveCriticalSection(&cs);
    return 0;
}

DWORD WINAPI CountElement(LPVOID array)
{
    EnterCriticalSection(&cs);
    for (int i = 0; ((int*)array)[i] != P; i++) {
        int elem = ((int*)array)[i];
        if (elem == X) {
            countEqual++;
        }
    }
    SetEvent(hEvent);
    LeaveCriticalSection(&cs);
    return 0;
}

int main()
{
    int n;
    cout << "Enter size of the array: ";
    cin >> n;
    int* array = new int[n + 1];
    cout << "Enter elements of the array: ";
    for (int i = 0; i < n; i++)
    {
        cin >> array[i];
    }
    array[n] = P;
    cout << "Size of the array is " << n << '\n';
    cout << "Elements of the array: ";
    for (int i = 0; i < n; i++)
    {
        cout << array[i] << " ";
    }
    HANDLE	hThreadWork, hThreadCount;
    DWORD	IDThreadWork, IDThreadCount;
    InitializeCriticalSection(&cs);
    hEvent = CreateEvent(NULL, true, false, NULL);
    hThreadCount = CreateThread(NULL, 0, CountElement, array, CREATE_SUSPENDED, &IDThreadCount);
    hThreadWork = CreateThread(NULL, 0, work, array, CREATE_SUSPENDED, &IDThreadWork);
    cout << "\nEnter X: ";
    cin >> X;
    hThreadWork = CreateThread(NULL, 0, work, array, 0, &IDThreadWork);
    hThreadCount = CreateThread(NULL, 0, CountElement, array, 0, &IDThreadCount);
    WaitForSingleObject(hEvent, INFINITE);
    cout << "number of elements of the array equal to X: " << countEqual << '\n';
    CloseHandle(hThreadWork);
    CloseHandle(hThreadCount);
    DeleteCriticalSection(&cs);
    CloseHandle(hEvent);
    return 0;
}