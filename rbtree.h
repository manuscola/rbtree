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
    void*  key;
    void *data;
}rbtree_node;

typedef int (*rbtree_cmp_fn_t)(void *key_a,  void *key_b);
typedef struct rbtree
{
    struct rbtree_node* root;
    rbtree_cmp_fn_t compare; 
}rbtree;

struct rbtree* rbtree_init(rbtree_cmp_fn_t fn);
int  rbtree_insert(struct rbtree *tree, void *key,void* data);
void*  rbtree_lookup(struct rbtree* tree,void *key);
int  rbtree_remove(struct rbtree* tree,void *key);
#endif
