// Ethan Johnson
// ID: 1002008374
// gcc

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

#define MAX_STRING_LENGTH 50
#define MAX_FILES 100
#define MAX_FILENAME_LENGTH 50

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
    FILE *in_file = fopen("in.dat", "r");
    if (!in_file) {
        printf("Error opening info.txt\n");
        return 1;
    }

    fscanf(in_file, "%d", &file_count);
    if (file_count <= 0 || file_count > MAX_FILES) {
        printf("Invalid number of files\n");
        fclose(in_file);
        return 1;
    }
    files = malloc(file_count * sizeof(FILE*));
    char filename[MAX_FILENAME_LENGTH];

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
        if (string_prev == NULL || strcmp(a[index].str, string_prev) != 0) {
            if (string_prev != NULL) {
                fprintf(out_file, "%s %d\n", string_prev, count);
                free(string_prev);
            }
            string_prev = strdup(a[index].str);
            count = 1;
        } else {
            count++;
        }

        char *nextStr = readNextString(files[a[index].fileIndex]);
        if (nextStr) {
            free(a[index].str);
            a[index].str = nextStr;
            minHeapInsert(index);
        }
    }

    if (string_prev != NULL) {
        fprintf(out_file, "%s %d\n", string_prev, count);
        free(string_prev);
    }

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