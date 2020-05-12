// optimized versions of matrix diagonal summing
#include "matvec.h"


int sumdiag_VER1(matrix_t mat, vector_t vec) {
  if(vec.len != (mat.rows + mat.cols -1)){
    printf("sumdiag_base: bad sizes\n");
    return 1;
  }
  for(int i=0; i<vec.len; i++){                   // initialize vector of diagonal sums
    VSET(vec,i,0);                                // to all 0s
  }
  
  for(int i = 0; i< mat.rows; i++){
     for(int j = 0; j< mat.cols; j++){
        int element_ij = MGET(mat, i,j);         // retrive an element based on row/col
        int diag_num = j - i + mat.cols - 1;     // calculate the diagonal number for it
        int vec_d = VGET(vec, diag_num);
        VSET(vec, diag_num, element_ij + vec_d);
     }
  }
  return 0;
}

int sumdiag_VER2(matrix_t mat, vector_t vec) {
  if(vec.len != (mat.rows + mat.cols -1)){
    printf("sumdiag_base: bad sizes\n");
    return 1;
  }
  for(int i=0; i<vec.len; i++){                   // initialize vector of diagonal sums
    VSET(vec,i,0);                                // to all 0s
  }
  
  int i,j;
  for(i=0; i<mat.rows; i+=4){ // unroll x4 for pipelining
    //int sum_0 = MGET(mat,i+0,0);  // assumes cols >= 4
    //int sum_1 = MGET(mat,i+1,0);  // deal with 4 columns at a time
    //int sum_2 = MGET(mat,i+2,0);
    //int sum_3 = MGET(mat,i+3,0);
    for(j=1; j<mat.cols; j++){  // iterate over every row
      int x_0 = MGET(mat, i+0,j);         // retrive an element based on row/col
      int d_num0 = j - i + mat.cols - 1;     // calculate the diagonal number for it
      int vec_d0 = VGET(vec, d_num0);
      VSET(vec, d_num0, x_0 + vec_d0);
      
      int x_1 = MGET(mat, i+1,j);         // retrive an element based on row/col
      int d_num1 = j - i+1 + mat.cols - 1;     // calculate the diagonal number for it
      int vec_d1 = VGET(vec, d_num1);
      VSET(vec, d_num1, x_1 + vec_d1);

      int x_2 = MGET(mat, i+2,j);         // retrive an element based on row/col
      int d_num2 = j - i+2 + mat.cols - 1;     // calculate the diagonal number for it
      int vec_d2 = VGET(vec, d_num2);
      VSET(vec, d_num2, x_2 + vec_d2);

      int x_3 = MGET(mat, i+3,j);         // retrive an element based on row/col
      int d_num3 = j - i+3 + mat.cols - 1;     // calculate the diagonal number for it
      int vec_d3 = VGET(vec, d_num3);
      VSET(vec, d_num3, x_3 + vec_d3);

    }

  }
  for(; i< mat.rows; i++){   //cleanup
     for(j = 1; j< mat.cols; j++){
        int element_ij = MGET(mat, i,j);        
        int diag_num = j - i + mat.cols - 1;     
        int vec_d = VGET(vec, diag_num);
        VSET(vec, diag_num, element_ij + vec_d);
     }
  }
  return 0;
}

int sumdiag_OPTM(matrix_t mat, vector_t vec) {
  // call your preferred version of the function
  return sumdiag_VER1(mat, vec);
}
