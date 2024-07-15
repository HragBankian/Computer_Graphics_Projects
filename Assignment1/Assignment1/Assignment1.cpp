// Assignment1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>



int* createArray(int n) {
    int* arr = new int[n];
    return arr;
}
 
void initializeArray(int* arr, int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = i;
    }
}


void printArray(int* arr, int n) {

    std::cout << "Array: [";
    for (int i = 0; i < n; i++) {
        std::cout << arr[i] << ", ";
    }
    std::cout << std::endl << "]\n";
}

void deleteArray(int* arr) {
    delete[] arr;
}


int main()
{
    
    int* arr = createArray(10);
    printArray(arr, 10);
    initializeArray(arr, 10);
    printArray(arr, 10);
    deleteArray(arr);
    
    std::cout << "Hello World!\n";
}


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
