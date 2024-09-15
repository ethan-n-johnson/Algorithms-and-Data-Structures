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

typedef char* Item;

int N,         // Number of items in queue
    *pq,       // Priority queue
    *qp,       // Table of handles (for tracking)
    maxQueued; // Capacity of priority queue
char **a;

int parent(int i)
{
    return i / 2;
}

int left(int i)
{
    return 2 * i;
}

int right(int i)
{
    return 2 * i + 1;
}

void exch(int i, int j)
{
    // Swaps parent with child in heap
    int t;

    t = pq[i];
    pq[i] = pq[j];
    pq[j] = t;
    qp[pq[i]] = i;
    qp[pq[j]] = j;
}

// Changed in maxheapInit to minHeapInit
// Changed int *items to char *items[] to support strings
void minHeapInit(char *items[], int n, int m)
{
    int i;

    a = items; // Save reference to index table
    maxQueued = m;
    N = 0;
    pq = (int *)malloc((maxQueued + 1) * sizeof(int)); // Contains subscripts to a[]
    if (!pq)
    {
        printf("malloc failed %d\n", __LINE__);
        exit(0);
    }
    qp = (int *)malloc(n * sizeof(int)); // Inverse of pq, allows changing priorities
    if (!qp)
    {
        printf("malloc failed %d\n", __LINE__);
        exit(0);
    }
    // Set all handles to unused
    for (i = 0; i < n; i++)
        qp[i] = (-1);
}

// Function name changed to minheapEmpty
int minHeapEmpty()
{
    return !N;
}

// Function name changed to minheapFull
int minHeapFull()
{
    return N == maxQueued;
}

int less(int i, int j)
{
    // Notice how heap entries reference a[]
    // Changed the return to support strings as the priority
    return strcmp(a[pq[i]], a[pq[j]]) > 0;
}

void heapIncreaseKey(int *pq, int k) // AKA swim
{
    while (k > 1 && less(parent(k), k))
    {
        exch(k, parent(k));
        k = parent(k);
    }
}

// Slight changes to support min heap instead of max heap
void minHeapify(int *pq, int k, int N) // AKA sink
{
    int j;

    while (left(k) <= N)
    {
        j = left(k);
        // Changed j, j+1 to j+1, j
        if (j < N && less(j + 1, j))
            j = right(k);
        // Changed !less(k, j) to !less(j, k)
        if (!less(j, k))
            break;
        exch(k, j);
        k = j;
    }
}

// Function name changed to minHeapInsert
void minHeapInsert(int k)
{
    qp[k] = ++N;
    pq[N] = k;
    heapIncreaseKey(pq, N);
}

// Function name changed to heapExtractMin
int heapExtractMin()
{
    exch(1, N);
    minHeapify(pq, 1, --N);
    qp[pq[N + 1]] = (-1); // Set to unused
    return pq[N + 1];
}

// Function name changed to minHeapChange
void minHeapChange(int k)
{ // To be called when priority[k] has changed.
    heapIncreaseKey(pq, qp[k]);
    minHeapify(pq, qp[k], N);
}

// Test driver for index-heap-based priority queue.
// Index is just a table of priorities whose
// subscripts are used in the PQ.

int main()
{
    // From Heap sort code
    // Read the first string from each file into a heap
    // Remove the first string from the heap and increase the counter
    // Read in another string from the file into the heap
    // Table of heap indexes (file numbers)
    // Use the heap index to access the correct file from the array
    // The only duplicates that occur are from different files
    int n, m, op, i, val, file_count;
    char file_name[100];
    FILE* in_file;

    in_file = fopen("in.dat", "r");
    if (in_file == NULL)
    {
        printf("error opening in.dat\n Exiting...");
        return 1;
    }

    // Read the number of files from in.dat
    fscanf(in_file, "%d", &file_count);

    // Initialize the array of pointers to files
    FILE* value_files[file_count];

    // Open all the files listed in in.dat
    for (int i = 0; i < file_count; i++)
    {
        fscanf(in_file, "%s", file_name);
        value_files[i] = fopen(file_name, "r");
        if (value_files[i] == NULL) {
            printf("Error opening file: %s\n", file_name);
        }
    }
    fopen("out.dat", "w+");

    // Close the in.dat file
    fclose(in_file);
    
//     int *priority;

//     printf("Enter table (dictionary) size\n");
//     scanf("%d", &n);
//     if (n < 1)
//     {
//         printf("Illegal size %d\n", __LINE__);
//         exit(0);
//     }
//     priority = (int *)malloc(n * sizeof(int));
//     if (!priority)
//     {
//         printf("malloc failed for table %d\n", __LINE__);
//         exit(0);
//     }
//     printf("Your table has keys %d through %d\n", 0, n - 1);
//     printf("Enter PQ (binary heap) size\n");
//     scanf("%d", &m);

//     maxHeapInit(priority, n, m);

//     for (i = 0; i < n; i++)
//         priority[i] = (-1);

//     while (1)
//     {
//         printf("----table (priority) active entries----\n");
//         for (i = 0; i < n; i++)
//             printf("%d %d\n", i, priority[i]);
//         printf("----heap (pq)-----\n");
//         for (i = 1; i <= N; i++)
//             printf("%d %d\n", i, pq[i]);
//         printf("----handles (qp)-----\n");
//         for (i = 0; i < n; i++)
//             printf("%d %d\n", i, qp[i]);

//         printf("Enter operation (<0 to quit, # of table item, >%d for heapExtractMax)\n", n - 1);
//         scanf("%d", &op);
//         if (op < 0)
//             break;
//         if (op >= n)
//             if (maxHeapEmpty())
//                 printf("Can't do heapExtractMax() when pq is empty\n");
//             else
//             {
//                 i = heapExtractMax();
//                 printf("heapExtractMax() indicates priority[%d]=%d\n", i, priority[i]);
//                 priority[i] = (-1);
//             }
//         else
//         {
//             if (priority[op] < 0)
//             {
//                 if (maxHeapFull())
//                     printf("pq already full\n");
//                 else
//                 {
//                     val = (-1);
//                     while (val < 0)
//                     {
//                         printf("Enter non-negative priority to maxHeapInsert() at table slot %d\n",
//                                op);
//                         scanf("%d", &val);
//                     }
//                     priority[op] = val;
//                     maxHeapInsert(op);
//                 }
//             }
//             else
//             {
//                 val = (-1);
//                 while (val < 0)
//                 {
//                     printf("Enter non-negative priority for maxHeapChange() of table slot %d\n",
//                            op);
//                     scanf("%d", &val);
//                 }
//                 priority[op] = val;
//                 maxHeapChange(op);
//             }
//         }
//     }
    // Close all the value files
    for (int i = 0; i < file_count; i++)
    {
        if (value_files[i] != NULL) {
            fclose(value_files[i]);
        }
    }
//     free(priority);
//     free(pq);
//     free(qp);
}