// Ethan Johnson
// ID: 1002008374

#include <stdio.h>
#include <stdlib.h>

int binSearchFirst(int *a, int n, int key)
// Input: int array a[] with n elements in ascending order.
//        int key to find.
// Output: Returns subscript of the first a element >= key.
//         Returns n if key>a[n-1].
// Processing: Binary search.
{
  int low, high, mid;
  low = 0;
  high = n - 1;
  // Subscripts between low and high are in search range.
  // Size of range halves in each iteration.
  // When low>high, low==high+1 and a[high]<key and a[low]>=key.
  while (low <= high)
  {
    mid = (low + high) / 2;
    if (a[mid] < key)
      low = mid + 1;
    else
      high = mid - 1;
  }
  return low;
}

int binSearchLast(int *a, int n, int key)
{
  // Input: int array a[] with n elements in ascending order.
  //        int key to find.
  // Output: Returns subscript of the last a element <= key.
  //         Returns -1 if key<a[0].
  // Processing: Binary search.
  int low, high, mid;
  low = 0;
  high = n - 1;
  // subscripts between low and high are in search range.
  // size of range halves in each iteration.
  // When low>high, low==high+1 and a[high]<=key and a[low]>key.
  while (low <= high)
  {
    mid = (low + high) / 2;
    if (a[mid] <= key)
      low = mid + 1;
    else
      high = mid - 1;
  }
  return high;
}

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
int main()
{
  int n;
  scanf("%d", &n);
  int selection;
  int *map = (int *)malloc(n * sizeof(int));
  int *index = (int *)malloc(n * sizeof(int));
  int *count = (int *)malloc(n * sizeof(int));
  printf("\n");
  // Fill the map index and count arrays
  for (int i = 0; i < n; i++)
  {
    map[i] = i;
    index[i] = i;
    count[i] = 0;
  }
  int i, key, top, bottom, temp = 0;
  do
  {
    print_header();
    scanf("%d", &selection);
    switch (selection)
    {
    case 0:
      // Exit
      break;
    case 1:
      // Ascending index
      for (i = 0; i < n; i++)
        printf("%d %d\n", i, count[map[i]]);
      break;
    case 2:
      // Ascending counter
      for (i = 0; i < n; i++)
        printf("%d %d\n", index[i], count[i]);
      break;
    case 3:
      // Add one to the counter indexed by i
      // Reorder the arrays
      printf("Enter an index: ");
      scanf("%d", &i);
      key = binSearchLast(count, n, count[map[i]]);
      count[map[i]]++;
      printf("key = %d\n", key);
      printf("map[i] = %d\n", map[i]);
      printf("Swap %d with %d", map[i], map[key]);
      temp = index[map[i]];
      index[map[i]] = index[key];
      index[key] = temp;

      temp = count[map[i]];
      count[map[i]] = count[key];
      count[key] = temp;

      temp = map[i];
      map[i] = map[key];
      map[key] = temp;
      break;
    case 4:
      printf("Enter an index: ");
      scanf("%d", &i);
      // subtract one to the counter indexed by i
      break;
    case 5:
      printf("Enter top and bottom bounds (i j): ");
      scanf("%d %d", &top, &bottom);
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