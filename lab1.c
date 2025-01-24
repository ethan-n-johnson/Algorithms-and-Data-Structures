// Ethan Johnson
// gcc lab1.c -std=c99

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
  int i, key, top, bottom = 0;
  int temp_count_i, temp_index_i, temp_map_i, temp_count_key, temp_index_key, temp_map_key = 0;
  do
  {
    scanf("%d", &selection);
    switch (selection)
    {
    case 0:
      // Exit
      break;
    case 1:
      // Print by ascending index
      printf("print by index\n");
      for (i = 0; i < n; i++)
        printf("%d %d\n", i, count[map[i]]);
      printf("-------\n");
      break;
    case 2:
      // Print by ascending counter
      printf("print by count\n");
      for (i = 0; i < n; i++)
        printf("%d %d\n", index[i], count[i]);
      printf("-------\n");
      break;
    case 3:
      scanf("%d", &i);
      key = binSearchLast(count, n, count[map[i]]);
      // Add one to the counter indexed by i
      count[map[i]]++;
      // Reorder the arrays
      temp_index_i = index[map[i]];
      temp_count_i = count[map[i]];
      temp_map_i = map[i];
      temp_index_key = index[key];
      temp_count_key = count[key];
      temp_map_key = map[index[key]];

      index[temp_map_i] = index[key];
      index[key] = temp_index_i;

      count[temp_map_i] = count[key];
      count[key] = temp_count_i;

      map[i] = map[temp_index_key];
      map[temp_index_key] = temp_map_i;
      break;
    case 4:
      scanf("%d", &i);
      key = binSearchFirst(count, n, count[map[i]]);
      // Subtract one to the counter indexed by i
      count[map[i]]--;
      // Reorder the arrays
      temp_index_i = index[map[i]];
      temp_count_i = count[map[i]];
      temp_map_i = map[i];
      temp_index_key = index[key];
      temp_count_key = count[key];
      temp_map_key = map[index[key]];

      index[temp_map_i] = index[key];
      index[key] = temp_index_i;

      count[temp_map_i] = count[key];
      count[key] = temp_count_i;

      map[i] = map[temp_index_key];
      map[temp_index_key] = temp_map_i;
      break;

    case 5:
      // Determine the number of counters whose values are no smaller than i and no larger than j
      scanf("%d %d", &bottom, &top);
      int higher = binSearchLast(count, n, top);
      int lower = binSearchFirst(count, n, bottom);
      int in_range = higher - lower + 1;
      printf("%d counters valued between %d and %d\n", in_range, bottom, top);
      break;
    default:
      break;
    }
  } while (selection != 0);

  free(map);
  free(index);
  free(count);
}