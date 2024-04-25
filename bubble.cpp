#include <iostream>
#include <vector>
#include <omp.h>

using namespace std;

// Function to perform sequential bubble sort
void bubbleSortSequential(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n-1; ++i) {
        for (int j = 0; j < n-i-1; ++j) {
            if (arr[j] > arr[j+1]) {
                swap(arr[j], arr[j+1]);
            }
        }
    }
}

// Function to perform parallel bubble sort
void bubbleSortParallel(vector<int>& arr) {
    int n = arr.size();
    bool swapped = true;
    while (swapped) {
        swapped = false;
        #pragma omp parallel for shared(arr, swapped)
        for (int i = 0; i < n-1; ++i) {
            if (arr[i] > arr[i+1]) {
                swap(arr[i], arr[i+1]);
                swapped = true;
            }
        }
        #pragma omp barrier
    }
}

int main() {
    int n;
    cout << "Enter the number of elements: ";
    cin >> n;

    // Input the elements
    vector<int> arr(n);
    cout << "Enter " << n << " elements: ";
    for (int i = 0; i < n; ++i) {
        cin >> arr[i];
    }

    // Make a copy for parallel sorting
    vector<int> arr_copy(arr);

    // Measure sequential bubble sort performance
    double start_time = omp_get_wtime();
    bubbleSortSequential(arr);
    double end_time = omp_get_wtime();
    cout << "Sequential Bubble Sort Time: " << end_time - start_time << " seconds" << endl;

    // Measure parallel bubble sort performance
    start_time = omp_get_wtime();
    bubbleSortParallel(arr_copy);
    end_time = omp_get_wtime();
    cout << "Parallel Bubble Sort Time: " << end_time - start_time << " seconds" << endl;

    // Print the sorted arrays
    cout << "Sorted array (sequential): ";
    for (int i = 0; i < n; ++i) {
        cout << arr[i] << " ";
    }
    cout << endl;

    cout << "Sorted array (parallel): ";
    for (int i = 0; i < n; ++i) {
        cout << arr_copy[i] << " ";
    }
    cout << endl;

    return 0;
}

