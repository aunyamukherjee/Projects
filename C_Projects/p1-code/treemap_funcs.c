// problem 3
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "treemap.h"


void treemap_init(treemap_t *tree)
{
  tree->root = NULL;
  tree->size = 0;
  return;
}

int treemap_add(treemap_t *tree, char key[], char val[])
{
  if (tree->root==NULL)       // if tree is empty, create root node
  {
    node_t *node = (node_t*)malloc(sizeof(node_t));
    strcpy(node->key,key);
    strcpy(node->val,val);
    node->left = NULL;
    node->right = NULL;
    tree->root = node;
    tree->size =1;
    return 1;
  }
  node_t *pointer = tree->root;
  while (pointer->key != NULL)      // if not empty, find insertion spot by comparing strings
  {
    if (strcmp(pointer->key,key)<0)
    {
      if (pointer->right ==NULL)
      {
          pointer->right = malloc(sizeof(node_t));
	        strcpy((pointer->right)->key,key);
  	      strcpy((pointer->right)->val,val);
  	      (pointer->right)->left = NULL;
  	      (pointer->right)->right = NULL;
  	      tree->size +=1;
  	      return 1;
      }
      pointer = pointer->right;
    }
    else if (strcmp(pointer->key,key)>0)
    {
      if (pointer->left ==NULL)
      {
        pointer->left = malloc(sizeof(node_t));
    	  strcpy((pointer->left)->key,key);
      	strcpy((pointer->left)->val,val);
      	(pointer->left)->left = NULL;
      	(pointer->left)->right = NULL;
      	tree->size +=1;
      	return 1;
      }
      pointer = pointer->left;
    }
    else
    {
      strcpy(pointer->val, val);    // if the strings are equal, update the value slot
      return 0;
    }
  }
  return 0;
}


char *treemap_get(treemap_t *tree, char key[])
{
  node_t *pointer = tree->root;
  while(pointer->key != NULL)       // loop through the tree to find the given key
  {
    if (strcmp(pointer->key,key)<0)
    {
      pointer = pointer->right;
    }
    else if(strcmp(pointer->key,key)>0)
    {
      pointer = pointer->left;
    }
    else
    {
      return pointer->val;        // key found, return it
    }
  }
  return NULL;                  // key missing
}

void treemap_clear(treemap_t *tree)
{
  node_remove_all(tree->root);    // calls recursive function to remove all nodes from tree
  tree->size = 0;
  tree->root = NULL;
}

void node_remove_all(node_t *cur)
{
  if (cur == NULL)
  {
    return;
  }
  node_remove_all(cur->left);
  node_remove_all(cur->right);
  free(cur);
}

void treemap_print_revorder(treemap_t *tree)
{
  node_print_revorder(tree->root, (int)0);      //calls recursive function to print nodes in reverse order
  return;
}

void node_print_revorder(node_t *cur, int indent)
{
  if (cur == NULL)
  {
    return;
  }
  node_print_revorder(cur->right,indent+2);     // go as deep into the tree as you can to the right, while adding 2 indents per level
  for (int i =0; i<indent; i++)
  {
    printf(" ");
  }
  printf("%s -> %s\n", cur->key, cur->val);     //print information with proper indentation
  node_print_revorder(cur->left,indent+2);      // recurse over further left nodes
}

void node_print_preorder(node_t *cur, int depth)
{
  if (cur == NULL)
  {
    return;
  }
  for (int i =0; i<depth; i++)          // works similarily to function above, but in preorder traversal
  {
    printf("  ");
  }
  printf("%s %s\n", cur->key, cur->val);
  node_print_preorder(cur->left,depth+1);
  node_print_preorder(cur->right,depth+1);
}

void treemap_print_preorder(treemap_t *tree)
{
  node_print_preorder(tree->root,0);
  return;
}

void treemap_save(treemap_t *tree, char *fname)
{
  FILE *out = fopen(fname,"w");     //create a file to write into
  if (out== NULL)
  {
    return;                         // if error occurs during fopen, return NULL
  }
  node_write_preorder(tree->root,out,0);    // call recursive function to write into the file, and then close
  fclose(out);
  return;
}

void node_write_preorder(node_t *cur, FILE *out, int depth)
{
  node_t *pointer = cur;

  if (pointer == NULL)
  {
    return;
  }
  for(int i = 0; i <depth; i++)
  {
    fprintf(out,"  ");
  }
  fprintf(out,"%s %s\n", pointer->key, pointer->val);   // write into file each node in preorder
  node_write_preorder(pointer->left,out,depth+1);
  node_write_preorder(pointer->right,out,depth+1);
}

int treemap_load(treemap_t *tree, char *fname )
{
  char key[128];
  char val[128];
  int success=0;
  FILE *fin;
  fin = fopen(fname,"r");
  if (fin == NULL)
  {
	printf("ERROR: could not open file '%s'\n", fname);
    return 0;
  }
  treemap_clear(tree);
  while(success!=EOF)
  {
    success = fscanf(fin,"%s",key);       //read key and value slots from file and add them into the tree using the add method
    success = fscanf(fin,"%s",val);
    treemap_add(tree,key,val);
  }
  fclose(fin);
  return 1;
}
