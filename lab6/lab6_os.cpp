#include <iostream>
#include <windows.h>
#include <synchapi.h>
#include <thread>
#include <mutex>
#include <condition_variable>

using namespace std;

mutex m;
condition_variable cv;

int X, countEqual = 0;
const int P = INT_MIN;

void work(int* array) {
    int sleepTime;
    cout << "Enter the time to sleep between elements: ";
    cin >> sleepTime;
    int count = 0;
    for (int i = 0; array[i] != P; i++) {
        int a = array[i];
        if (a < 0 && a % 3 == 0) {
            array[i] = array[count];
            array[count++] = a;
        }
    }
    for (int i = 0; array[i] != P; i++) {
        cout << array[i] << " ";
        Sleep(sleepTime);
    }
    cout << '\n';
}

void CountElement(int* array) {
    unique_lock<mutex> lock(m);
    for (int i = 0; array[i] != P; i++) {
        int elem = array[i];
        if (elem == X) {
            countEqual++;
        }
    }
    cv.notify_one();
}

int main() {
    int n;
    cout << "Enter size of the array: ";
    cin >> n;
    auto array = make_unique<int[]>(n + 1);
    cout << "Enter elements of the array: ";
    for (int i = 0; i < n; i++) {
        cin >> array[i];
    }
    array[n] = P;
    cout << "Array size is " << n << '\n';
    cout << "Array elements: ";
    for (int i = 0; i < n; i++) {
        cout << array[i] << " ";
    }

    cout << "\nEnter X: ";
    cin >> X;

    thread t1(work, array.get());

    thread t2(CountElement, array.get());

    unique_lock<mutex> lock(m);

    cv.wait(lock);

    cout << "Number of elements of the array equal to X: " << countEqual << '\n';

    t1.join();

    t2.join();

    return 0;
}