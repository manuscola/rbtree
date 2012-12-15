#include"rbtree.h"
#include <stdio.h>
#include <stdlib.h>
#include<assert.h>

#define SIZE 1600

void padding ( char ch, int n )
{
  int i;

  for ( i = 0; i < n; i++ )
    putchar ( ch );
}

void print_node ( struct rbtree_node *root, int level )
{

  if ( root == NULL ) 
  {
    padding ( '\t', level );
    puts ( "NIL" );

  }
  else 
  {
    print_node ( root->right, level + 1 );
    padding ( '\t', level );
    if(root->color == RB_BLACK)
    {
        printf ( "(%llu)\n", root->key );
    }
    else
       printf ( "%llu\n", root->key );
    print_node ( root->left, level + 1 );
  }
}

void print_tree(struct rbtree* tree)
{
    print_node(tree->root,0);
    printf("-------------------------------------------\n");
}
int main()
{
    struct rbtree* tree = rbtree_init();
    if(tree == NULL)
    {
    	fprintf(stderr,"malloc tree failed\n");
    	return -1;
    }

    int i = 0;
    int array[SIZE] = {0};
    for(i = 0;i<SIZE;i++)
    {
        array[i] = rand()%100000;
    	unsigned long long key = array[i];
        int ret  = rbtree_insert(tree,key,&array[i]);//-1 mean alloc node failed, 
                                                     //-2 mean existed node with same key
       // print_tree(tree);
        void * data = rbtree_lookup(tree,key);
        if(ret == 0)
            assert(data == &array[i]);
    }
    


    for(i = 0; i <SIZE;i+=2)
    {
        rbtree_remove(tree,array[i]);
        //print_tree(tree);
    }
    return 0;
}


