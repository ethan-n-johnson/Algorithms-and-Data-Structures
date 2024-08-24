#include <stdio.h>
#include <stdlib.h>

typedef struct{
    int value;
    int initial_position;
} Pair;

// Print the array
void print_array(Pair *array, int length){
    for (int i = 0; i < length; i++){
        printf("%d ", array[i].value);
        printf("%d\n", array[i].initial_position);
    }
    printf("\n");
}

// Compares two pair elements based on their value for use in Qsort
int comp_value(const void* elem1, const void* elem2){
    Pair *pair1 = (Pair *)elem1;
    Pair *pair2 = (Pair *)elem2;
    if (pair1->initial_position == pair2->initial_position) return pair1->initial_position - pair2->initial_position;
    else return pair1->value - pair2->value;
}
// Compares two pair elements based on their postition for use in Qsort
int comp_position(const void* elem1, const void* elem2){
    Pair *pair1 = (Pair *)elem1;
    Pair *pair2 = (Pair *)elem2;
    return pair1->initial_position - pair2->initial_position;
}

// Keep only the last occurence of a duplicate
int remove_dupe(Pair* array, int length){
    int new_length = length;
    Pair temp;
    for (int i = 0; i < length; i++) {
        if (array[i].value == array[i+1].value){
            array[i] = array[i+1];
            temp = array[length-i];
            new_length--;
        }
    }
    return new_length;
}

int main(){
    int array_length;
    printf("Enter the size of the array: ");
    scanf("%d", &array_length);
    Pair array[array_length];
    printf("\n");
    // scan in the values for the array
    for (int i = 0; i < array_length; i++){
        scanf("%d", &array[i].value);
        array[i].initial_position = i;
    }
    printf("\n");
    printf("Before Sorting:\n");
    print_array(array, array_length);

    qsort(array, array_length, sizeof(Pair), comp_value);
    printf("After Sorting:\n");
    print_array(array, array_length);

    int new_length = remove_dupe(array, array_length); // Implement dupe removal
    printf("Remove Extras:\n");
    print_array(array, new_length);

    // qsort(array, new_length, sizeof(Pair), comp_position);
    // printf("After Sorting:\n");
    // print_array(array, new_length);
}