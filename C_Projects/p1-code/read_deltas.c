#include <stdlib.h>   // require for NULL
#include <stdio.h>    // for built in functions
#include "deltas.h"   // function prototypes
#include <sys/stat.h> // declares stat()

int *read_text_deltas(char *fname, int *len)
{
  *len = 0;
  FILE *fp;
  fp = fopen(fname,"r");
  int integer;
  if(fp ==NULL){
    *len = -1;
    return NULL;
  }
  while(fscanf(fp,"%d", &integer)!=EOF){
    (*len) = (*len) +1;
  }
  if ((*len)==0){
    *len = -1;
    return NULL;
  }
  rewind(fp);
  int *buffer = malloc((*len)*sizeof(int));                   // allocate space for ints*how long the array will be
  for (int i = 0; i < (*len); i++)
    {
        fscanf(fp, "%d", &buffer[i]);
        if (i>=1){                                            // compute delta numbers from 2nd number onwards.
          buffer[i]= buffer[i] + buffer[i-1];
        }
    }
  fclose(fp);
  return buffer;  //pointer to array of ints
}

int *read_int_deltas(char *fname, int *len)
{
  struct stat sb;                                    // struct to hold
  int result = stat(fname, &sb);                     // unix system call to determine size of named file
  if(result==-1 || sb.st_size < sizeof(int)){        // if something went wrong or bail if file is too small
    *len = -1;
    return NULL;
  }
  int total_bytes = sb.st_size;                      // size of file in bytes
  *len = total_bytes/ 4;                             // we can divide by 4 since ints are 4 bytes
  int *buffer = malloc((*len)*sizeof(int));
  FILE *fp;
  fp = fopen(fname, "rb");
  if (fp == NULL){                                   // file failed to open, or is empty
    *len = -1;
    return NULL;
  }
  for (int i =0; i< (*len); i++){
    fread(&buffer[i], sizeof(int), 1, fp);
    if (i>=1){
      buffer[i]= buffer[i] + buffer[i-1];            // compute delta numbers from 2nd number onwards.
    }
  }
  fclose(fp);
  return buffer;
}
