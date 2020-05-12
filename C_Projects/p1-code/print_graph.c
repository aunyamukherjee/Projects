#include <stdlib.h>
#include <stdio.h>
#include <time.h>

void print_graph(int *data, int len, int max_height){
  int i =0;
  int j =0;
  int min = data[0];
  int max = data[0];
  for (i = 1; i < len; i++){		// setting the min and max of the data list
      if (min > data[i]){
          min = data[i];
      }
      else if (max < data[i]){
          max = data[i];
      }
  }
  int range = max - min;
  double units_per_height = (double)range / max_height;
  printf("length: %d\n", len);
  printf("min: %d\n", min);
  printf("max: %d\n", max);
  printf("range: %d\n", range);
  printf("max_height: %d\n", max_height);
  printf("units_per_height: %.2lf\n", units_per_height);
  printf("%5c", ' '); //line breaks and spaces to make output cleaner
  for(i=0; i<len; i++){
      if(i%5==0){     // if i is divisible by 5
          printf("+");
      }
      else{
          printf("-");
      }
  }
  printf("\n");       // line break
  for (double level = max_height; level >= 0; level -= 1){
      int cutoff = (int)(min+level*units_per_height);
      printf("%3d |", cutoff);    // print up to 4 digits of (int)val or print spaces and then (int)val, so the y axis is spaced well
      for (j = 0; j < len; j++){
          if (data[j] >= cutoff){
              printf("X");
          }
          else{
              printf(" ");
          }
      }
      printf("\n");
  }
  printf("%4c", ' ');     // setting up to do the bottom x-axis
  for(i=0; i<len; i++){
      if(i%5==0){
          printf("+");
      }
      else{
          printf("-");
      }
  }
  printf("\n%4c", ' ');
  for(i=0; i<len; i+=5){
      printf("%-5d", i); // setting the scale for the bottom x axis
  }
  printf("\n");
}
