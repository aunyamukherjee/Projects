#include <stdlib.h>
#include <stdio.h>
#include <error.h>
#include <string.h>
#include <time.h>
#include "search.h"


int main(int argc, char *argv[]){

  if(argc < 4){
    printf("not enough arguments!!");
    return 0;                
  }

  int do_linear_array = 0;
  int do_linked_list  = 0;
  int do_binary  = 0;
  int do_tree  = 0;

  if(argc == 5){
    char *algs_string = argv[4];
    for(long i=0; i < strlen(algs_string); i++){
      if(algs_string[i] == 'a'){
        do_linear_array = 1;
      }
      if(algs_string[i] == 'b'){
        do_binary = 1;
      }
      if(algs_string[i] == 'l'){
        do_linked_list = 1;
      }
      if(algs_string[i] == 't'){
        do_tree = 1;
      }   
    }
  }

  else{
    do_linear_array = 1;
    do_linked_list  = 1;
    do_binary  = 1;
    do_tree  = 1;
  }

  printf("%8s ","LENGTH");
  printf("%8s ","SEARCHES");
  if(do_linear_array){
    printf("%10s ","array");
  }
  if(do_linked_list){
    printf("%10s ","list");
  }
  if(do_binary){
    printf("%10s ","binary");
  }
  if(do_tree){
    printf("%10s ","tree");
  }
  printf("\n");
  int min_pow = atoi(argv[1]);
  int max_pow = atoi(argv[2]);
  int repeats = atoi(argv[3]);
  int cur_search_size = 2;
  
  for(int i = 1; i < min_pow; i++){
    cur_search_size = cur_search_size *2;
  }

  for(int k = min_pow; k <= max_pow; k++){ 

    printf("%8d ", cur_search_size);
    printf("%8d ", cur_search_size*2*repeats); 
  
    if(do_linear_array){
      int *array = make_evens_array(cur_search_size);
      clock_t begin, end;
      begin = clock(); 
      for(int cur_reps = 0; cur_reps < repeats; cur_reps++){
        for(int i = 0; i< 2*cur_search_size; i++){
          linear_array_search(array, cur_search_size, i);
        }
      }
      end = clock();
      double runtime = ((double) (end - begin)) / CLOCKS_PER_SEC;
      printf("%10.4e ", runtime);
      free(array);
    }

    if(do_linked_list){
      list_t *list = make_evens_list(cur_search_size);
      clock_t begin, end;
      begin = clock();
      for(int cur_reps = 0; cur_reps < repeats; cur_reps++){
        for(int i = 0; i< 2*cur_search_size; i++){
          linkedlist_search(list, cur_search_size, i);
        }
      }
      end = clock();
      double runtime = ((double) (end - begin)) / CLOCKS_PER_SEC;
      printf("%10.4e ", runtime);
      list_free(list);
    }

    if(do_binary){
      int *barray = make_evens_array(cur_search_size);
      clock_t begin, end;
      begin = clock(); 
      for(int cur_reps = 0; cur_reps < repeats; cur_reps++){
        for(int i = 0; i< 2*cur_search_size; i++){
          binary_array_search(barray, cur_search_size, i);
        }
      }
      end = clock();
      double runtime = ((double) (end - begin)) / CLOCKS_PER_SEC;
      printf("%10.4e ", runtime);
      free(barray);
    }

    if(do_tree){
      bst_t *tree = make_evens_tree(cur_search_size);
      clock_t begin, end;
      begin = clock();
      for(int cur_reps = 0; cur_reps < repeats; cur_reps++){
        for(int i = 0; i< 2*cur_search_size; i++){
          binary_tree_search(tree, cur_search_size, i);
        }
      }
      end = clock();
      double runtime = ((double) (end - begin)) / CLOCKS_PER_SEC;
      printf("%10.4e ", runtime);
      bst_free(tree);
    }
    cur_search_size = cur_search_size *2;  
    printf("\n"); 
  }

}
