#include <bits/stdc++.h>
using namespace std;

// Module: Array Analysis
namespace ArrayAnalysis {
    // Function to find the smallest element in an array
    int findSmallest(int arr[], int n) {
        int minVal = INT_MAX;
        for (int i = 0; i < n; i++) {
            minVal = min(minVal, arr[i]);
        }
        return minVal;
    }

    // Function to find the largest element in an array
    int findLargest(int arr[], int n) {
        int maxVal = INT_MIN;
        for (int i = 0; i < n; i++) {
            maxVal = max(maxVal, arr[i]);
        }
        return maxVal;
    }
}

int main() {
    cout << "Finding Maximum and Minimum elements in an array" << endl;
    int n;
    cout << "Give size of array : ";
    cin >> n;
    int arr[n];
    cout << "Give elements of array : ";
    for (int i = 0; i < n; i++) cin >> arr[i];

    // Call the analysis functions and capture their results
    int minVal = ArrayAnalysis::findSmallest(arr, n);
    int maxVal = ArrayAnalysis::findLargest(arr, n);

    cout << "Minimum element is : " << minVal << endl;
    cout << "Maximum element is : " << maxVal << endl;

    return 0;
}
