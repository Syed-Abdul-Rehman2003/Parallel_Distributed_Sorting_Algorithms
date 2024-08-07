#include <stdio.h>
#include <omp.h>
#define N 1000000
//make a swap function
void swap(int* n1, int* n2) {
    int temp = *n1;
    *n1 = *n2;
    *n2 = temp;
}
//partition func choosing last element as pivot
int PART(int arr[], int low, int high) {
    int pivt = arr[high]; //chhose the last one as pivot
    int i = (low - 1);
    for (int j = low; j <= high - 1; j++) {
    	//if curr <=pivot
        if (arr[j] <= pivt) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}
//serial quick sort function
void QSS(int arr[], int low, int high) {
    if (low < high) {
    	// Get the partitioning index
        int piv = PART(arr, low, high);
         // Recursively sort elements before partition and after partition
        QSS(arr, low, piv - 1);
        QSS(arr, piv + 1, high);
    }
}
//parallel quick sort func
void QSP(int arr[], int low, int high) {
    if (low < high) {
    	// Get the partitioning index
        int piv = PART(arr, low, high);
         // Recursively sort elements before partition and after partition
        #pragma omp task
        QSP(arr, low, piv - 1);
        #pragma omp task
        QSP(arr, piv + 1, high);
    }
}
void print(int arr[], int size) {
    for (int i = 0; i < size; i++)
        printf("%d ", arr[i]);
    printf("\n");
}
int main() {
    int arr[N];
    // Initialize the array in descending order
    for(int i=0;i<N;i++) arr[i] = N-i;
    //perform the parallel quick sort
    double st = omp_get_wtime();
    
    #pragma omp parallel
    {
        #pragma omp single
        QSP(arr, 0, N - 1);
    }
    double et = omp_get_wtime();
    printf("\nTime taken: %f seconds\n", et - st);
    return 0;
}
