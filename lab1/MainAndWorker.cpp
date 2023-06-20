#include <windows.h>
#include <iostream>

using namespace std;


DWORD WINAPI worker(LPVOID array) {
	int count = 0;
	for (int i = 1; i < ((int*)array)[0] + 1; i++) {
		if (((int*)array)[i] == 0) {
			count++;
		}
		Sleep(12);
	}
	cout << count;
	return 0;
}

int main()
{
	int n;
	cin >> n;
	int* array = new int[n + 1];
	array[0] = n;
	for (int i = 1; i < n + 1; i++) {
		int element;
		cin >> element;
		array[i] = element;
	}
	HANDLE hThread;
	DWORD IDThread;
	hThread = CreateThread(NULL, 0, worker, (void*)array, 0, &IDThread);
	if (hThread == NULL) {
		return GetLastError();
	}
	SuspendThread(hThread);
	Sleep(5000);
	ResumeThread(hThread);
	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hThread);

}