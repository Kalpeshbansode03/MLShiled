#include <iostream>
#include <vector>
#include <omp.h>

using namespace std;

// Function to perform parallel reduction for finding minimum value
int parallel_min(const vector<int>& data) {
    int min_val = data[0];

    #pragma omp parallel for reduction(min: min_val)
    for (int i = 1; i < data.size(); ++i) {
        if (data[i] < min_val) {
            min_val = data[i];
        }
    }

    return min_val;
}

// Function to perform parallel reduction for finding maximum value
int parallel_max(const vector<int>& data) {
    int max_val = data[0];

    #pragma omp parallel for reduction(max: max_val)
    for (int i = 1; i < data.size(); ++i) {
        if (data[i] > max_val) {
            max_val = data[i];
        }
    }

    return max_val;
}

// Function to perform parallel reduction for finding sum
int parallel_sum(const vector<int>& data) {
    int sum = 0;

    #pragma omp parallel for reduction(+: sum)
    for (int i = 0; i < data.size(); ++i) {
        sum += data[i];
    }

    return sum;
}

// Function to perform parallel reduction for finding average
double parallel_average(const vector<int>& data) {
    int sum = parallel_sum(data);
    return static_cast<double>(sum) / data.size();
}

int main() {
    // Input the size of the data
    int n;
    cout << "Enter the number of elements: ";
    cin >> n;

    // Input data from the user
    vector<int> data(n);
    cout << "Enter " << n << " elements: ";
    for (int i = 0; i < n; ++i) {
        cin >> data[i];
    }

    // Find minimum value
    int min_val = parallel_min(data);
    cout << "Minimum value: " << min_val << endl;

    // Find maximum value
    int max_val = parallel_max(data);
    cout << "Maximum value: " << max_val << endl;

    // Find sum
    int sum = parallel_sum(data);
    cout << "Sum: " << sum << endl;

    // Find average
    double average = parallel_average(data);
    cout << "Average: " << average << endl;

    return 0;
}

