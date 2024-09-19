// Ethan Johnson
// ID: 1002008374
// gcc

// The following heap sort code has been changed to 
// support minheaps instead of maxheaps. So the names
// of all of the functions have been changed.

// For heap sort code
// index-heap-based priority queue.  heap routines are
// from "Algorithms in C, Third Edition", and
// "Algorithms in Java, Third Edition", Robert Sedgewick

// Summer 2016, changed for CLRS, 3rd ed., but still non-recursive.

// This is a prototype for demonstration purposes only.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    FILE* fp;
    char* current_string;
} FileInfo;

typedef struct {
    char *string;
    int file_index;
} HeapNode;

HeapNode* heap;
FileInfo* files;
int N,          // Number of items in queue
    *pq,        // Priority queue
    *qp,        // Table of handles (for tracking)
    maxQueued,  // Capacity of priority queue
    file_count; // Number of files
int *a;

int parent(int i){
    return (i-1)/2;
}
int left(int i) {
    return 2 * i + 1;
}

int right(int i) {
    return 2 * i + 2;
}

void exch(int i, int j) {
    HeapNode temp = heap[i];
    heap[i] = heap[j];
    heap[j] = temp;
    qp[heap[i].file_index] = i;
    qp[heap[j].file_index] = j;
}

// Changed in maxheapInit to minHeapInit
// Changed int *items to char *items[] to support strings
void minHeapInit()
{
    maxQueued = file_count;
    N = 0;
    heap = malloc(maxQueued * sizeof(HeapNode));
    pq = (int *)malloc((maxQueued + 1) * sizeof(int)); // Contains subscripts to a[]
    if (!pq)
    {
        printf("malloc failed %d\n", __LINE__);
        exit(0);
    }
    qp = (int *)malloc(maxQueued * sizeof(int)); // Inverse of pq, allows changing priorities
    if (!qp)
    {
        printf("malloc failed %d\n", __LINE__);
        exit(0);
    }
    // Set all handles to unused
    for (int i = 0; i < maxQueued; i++) {
        qp[i] = -1;
    }
}

int less(int i, int j)
{
    // Changed the return to support strings as the priority
    return strcmp(heap[i].string, heap[j].string) > 0;
}

void heapIncreaseKey(int k) // AKA swim
{
    while (k > 0 && less(parent(k), k))
    {
        exch(k, parent(k));
        k = parent(k);
    }
}

// Slight changes to support min heap instead of max heap
void minHeapify(int k) // AKA sink
{
    int min = k;
    int j;

    j = left(k);
    if (j < N && less(j, k)) min = j;
    j = right(k);
    if (j < N && less(j, k)) min = j;
    if (min != k)
    {
        exch(k, min);
        minHeapify(min);
    }
}

// Function name changed to minHeapInsert
void minHeapInsert(int k)
{
    if (N == maxQueued) {
        maxQueued *= 2;
        heap = realloc(heap, maxQueued * sizeof(HeapNode));
        pq = realloc(pq, (maxQueued + 1) * sizeof(int));
        qp = realloc(qp, maxQueued * sizeof(int));
    }
    pq[N] = k;
    qp[k] = N;
    heap[N].string = files[k].current_string;
    heap[N].file_index = k;
    heapIncreaseKey(N);
    N++;
}

// Function name changed to heapExtractMin
HeapNode heapExtractMin()
{
    HeapNode root = heap[0];
    if (N > 1)
    {
        heap[0] = heap[N-1];
        qp[heap[0].file_index] = 0;
        minHeapify(0);
    }
    N--;
    return root;
}

// Function name changed to minHeapChange
void minHeapChange(int k)
{ // To be called when priority[k] has changed.
    heapIncreaseKey(qp[k]);
    minHeapify(qp[k]);
}

// Test driver for index-heap-based priority queue.
// Index is just a table of priorities whose
// subscripts are used in the PQ.

int main() {
    char file_name[100];
    FILE *in_file, *out_file;

    in_file = fopen("in.dat", "r");
    out_file = fopen("out.dat", "w");
    if (in_file == NULL || out_file == NULL) {
        printf("Error opening files\n");
        return 1;
    }

    // Read the number of files from in.dat
    fscanf(in_file, "%d", &file_count);

    // Allocate memory for files and heap structures
    files = (FileInfo*)malloc(file_count * sizeof(FileInfo));
    if (!files) {
        printf("Memory allocation failed for files\n");
        return 1;
    }

    minHeapInit();

    // Open all the files listed in in.dat
    for (int i = 0; i < file_count; i++) {
        fscanf(in_file, "%s", file_name);
        files[i].fp = fopen(file_name, "r");
        if (files[i].fp == NULL) {
            printf("Error opening file: %s\n", file_name);
            continue; // Proceed to the next file
        }
        files[i].current_string = (char*)malloc(51 * sizeof(char)); // +1 for null terminator
        if (!files[i].current_string) {
            printf("Memory allocation failed for current_string[%d]\n", i);
            return 1;
        }
        if (fscanf(files[i].fp, "%50s", files[i].current_string) == 1) {
            minHeapInsert(i);
        } else {
            free(files[i].current_string);
            files[i].current_string = NULL;
        }
    }
    // Close the in.dat file
    fclose(in_file);

    char* last_string = NULL;
    int count = 0;

    while (N > 0) {
        HeapNode min_node = heapExtractMin();
        if (last_string == NULL || strcmp(min_node.string, last_string) != 0) {
            if (last_string) {
                printf("%s %d\n", last_string, count);
                fprintf(out_file, "%s %d\n", last_string, count);
                fflush(out_file); 
                free(last_string);
            }
            last_string = strdup(min_node.string);
            printf("%s", last_string);
            count = 1;
        } else {
            count++;
        }

        if (fscanf(files[min_node.file_index].fp, "%50s", files[min_node.file_index].current_string) == 1) {
            minHeapChange(min_node.file_index);
        } else {
            fclose(files[min_node.file_index].fp);
            free(files[min_node.file_index].current_string);
            qp[min_node.file_index] = -1;
        }
    }

    if (last_string) {
        fprintf(out_file, "%s %d\n", last_string, count);
        free(last_string);
    }

    fclose(out_file);
    // Free allocated memory
    for (int i = 0; i < file_count; i++) {
        free(files[i].current_string);
    }
    free(heap);
    free(pq);
    free(qp);
    free(files);

    return 0;
}