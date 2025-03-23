#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int* values;
  size_t length;
  size_t capacity;
} array;

// Check if in array already exist this value 
int is_in_array(array* arr, int value) {
  for (size_t i = 0; i < arr->length; i++) {
	if (arr->values[i] == value)
	  return 1;
  }

  return 0;
}


int replace_minval(array* arr) {
  int minval = arr->values[0];
  size_t minval_index = 0;

  // Find min value
  for (size_t i = 0; i < arr->length; i++) {
	if (minval > arr->values[i]) {
	  minval = arr->values[i];
	  minval_index = i;
	}
  }

  // Replace with two new values
  // Replace current min value with first one no duplicated
  if (!is_in_array(arr, minval * 2 + 1))
	arr->values[minval_index] = minval * 2 + 1;
  else if (!is_in_array(arr, minval * 3 + 1))
	arr->values[minval_index] = minval * 3 + 1;

  // Add second one new variable
  // Check for free space
  if (arr->capacity <= arr->length) {
	arr->capacity *= 2;
	arr->values = (int*)realloc(arr->values, arr->capacity * sizeof(int));
  }

  if (!is_in_array(arr, minval * 3 + 1)) {
	arr->values[arr->length] = minval * 3 + 1;
	arr->length++;
  }

  return minval;
}


int DblLinear(int n) {
  array arr;
  arr.capacity = 2048;
  arr.values = (int*)malloc(sizeof(int) * arr.capacity);
  arr.values[0] = 1;
  arr.length = 1;
	
  
  for (size_t i = 0; i < n; i++) {
	replace_minval(&arr);
  }

  int ans = replace_minval(&arr);
  free(arr.values);
	
  return ans;
}


int main() {
  printf("for n = 10: %d\n", DblLinear(10));
  printf("for n = 50: %d\n", DblLinear(50));
  printf("for n = 100: %d\n", DblLinear(100));
  printf("for n = 500: %d\n", DblLinear(500));
  printf("for n = 1000: %d\n", DblLinear(1000));
  printf("for n = 1000000: %d\n", DblLinear(100000));
  return 0;
}
