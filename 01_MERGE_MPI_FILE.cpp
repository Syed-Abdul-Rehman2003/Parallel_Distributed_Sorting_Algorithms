#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
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
        //// Recursively sort the first half
        MSP(arr, l, m, temp);
        //// Recursively sort the 2nd half
        MSP(arr, m + 1, r, temp);
        //MERGE 
        M(arr, l, m, r, temp);
    }
}
int main(int argc, char *argv[]) {
    int np, rank;
    // Enter into MPI env
    MPI_Init(&argc, &argv);
    // Get total no of processes
    MPI_Comm_size(MPI_COMM_WORLD, &np);
    // Get the rank of the process
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (argc != 3) {
        if (rank == 0)
            printf("Usage: %s <size_of_dataset> <num_processes>\n", argv[0]);
        MPI_Finalize();
        return 1;
    }
    int size = atoi(argv[1]);
    int *array = NULL;
    if (rank == 0) {
        srand((unsigned int)time(NULL));
        array = (int *)malloc(size * sizeof(int));

        for (int i = 0; i < size; ++i) {
            array[i] = rand() % 100;
        }
    }
    MPI_Bcast(&size, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank != 0) {
        array = (int *)malloc(size * sizeof(int));
    }
    MPI_Bcast(array, size, MPI_INT, 0, MPI_COMM_WORLD);
    // Perform parallel merge sort on each process
    int ls = size / np;
    int *local_array = (int *)malloc(ls * sizeof(int));
    MPI_Scatter(array, ls, MPI_INT, local_array, ls, MPI_INT, 0, MPI_COMM_WORLD);
    // Additional check for insufficient memory
    if (local_array == NULL) {
        fprintf(stderr, "Memory allocation failed for local array in process %d\n", rank);
        MPI_Finalize();
        return 1;
    }
    int *tla = (int *)malloc(ls * sizeof(int));
	double start_time = MPI_Wtime();
    MSP(local_array, 0, ls - 1, tla);
    double end_time = MPI_Wtime();
    // Gather the sorted data to the root process
    MPI_Gather(local_array, ls, MPI_INT, array, ls, MPI_INT, 0, MPI_COMM_WORLD);
    if (rank == 0) {
        // Merge the sorted arrays at the root process
        int *tam = (int *)malloc(size * sizeof(int));
        M(array, 0, (size / 2 - 1) / 2, size - 1, tam);
	 printf("Parallel Merge Sort - Execution time: %f seconds with %d data set\n", end_time - start_time, size);
        // Print the sorted array
        free(array);
        free(tam);
    } else {
        free(local_array);
    }
    free(tla);
    MPI_Finalize();
    return 0;
}

