#include <iostream>
#include <vector>
#include <omp.h>

using namespace std;

// Function to merge two sorted subarrays
void merge(vector<int>& arr, int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;

    vector<int> L(n1), R(n2);

    for (int i = 0; i < n1; ++i)
        L[i] = arr[l + i];
    for (int j = 0; j < n2; ++j)
        R[j] = arr[m + 1 + j];

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

// Function to perform sequential merge sort recursively
void mergeSortSequential(vector<int>& arr, int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;

        mergeSortSequential(arr, l, m);
        mergeSortSequential(arr, m + 1, r);

        merge(arr, l, m, r);
    }
}

// Function to perform parallel merge sort recursively
void mergeSortParallel(vector<int>& arr, int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;

        #pragma omp parallel sections
        {
            #pragma omp section
            mergeSortParallel(arr, l, m);
            #pragma omp section
            mergeSortParallel(arr, m + 1, r);
        }

        merge(arr, l, m, r);
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

    // Measure sequential merge sort performance
    double start_time = omp_get_wtime();
    mergeSortSequential(arr, 0, n - 1);
    double end_time = omp_get_wtime();
    cout << "Sequential Merge Sort Time: " << end_time - start_time << " seconds" << endl;

    // Measure parallel merge sort performance
    start_time = omp_get_wtime();
    mergeSortParallel(arr_copy, 0, n - 1);
    end_time = omp_get_wtime();
    cout << "Parallel Merge Sort Time: " << end_time - start_time << " seconds" << endl;

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

