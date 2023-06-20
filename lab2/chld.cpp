#include <iostream>
#include <cmath>
using namespace std;
int main(int argc, char* argv[])
{

    int* arr = new int[argc];
    for (int i = 0; i < argc; i++) {
        arr[i] = atoi(argv[i]);
    }
    int count, key;
    for (int i = 0; i < argc - 1; i++) {
        count = arr[i];
        key = i;
        for (int j = i + 1; j < argc; j++) {
            if (arr[j] < arr[key]) {
                key = j;
            }
        }
        if (key != i) {
            arr[i] = arr[key];
            arr[key] = count;
        }
    }
    
    for (int i = 0; i < argc; i++)
    {
        cout << arr[i] << " ";
    }
    cout << "\n";
    system("pause");
    return 0;
}