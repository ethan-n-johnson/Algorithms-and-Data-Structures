#include <stdio.h>
#include <stdlib.h>

void print_header()
{
    printf("Make a selection:\n");
    printf("0: Terminate execution\n");
    printf("1: Print the counters in ascending index value order\n");
    printf("2: Print the counters in ascending counter value order\n");
    printf("3: Add one to the counter indexed by i\n");
    printf("4: Subtract one from the counter indexed by i\n");
    printf("5: Determine the number of counters whose values are no smaller than i and no larger than j\n");
    printf("> ");
}
int main(){
    int n, selection;
    printf("Enter the size of the map: ");
    scanf("%d", &n);
    int *map = (int *)malloc(n*sizeof(int));
    int *index = (int *)malloc(n*sizeof(int));
    int *count = (int *)malloc(n*sizeof(int));
    printf("\n");
    // Fill the map index and count arrays
    for (int i = 0; i < n; i++){
        map[i] = i;
        index[i] = i;
        count[i] = 0;
    }
    do
    {
        int i, j = 0;
        print_header();
        scanf("%d", &selection);
        switch (selection)
        {
        case 0:
            // Exit
            break;
        case 1:
            // Ascending index
            for (int i = 0; i < n; i++)
            {
                printf("%d %d\n", i, count[map[i]]);
            }
            break;
        case 2:
            // Ascending counter
            for (int i = 0; i < n; i++)
            {
                printf("%d %d\n", index[i], count[i]);
            }
            break;
        case 3:
            printf("Enter an index: ");
            scanf("%d", &i);
            // add one to the counter indexed by i
            break;
        case 4:
            printf("Enter an index: ");
            scanf("%d", &i);
            // subtract one to the counter indexed by i
            break;
        case 5:
            printf("Enter top and bottom bounds (i j): ");
            scanf("%d %d", &i, &j);
            // determine the number of counters whose values are no smaller than i and no larger than j
            break;
        default:
            break;
        }
    } while (selection != 0);
    
    free(map);
    free(index);
    free(count);
}