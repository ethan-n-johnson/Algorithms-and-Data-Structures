// Ethan Johnson
// ID: 1002008374
// gcc lab3.c -std=c99

#include <stdio.h>
#include <stdlib.h>

// Function to read user input updated to support two targets instead of one in the example code
void readInput(int* n, int* m1, int* m2, int** S, int*** C)
{
    int i;

    scanf("%d", n);
    
    scanf("%d %d", m1, m2);

    *S = (int*) malloc((*n + 1)*sizeof(int));
    *C = (int**) malloc((*m1 + 1)*sizeof(int*));
    if (!(*C) || !(*S))
    {
        printf("malloc failed %d\n", __LINE__);
        exit(0);
    }
    for (i = 0; i <= *m1; i++)
    {
        (*C)[i] = (int*) malloc((*m2 + 1)*sizeof(int));
        if (!(*C)[i])
        {
            printf("malloc failed %d\n", __LINE__);
            exit(0);
        }
    }

    (*S)[0] = 0;  // Sentinel zero
    for (i = 1; i <= *n; i++)
    {
        scanf("%d", &(*S)[i]);
    }
}

// Calculate the subsetSum with two targets
void subsetSum(int n, int m1, int m2, int* S, int** C)
{
    int i, j, row, col, leftover;
    // Initialize the DP table
    C[0][0] = 0;
    // Initialize the first column 
    for (i = 1; i <= m2; i++)
    {
        for (j = 0; j <= n; j++)
        {
            leftover = i - S[j];
            if (leftover >= 0 &&
                C[0][leftover] < j)
                break;
        }
        C[0][i] = j;
    }
    // Initialize the first row
    for (i = 1; i <= m1; i++)
    {
        for (j = 0; j <= n; j++)
        {
            leftover = i - S[j];             // To be achieved with other values
            if (leftover >= 0 &&               // Possible to have a solution
                C[leftover][0] < j)          // Indices are included in
                break;                         // ascending order.
        }
        C[i][0] = j;
    }
    // Fill the DP table
    for (row = 1; row <= m1; row++)
    {
        for (col = 1; col <= m2; col++)
        {
            for (i = 1; i <= n; i++)
            {
                leftover = row - S[i];             // To be achieved with other values
                if (leftover >= 0 &&               // Possible to have a solution
                    C[leftover][col] < i)          // Indices are included in
                    break;                         // ascending order.
                leftover = col - S[i];             // To be achieved with other values
                if (leftover >= 0 &&               // Possible to have a solution
                    C[row][leftover] < i)          // Indices are included in
                    break;                         // ascending order.
            }
            C[row][col] = i;
        }
    }
}

void isSubsetSumStable(int n, int m1, int m2, int* S, int** C)
{
    int i, j, row, col, leftover;
    // Initialize the DP table
    if (C[0][0] != 0)
      printf("C[0][0] is %d\n",C[0][0]);
    // Initialize the first row 
    for (i = 1; i <= m2; i++)
    {
        for (j = 0; j <= n; j++)
        {
            leftover = i - S[j];
            if (leftover >= 0 &&
                C[0][leftover] < j)
                break;
        }
        if (C[0][i] != j)
          printf("C[0][%d] is %d, should be %d!!!\n",i,C[0][i],j);
    }
    // Initialize the first column
    for (i = 1; i <= m1; i++)
    {
        for (j = 0; j <= n; j++)
        {
            leftover = i - S[j];             // To be achieved with other values
            if (leftover >= 0 &&               // Possible to have a solution
                C[leftover][0] < j)          // Indices are included in
                break;                         // ascending order.
        }
        if (C[i][0] != j)
          printf("C[%d][0] is %d, should be %d!!!\n",i,C[i][0],j);
    }
    // Fill the DP table
    for (row = 1; row <= m1; row++)
    {
        for (col = 1; col <= m2; col++)
        {
            for (i = 1; i <= n; i++)
            {
                leftover = row - S[i];             // To be achieved with other values
                if (leftover >= 0 &&               // Possible to have a solution
                    C[leftover][col] < i)          // Indices are included in
                    break;                         // ascending order.
                leftover = col - S[i];             // To be achieved with other values
                if (leftover >= 0 &&               // Possible to have a solution
                    C[row][leftover] < i)          // Indices are included in
                    break;                         // ascending order.
            }
            if (C[row][col] != i)
              printf("C[%d][%d] is %d, should be %d!!!\n",row,col,C[row][col],i);
        }
    }
}


// Solves and prints the backtrace
void writeOutput(int n, int m1, int m2, int *S, int **C)
{
    int right = m2, left = m1;
    int i=0, j=0;
    int solution1[n], solution2[n];
    printf("Targets are %d and %d\n", m1, m2);

    // Output the input set
    printf("  i   S\n");
    printf("-------\n");
    for (i = 1; i <= n; i++)
        printf("%3d %3d\n", i, S[i]);
    
    i=0, j=0;
    if (C[m1][m2] == n+1)
    {
        printf("No solution");
    }
    else
    {
        // Works backwards through the C table to add the backtrace to the solution
        while(left > 0 || right > 0)
        {
            // Backtrace for m1
            if(S[C[left][right]] <= left && C[left-S[C[left][right]]][right] < C[left][right])
            {
                solution1[i] = C[left][right];
                left-=S[C[left][right]];
                i++;
            }
            // Backtrace for m2
            else if (S[C[left][right]] <= right && C[left][right-S[C[left][right]]] < C[left][right])
            {
                solution2[j] = C[left][right];
                right-=S[C[left][right]];
                j++;
            }
            else
            {
    printf("Error!?!?!?\n");
    exit(0);
//                break;
            }
        }
        i--, j--;
        // Print the backtrace for m1
        printf("subsequence for %d:\n", m1);
        for (; i >= 0; i--)
        {
            printf("%3d %3d\n", solution1[i], S[solution1[i]]);
        }
        // Print the backtrace for m2
        printf("subsequence for %d:\n", m2);
        for (; j >= 0; j--)
        {
            printf("%3d %3d\n", solution2[j], S[solution2[j]]);
        }
    }
}

int main()
{
    int n;    // Size of input set
    int m1;   // First target value
    int m2;   // Second target value
    int *S;   // Input set
    int **C;  // Cost table

    readInput(&n, &m1, &m2, &S, &C);
    subsetSum(n, m1, m2, S, C);
    isSubsetSumStable(n, m1, m2, S, C);
    writeOutput(n, m1, m2, S, C);

    // Free allocated memory
    for (int i = 0; i <= m1; i++)
        free(C[i]);
    free(C);
    free(S);

    return 0;
}