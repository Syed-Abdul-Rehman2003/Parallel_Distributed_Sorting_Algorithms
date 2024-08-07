#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
//Merge function (M) to merge two halves of the array in sorted order
void M(int arr[], int l, int m, int r, int temp[]) {
	//calculate the size of 2 halfs
    int n1 = m - l + 1;
    int n2 = r - m;
    // // Copy data to temporary arrays temp[]
    for (int i = 0; i < n1; i++)
        temp[i] = arr[l + i];
    for (int j = 0; j < n2; j++)
        temp[n1 + j] = arr[m + 1 + j];
        //// Merge the two halves back into arr[l..r]
    int i = 0, j = n1, k = l;
    // Copy the remaining elements of temp[], if any
    while (i < n1 && j < n1 + n2) {
        if (temp[i] <= temp[j]) {
            arr[k] = temp[i];
            i++;
        } else {
            arr[k] = temp[j];
            j++;
        }
        k++;
    }
    while (i < n1) {
        arr[k] = temp[i];
        i++;
        k++;
    }
    while (j < n1 + n2) {
        arr[k] = temp[j];
        j++;
        k++;
    }
}
//Main parallel merge sort function (MSP)
void MSP(int arr[], int l, int r, int temp[]) {
	//Recursive function to perform merge sort
    if (l < r) {
    	//// Find the middle point
        int m = l + (r - l) / 2;
        //// Parallelize the recursive calls using OpenMP
#pragma omp parallel sections
        {
#pragma omp section
//// Recursively sort the first half
            MSP(arr, l, m, temp);
#pragma omp section
//// Recursively sort the sec half
            MSP(arr, m + 1, r, temp);
        }
        //merge the sorted halfs
        M(arr, l, m, r, temp);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <size_of_dataset> <num_threads>\n", argv[0]);
        return 1;
    }
    int size = atoi(argv[1]);
    int num_threads = atoi(argv[2]);
    omp_set_num_threads(num_threads);//set no of threads taken from run time
    //seed the random number
    srand((unsigned int)time(NULL));
    int *array = malloc(size * sizeof(int));
    int *temp_array = malloc(size * sizeof(int));
    for (int i = 0; i < size; ++i) {
        array[i] = rand() % 100;
    }
    double start_time = omp_get_wtime();
    MSP(array, 0, size - 1, temp_array);
    double end_time = omp_get_wtime();
    printf("Parallel Merge Sort - Execution time: %f seconds with %d data set and %d no of threads\n", end_time - start_time, size, num_threads);
    FILE *output = fopen("sorted_array_merge.txt", "w");
    if (output == NULL) {
        fprintf(stderr, "Error opening output file for writing.\n");
        return 1;
    }

    for (int i = 0; i < size; ++i) {
        fprintf(output, "%d ", array[i]);
    }
    fclose(output);
	free(array);
    free(temp_array);

    return 0;
}
