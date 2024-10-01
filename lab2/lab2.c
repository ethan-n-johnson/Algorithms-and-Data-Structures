// Ethan Johnson
// ID: 1002008374
// gcc lab2.c -std=c99

// The following heap sort code has been changed to 
// support minheaps instead of maxheaps. So the names
// of all of the functions have been changed. As well
// as some of their implementations.

// For heap sort code
// index-heap-based priority queue.  heap routines are
// from "Algorithms in C, Third Edition", and
// "Algorithms in Java, Third Edition", Robert Sedgewick

// Summer 2016, changed for CLRS, 3rd ed., but still non-recursive.

// This is a prototype for demonstration purposes only.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _POSIX_C_SOURCE 200809L
#define MAX_STRING_LENGTH 50

// Struct for the string map
typedef struct {
    char *str;
    int fileIndex;
} Item;

int N,          // Number of items in queue
    *pq,        // Priority queue
    *qp,        // Table of handles (for tracking)
    minQueued;  // Capacity of priority queue
Item *a;
FILE **files;
int file_count;

// Implementation of strdup because C99 does not have it
char *strdup(const char *str) {
    size_t len = strlen(str) + 1;      // Calculate the length of the string
    char *dup = malloc(len);           // Allocate memory for the new string
    if (dup == NULL) {
        return NULL;                   // Handle memory allocation failure
    }
    strcpy(dup, str);                  // Copy the original string to the new memory
    return dup;                        // Return the duplicated string
}

int parent(int i) {
    return i / 2;
}

int left(int i) {
    return 2 * i;
}

int right(int i) {
    return 2 * i + 1;
}

void exch(int i, int j) {
// Swaps parent with child in heap
    int t = pq[i];
    pq[i] = pq[j];
    pq[j] = t;
    qp[pq[i]] = i;
    qp[pq[j]] = j;
}

void minHeapInit(Item *items, int n, int m) {
    int i;

    a = items; // Save reference to index table
    minQueued = m;
    N = 0;
    pq = (int*)malloc((minQueued + 1) * sizeof(int)); // Contains subscripts to a[]
    if (!pq)
    {
        printf("malloc failed %d\n",__LINE__);
        exit(0);
    }
    qp = (int*)malloc(n * sizeof(int)); // Inverse of pq, allows changing priorities
    if(!qp)
    {
        printf("malloc failed %d\n",__LINE__);
        exit(0);
    }
    // Set all handles to unused
    for (i=0;i<n;i++)
        qp[i]=(-1);
}

int minHeapEmpty() {
    return !N;
}

int less(int i, int j) {
    // Notice how heap entries reference a[]
    return strcmp(a[pq[i]].str, a[pq[j]].str) > 0;
}

void heapIncreaseKey(int *pq, int k) // AKA swim
{
    while (k > 1 && less(parent(k), k)) {
        exch(k, parent(k));
        k = parent(k);
    }
}

void minHeapify(int *pq, int k, int N) // AKA sink
{
    int j;
    while (left(k) <= N) {
        j = left(k);
        if (j < N && less(j, j + 1))
            j = right(k);
        if (!less(k, j))
            break;
        exch(k, j);
        k = j;
    }
}

void minHeapInsert(int k) {
    qp[k] = ++N;
    pq[N] = k;
    heapIncreaseKey(pq, N);
}

int heapExtractMin() {
    exch(1, N);
    minHeapify(pq, 1, --N);
    qp[pq[N+1]]=(-1);  // Set to unused
    return pq[N + 1];
}

char* readNextString(FILE *file) {
    char *str = malloc(MAX_STRING_LENGTH * sizeof(char));
    if (fscanf(file, "%s", str) == 1) {
        return str;
    }
    free(str);
    return NULL;
}

int main() {
    // Try to open the infile. If it doesn't open return an error.
    FILE *in_file = fopen("in.dat", "r");
    if (!in_file) {
        printf("Error opening info.txt\n");
        return 1;
    }

    fscanf(in_file, "%d", &file_count);

    files = malloc(file_count * sizeof(FILE*));
    char filename[MAX_STRING_LENGTH];

    // Try to open the input files. If one doesn't open free all files and return an error.
    for (int i = 0; i < file_count; i++) {
        fscanf(in_file, "%s", filename);
        files[i] = fopen(filename, "r");
        if (!files[i]) {
            printf("Error opening input file %s\n", filename);
            fclose(in_file);
            for (int j = 0; j < i; j++) {
                fclose(files[j]);
            }
            free(files);
            return 1;
        }
    }
    fclose(in_file);
    // Try to open the outfile. If it doesn't open free all files and return an error.
    FILE *out_file = fopen("out.dat", "w");
    if (!out_file) {
        printf("Error opening out.dat\n");
        for (int i = 0; i < file_count; i++) {
            fclose(files[i]);
        }
        free(files);
        return 1;
    }

    minHeapInit(malloc(file_count * sizeof(Item)), file_count, file_count);

    // Read in the strings create a map
    for (int i = 0; i < file_count; i++) {
        char *str = readNextString(files[i]);
        if (str) {
            a[i].str = str;
            a[i].fileIndex = i;
            minHeapInsert(i);
        }
    }

    char *string_prev = NULL;
    int count = 0;
    
    while (!minHeapEmpty()) {
        int index = heapExtractMin();
        // Compare the previous string with the current string 
        if (string_prev == NULL || strcmp(a[index].str, string_prev) != 0) {
            if (string_prev != NULL) {
                // Print the string to the output file
                fprintf(out_file, "%s %d\n", string_prev, count);
                free(string_prev);
            }
            // Copy the current string to string_prev
            string_prev = strdup(a[index].str);
            count = 1;
        } else {
            count++;
        }
        // Read in the next string and add it to the heap
        char *nextStr = readNextString(files[a[index].fileIndex]);
        if (nextStr) {
            free(a[index].str);
            a[index].str = nextStr;
            minHeapInsert(index);
        }
    }
    // Print the string to the output file
    if (string_prev != NULL) {
        fprintf(out_file, "%s %d\n", string_prev, count);
        free(string_prev);
    }

    // Close all files
    for (int i = 0; i < file_count; i++) {
        fclose(files[i]);
    }
    fclose(out_file);
    free(files);
    free(pq);
    free(qp);
    free(a);

    return 0;
}