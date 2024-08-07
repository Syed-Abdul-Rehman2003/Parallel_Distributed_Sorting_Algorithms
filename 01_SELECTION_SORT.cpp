#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

void SSP(int arr[], int size) {
	//extablishing openmp parallel region
    #pragma omp parallel
    {
    	//nested loop will  iterates through the array to find the minimum element's index in each iteration.
        for (int i = 0; i < size - 1; i++) {
            int min_idx = i;
            #pragma omp for
            for (int j = i + 1; j < size; j++) {
                if (arr[j] < arr[min_idx]) {
                    min_idx = j;
                }
            }
            //here starts actual swapping  under single to ensure mutual exclusion and avoid race conditions.
            #pragma omp single
            {
                if (min_idx != i) {
                    int temp = arr[i];
                    arr[i] = arr[min_idx];
                    arr[min_idx] = temp;
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
    omp_set_num_threads(num_threads);//set no of threads
    srand((unsigned int)time(NULL));//seed the rand value
    int *array = malloc(size * sizeof(int));// allocate mem and initialized the array with random values.
    for (int i = 0; i < size; ++i) {
        array[i] = rand() % 100;
    }
    printf("\n");
    double start_time = omp_get_wtime();
    SSP(array, size);
    double end_time = omp_get_wtime();
    printf("\nParallel Selection Sort - Execution time: %f seconds\n", end_time - start_time);
    free(array);
    return 0;
}

