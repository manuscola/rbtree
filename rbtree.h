#ifndef __RBTREE_H__
#define __RBTREE_H__

enum rb_color
{
    RB_BLACK,
    RB_RED,
};

typedef struct rbtree_node
{
    struct rbtree_node* parent;
    struct rbtree_node* left;
    struct rbtree_node* right;
    enum rb_color color;
    unsigned long long  key;
    void *data;
}rbtree_node;

typedef struct rbtree
{
    struct rbtree_node* root;
}rbtree;

struct rbtree* rbtree_init();
int  rbtree_insert(struct rbtree *tree, unsigned long long key,void* data);
void*  rbtree_lookup(struct rbtree* tree,unsigned long long key);
int  rbtree_remove(struct rbtree* tree,unsigned long long key);
#endif
