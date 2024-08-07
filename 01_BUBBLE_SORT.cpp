#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
//Function made for parallel bubble sort using openmp
void BSP(int arr[], int size) {
	//omp parallel region starts
    #pragma omp parallel
    {
    	//outer loop for the no of elements in array
        for (int i = 0; i < size - 1; i++) {
        	//omp loop parallel for comparing and sorting
            #pragma omp for
            for (int j = 0; j < size - i - 1; j++) {
            	//if required then compare and swap
                if (arr[j] > arr[j + 1]) {
                    int temp = arr[j];
                    arr[j] = arr[j + 1];
                    arr[j + 1] = temp;
                }
            }
        }
    }
}
//will read no of threads and dat set form command line
int main(int argc, char *argv[]) {
	//check if CL arg are passed
    if (argc != 3) {
        printf("Usage: %s <size_of_dataset> <num_threads>\n", argv[0]);
        return 1;
    }
    int size = atoi(argv[1]);
    int num_threads = atoi(argv[2]);
    //set the no of threads
    omp_set_num_threads(num_threads);
    //seed the rand no
    srand((unsigned int)time(NULL));
    //memory alloc for array of size which user entered
    int *array = malloc(size * sizeof(int));
    //generate the rand numbers
    for (int i = 0; i < size; ++i) {
        array[i] = rand() % 100;
    }
    //printf("\nUnsorted array: ");
    //for (int i = 0; i < size; ++i) {
      //  printf("%d ", array[i]);
    //}
    printf("\n");
    double start_time = omp_get_wtime();
    BSP(array, size);
    double end_time = omp_get_wtime();
    // Display the sorted array
    //printf("Sorted array: ");
    //for (int i = 0; i < size; ++i) {
      //  printf("%d ", array[i]);
    //}
    //printf("\n");
    printf("Parallel Bubble Sort - Execution time: %f seconds\n", end_time - start_time);
    // Free allocated memory
    free(array);
    return 0;
}

