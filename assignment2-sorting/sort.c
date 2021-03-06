/*
sort.c, code demonstrating and quicksort & heapsort for the purpose of comparison
Author: David J. Bourke, Student Number: 12304135
Date started:   4th of November 2016
Date submitted: 15th of November 2016
Dependencies:   heapsort.h, quicksort.h
Compile with $ gcc sort.c -std=c99 -o sort
*/

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>

#include "quicksort.h"
#include "heapsort.h"

#define HEAPSORT_CODE   1
#define QUICKSORT_CODE  2

// This is ugly, but you asked for it
typedef struct sort_metrics {
    int probes;
    double clock_ticks;
    bool sorted;
} sort_metrics;

// Tests if an array is sorted (increasing values), 
// returns true if array is sorted, returns false if the array is unsorted
static inline bool isSorted (int* array, int array_length) {
    int failtest = 0;
    for (int i = 1; i < array_length; i++) {
        // If the ith value is less than its preceding value, array is unsorted
        if (array[i] < array[i - 1]) {
            return false;   // Return false if any two adjacant indexes out of order
        }
    }
    return true;
}

// pass 1 to time heapsort, pass 2 to time quicksort, function should default to quicksort
// returns runtime of sorting function with test_array and array_length passed as arguments
// returns -1 if  array (up to the length specified) is not sorted correctly
static inline sort_metrics testSortTime (int* test_array, int array_length, int sort_function) {
     // Struct to record runtime in probes and clock tics, also a bool to identify if the array is correctly sorted
    sort_metrics test_metrics;
    clock_t t_start;

    if (sort_function == HEAPSORT_CODE) {
        t_start = clock();
        test_metrics.probes = heapsort(test_array, array_length + 1);
        test_metrics.clock_ticks = clock() - t_start;
    } else {    // Default kept so we can add more sort functions later
        t_start = clock();
        test_metrics.probes = quicksort(test_array, array_length);
        test_metrics.clock_ticks = clock() - t_start;
    }
    // check if array is sorted (up to the length specified in array_length)
    test_metrics.sorted = isSorted(test_array, array_length);
    return test_metrics;
}

static inline void randomiseArray (int* array, int array_length) {
    srand(time(NULL));  // Seed prng for test_array random ints
    // Loop through array, assigning random ints to each element
    for (int i = 0; i < array_length; i++) {
        array[i] = rand();
    }
}

// Print array of doubles to a CSV file
static inline void fprintArrayCSV_double (double* array, int array_length, char* filename) {
    FILE* myfile = fopen(filename, "w");
    for (int i = 0; i < array_length; i++) {
        fprintf(myfile, "%d,%lf\n", i + 1, array[i]);
    }
    fclose(myfile);
}

// Print array of ints to a CSV file
static inline void fprintArrayCSV_int (int* array, int array_length, char* filename) {
    FILE* myfile = fopen(filename, "w");
    for (int i = 0; i < array_length; i++) {
        fprintf(myfile, "%d,%d\n", i + 1, array[i]);
    }
    fclose(myfile);
}

int main (void) {
    int max_array_size = 5000;
    int* test_array = (int*)malloc(max_array_size * sizeof(int));
    double clocks_to_sort_QS[max_array_size];
    double clocks_to_sort_HS[max_array_size];
    int probes_to_sort_HS[max_array_size];
    int probes_to_sort_QS[max_array_size];
    sort_metrics test_metrics;
    
    for (int i = 1; i <= max_array_size; i++) {
        randomiseArray(test_array, i - 1);  // Randomise test data array up to index i - 1
        test_metrics = testSortTime(test_array, i, QUICKSORT_CODE);
        if (test_metrics.sorted == false) { printf("quicksort failed\n"); }
        clocks_to_sort_QS[i - 1] = test_metrics.clock_ticks;    // Record clock tics taken to sort i ints
        probes_to_sort_QS[i - 1] = test_metrics.probes;         // Record probes taken to sort i ints

        randomiseArray(test_array, i - 1);  // Randomise test data array up to index i - 1
        test_metrics = testSortTime(test_array, i, HEAPSORT_CODE);
        if (test_metrics.sorted == false) { printf("Heapsort failed\n"); }
        clocks_to_sort_HS[i - 1] = test_metrics.clock_ticks;   // Record clock tics take to sort i ints
        probes_to_sort_HS[i - 1] = test_metrics.probes;        // Record probes taken to sort i ints
    }
    // Write test results to csv files
    fprintArrayCSV_double(clocks_to_sort_QS, max_array_size, "./quicksort_clock_results.csv");
    fprintArrayCSV_double(clocks_to_sort_HS, max_array_size, "./heapsort_clock_results.csv");
    fprintArrayCSV_int(probes_to_sort_QS, max_array_size, "./quicksort_probe_results.csv");
    fprintArrayCSV_int(probes_to_sort_HS, max_array_size, "./heapsort_probe_results.csv");
}
