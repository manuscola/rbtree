#include"rbtree.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#define SIZE 12
typedef unsigned long long ULL;
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
            printf ( "(%llu)\n", *(ULL*)(root->key) );
        }
        else
            printf ( "%llu\n",*(ULL*)(root->key) );
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
    ULL key_a_real = *(ULL*) (key_a);
    ULL key_b_real = *(ULL*) (key_b);
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

void  process_null_node(struct rbtree_node* node, int nullcount, FILE* stream)
{
        fprintf(stream, "    null%d [shape=hexagon];\n", nullcount);
        fprintf(stream, "    %llu -> null%d;\n",*(ULL*)(node->key), nullcount);
}
void __tree2dot(struct rbtree_node* node,FILE* stream)
{
    static int  null_node_cnt = 0;
    if(node->color == RB_BLACK)
    {
        fprintf(stream,"%llu [shape=box];\n",*(ULL*)(node->key));
    }

    if(node->left)
    {
        
        fprintf(stream,"  %llu -> %llu;\n",*(ULL*)(node->key),*(ULL*)(node->left->key));
        __tree2dot(node->left,stream);
    }
    else
    {
        process_null_node(node,null_node_cnt++,stream);
    }
    if(node->right)
    {
        fprintf(stream,"  %llu -> %llu;\n",*(ULL*)(node->key),*(ULL*)(node->right->key));
        __tree2dot(node->right,stream);
    }
    else
    {
        process_null_node(node,null_node_cnt++,stream);
    }
}
int tree2dot(struct rbtree* tree,char* filename)
{
    assert(tree != NULL && filename != NULL);
    FILE* stream = fopen(filename,"w+");
    if(stream == NULL)
    {
        fprintf(stderr, "open failed \n");
        return -1;
    }

    fprintf(stream,"digraph {\n");
    __tree2dot(tree->root,stream);


    fprintf(stream,"}\n");
    fclose(stream);

    return 0;
    
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
    ULL  * array = malloc(SIZE*sizeof(ULL ));
    if(array == NULL)
    {
        fprintf(stderr,"malloc failed\n");
        return -1;
    }
 //   srand(time(NULL));
    for(i = 0;i<SIZE;i++)
    {
        array[i] = rand()%1000;
        ret  = rbtree_insert(tree,&array[i],&array[i]);//-1 mean alloc node failed, 
                                                     //-2 mean existed node with same key
        void * data = rbtree_lookup(tree,&array[i]);
        if(ret == 0)
            assert(data == &array[i]);
    }

    print_tree(tree);
    tree2dot(tree,"tree.dot");
    return 0;
}


