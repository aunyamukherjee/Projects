#include <stdio.h>
#include <string.h>
#include "treemap.h"
#include <stdlib.h>


int main(int argc, char *argv[])
{
  int echo = 0;
  if(argc > 1 && strcmp("-echo",argv[1])==0)
  {
    echo=1;
  }
  // the avaiable commands are printed to the screen for the user to choose
  printf("TreeMap Editor\n");
  printf("Commands:\n");
  printf("  quit:           exit the program\n");
  printf("  print:          shows contents of the tree in reverse sorted order\n");
  printf("  add <key> <val>:     inserts the given key/val into the tree, duplicate keys are ignored\n");
  printf("  get <key>:    prints FOUND if the name is in the tree, NOT FOUND otherwise\n");
  printf("  clear:          eliminates all key/vals from the tree\n");
  printf("  preorder:       prints contents of the tree in pre-order which is how it will be saved\n");
  printf("  save <file>:    writes the contents of the tree in pre-order to the given file\n");
  printf("  load <file>:    clears the current tree and loads the one in the given file\n");
  // creating a command string
  char command[128];
  char key[128];
  char val[128];
  char *fname = malloc(sizeof(char)*128);
  treemap_t tree;
  int success = 0;
  //initializig the tree
  treemap_init(&tree);
  // while loop continues until quit command is selected by user
  while(1) // keep going until broken
  {
    printf("TM> ");
    success = fscanf(stdin,"%s",command);
    if(success==EOF)
    {   //break once we get to the end of the file
      printf("\n");
      break;
    }
    // if save is typed in as the command
    if(strcmp("save",command) == 0)
    {
      fscanf(stdin,"%s", fname);
      if(echo)
      {
        printf("save %s\n",fname);
      }
      treemap_save(&tree, fname);
    }
    // if quit is typed in as the command
    else if(strcmp("quit", command)==0 )
    {
      if(echo)
      {               // echo the command to make output more user friendly
        printf("quit\n");
      }
      break;
    }
    // if add *something* *something* is typed in as the command
    else if( strcmp("add", command)==0 )
    {
      fscanf(stdin,"%s", key);
      fscanf(stdin,"%s",val);
      if(echo)
      {
        printf("add %s %s\n",key, val);
      }
      success = treemap_add(&tree, key, val);
      if(!success)
      {
        printf("modified existing key\n");
      }
    }
    // if get *something* is typed in as the command
    else if( strcmp("get", command)==0 )
    {
      fscanf(stdin,"%s",key);
      if(echo)
      {
        printf("get %s\n",key);
      }
      // returns 1 if we found the item, 0 if not
      char* get_success = treemap_get(&tree,key);
      if(get_success == NULL)
      {
        printf("NOT FOUND\n");
      }
      else
      {
        printf("FOUND: %s\n", get_success);
      }
    }
    // if clear is typed into the command
    else if( strcmp("clear", command)==0 )
    {
      if(echo)
      {
        printf("clear\n");
      }
      treemap_clear(&tree);
    }
    // if print is typed in as the command
    else if( strcmp("print", command)==0 )
    {
      if(echo)
      {
        printf("print\n");
      }
      treemap_print_revorder(&tree);
    }
    // if preorder is typed in as the command
    else if(strcmp("preorder",command) == 0)
    {
      if(echo)
      {
        printf("preorder\n");
      }
      treemap_print_preorder(&tree);
    }
    // if load is typed in as the command
    else if(strcmp("load",command) == 0)
    {
      fscanf(stdin,"%s",fname);
      if(echo)
      {
        printf("load %s\n",fname);
      }
      int load_value =treemap_load(&tree, fname);
      if (load_value ==0)
      {
	printf("load failed\n");
	}
    }
    // unknown command
    else
    {
      if(echo)
      {
        printf("%s\n",command);
      }
      printf("Unknown command '%s'\n",command);
    }
  }
  // free the memory taken by the tree and clear the nodes
  free(fname);
  treemap_clear(&tree);
  return 0;
}
