#include"rbtree.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#define SIZE 1600000

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
            printf ( "(%llu)\n", *(unsigned long long*)(root->key) );
        }
        else
            printf ( "%llu\n",*(unsigned long long*)(root->key) );
        print_node ( root->left, level + 1 );
    }
}

void print_tree(struct rbtree* tree)
{
    print_node(tree->root,0);
    printf("-------------------------------------------\n");
}

int compare(void* key_a,void* key_b)
{
    unsigned long long key_a_real = *(unsigned long long*) (key_a);
    unsigned long long key_b_real = *(unsigned long long*) (key_b);
    if(key_a_real > key_b_real)
    {
        return 1;
    }
    else if(key_a_real == key_b_real)
    {
       return 0;
    }
    else
        return -1;
}
int main()
{
    struct rbtree* tree = rbtree_init(compare);
    int ret = 0;
    if(tree == NULL)
    {
        fprintf(stderr,"malloc tree failed\n");
        return -1;
    }

    int i = 0;
    unsigned long long  * array = malloc(SIZE*sizeof(unsigned long long ));
    if(array == NULL)
    {
        fprintf(stderr,"malloc failed\n");
        return -1;
    }
    srand(time(NULL));
    for(i = 0;i<SIZE;i++)
    {
        array[i] = rand();
        ret  = rbtree_insert(tree,&array[i],&array[i]);//-1 mean alloc node failed, 
                                                     //-2 mean existed node with same key
  //         print_tree(tree);
        void * data = rbtree_lookup(tree,&array[i]);
        if(ret == 0)
            assert(data == &array[i]);
       /* else
        {
            fprintf(stderr," %d - %llu, same node \n",i,array[i]);
        }*/
    }



    for(i = 0; i <SIZE;i+=2)
    {
        ret = rbtree_remove(tree,&array[i]);
        /*if(ret != 0)
        {
            fprintf(stderr,"remove failed (%d) %llu\n",i,array[i]);
            return -2;
        }*/
    //    print_tree(tree);
    }
    return 0;
}


