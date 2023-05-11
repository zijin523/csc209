#include <stdio.h>
#include <stdlib.h>

/* Return a pointer to an array of two dynamically allocated arrays of ints.
   The first array contains the elements of the input array s that are
   at even indices.  The second array contains the elements of the input
   array s that are at odd indices.

   Do not allocate any more memory than necessary.
*/
int **split_array(const int *s, int length) {
  int odd = length/2;
  int even = length - length/2;
  int odd_index = 0;
  int even_index = 0;
  int **result = malloc(sizeof(int*)*length);
  result[0] = malloc(sizeof(int)*even);
  result[1] = malloc(sizeof(int)*odd);
  for(int i = 0; i < length; i ++){
    if(i%2 == 0){
      result[0][odd_index] = s[i];
      odd_index++;
    }
    else{
      result[1][even_index] = s[i];
      even_index++;
    }
  }
  return result;
}

/* Return a pointer to an array of ints with size elements.
   - strs is an array of strings where each element is the string
     representation of an integer.
   - size is the size of the array
 */

int *build_array(char **strs, int size) {
  int* n = malloc(sizeof(int)*size);
  for(int i = 1; i < size; i++){
    n[i - 1] = strtol(strs[i], NULL, 10);
  }
  return n;
}


int main(int argc, char **argv) {
    /* Replace the comments in the next two lines with the appropriate
       arguments.  Do not add any additional lines of code to the main
       function or make other changes.
     */
    int *full_array = build_array(argv, argc);
    int **result = split_array(full_array, argc);

    printf("Original array:\n");
    for (int i = 0; i < argc - 1; i++) {
        printf("%d ", full_array[i]);
    }
    printf("\n");

    printf("result[0]:\n");
    for (int i = 0; i < argc / 2; i++) {
        printf("%d ", result[0][i]);
    }
    printf("\n");

    printf("result[1]:\n");
    for (int i = 0; i < (argc - 1) / 2; i++) {
        printf("%d ", result[1][i]);
    }
    printf("\n");
    free(full_array);
    free(result[0]);
    free(result[1]);
    free(result);
    return 0;
}
