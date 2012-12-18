#include "rbtree.h"
#include<stdlib.h>
#include<stdio.h>
#include<assert.h>

void delete_case1(struct rbtree* tree,struct rbtree_node* node);
void delete_case2(struct rbtree* tree,struct rbtree_node* node);
void delete_case3(struct rbtree* tree,struct rbtree_node* node);
void delete_case4(struct rbtree* tree,struct rbtree_node* node);
void delete_case5(struct rbtree* tree,struct rbtree_node* node);
void delete_case6(struct rbtree* tree,struct rbtree_node* node);

static inline enum rb_color get_color(struct rbtree_node* node)
{
    if(node == NULL)
        return RB_BLACK;
    else
        return node->color;    
}

static inline void set_color(enum rb_color color,struct rbtree_node* node)
{
    assert(node != NULL);
    node->color = color;
}

static inline struct rbtree_node*  get_parent(struct rbtree_node* node)
{
    assert(node != NULL);
    return node->parent;
}

static inline void set_parent(struct rbtree_node* parent,struct rbtree_node* node)
{
    assert(node != NULL);
    node->parent = parent;
}

static int is_root(struct rbtree_node* node)
{
    assert(node != NULL);
    return (get_parent(node)==NULL);
}

static inline int is_black(struct rbtree_node* node)
{
    assert(node != NULL);
    return (get_color(node) == RB_BLACK);
}

static inline int is_red(struct rbtree_node *node)
{
    assert(node != NULL);
    return (get_color(node) == RB_RED);
}

struct rbtree_node* sibling(rbtree_node* node)
{
    assert (node != NULL);
    assert (node->parent != NULL); /* Root node has no sibling */
    if (node == node->parent->left)
        return node->parent->right;
    else
        return node->parent->left;
}
static inline rbtree_node* get_min(struct rbtree_node* node)
{
    assert(node != NULL);
    while(node->left)
    {
        node =  node->left;
    }
    return node;
}

static inline rbtree_node* get_max(struct rbtree_node* node)
{
    assert(node != NULL);
    while(node->right)
    {
        node = node->right;
    }
    return node;
}

struct rbtree_node* rbtree_min(struct rbtree *tree)
{
    if(tree->root == NULL)
        return NULL;
    else
    {
        return get_min(tree->root);
    }
}

struct rbtree_node* rbtree_max(struct rbtree* tree)
{
    if(tree->root == NULL)
        return NULL;
    else
    {
        return get_max(tree->root);
    }
}

struct rbtree_node* rbtree_prev(struct rbtree_node* node)
{
    assert(node != NULL);
    if(node->left)
    {
        return get_max(node->left);
    }
    else
    {
        struct rbtree_node* parent;
        while ((parent = get_parent(node)) && parent->left == node)
        {
            node = parent;
        }
        return parent;
    }
}

struct rbtree_node* rbtree_next(struct rbtree_node* node)
{
    assert(node != NULL);

    if(node->right)
        return get_min(node->right);
    else
    {
        struct rbtree_node* parent = NULL;
        while((parent = get_parent(node)) != NULL && parent->right == node)
        {
            node = parent;
        }
        return parent;
    }
}

struct rbtree_node* rbtree_createnode(void *key, void* data)
{
    struct rbtree_node* newnode = malloc(sizeof(struct rbtree_node));
    if(newnode == NULL)
        return NULL;

    newnode->key = key;
    newnode->data = data;
    newnode->parent = NULL;
    newnode->left = NULL;
    newnode->right = NULL;
    return newnode;
}

/*
static inline int compare(void* key_a,void* key_b)
{
    if(key_a > key_b)
        return 1;
    else if(key_a == key_b)
        return 0;
    else
        return -1;
}*/

struct rbtree_node* do_lookup(void* key,
        struct rbtree* tree,
        struct rbtree_node** pparent)
{
    struct rbtree_node *current = tree->root;

    while(current)
    {
        int ret = tree->compare(current->key,key);
        if(ret == 0 )
            return  current;
        else
        {
            if(pparent != NULL)
            {
                *pparent = current;
            }
            if (ret < 0 )
                current = current->right;
            else
                current = current->left;
        }
    }
    return NULL;

}

void*  rbtree_lookup(struct rbtree* tree,void* key)
{
    assert(tree != NULL) ;
    struct rbtree_node* node;
    node = do_lookup(key,tree,NULL);
    return node == NULL ?NULL:node->data;
}

static void set_child(struct rbtree* tree,struct rbtree_node* node,struct rbtree_node* child)
{
    int ret = tree->compare(node->key,child->key);
    assert(ret != 0);

    if(ret > 0)
    {
        node->left = child;
    }
    else{
        node->right = child;
    }
}

static void rotate_left(struct rbtree_node* node,struct rbtree* tree)
{
    struct rbtree_node* p = node;
    struct rbtree_node* q = node->right;
    struct rbtree_node* parent = node->parent;
    if(parent == NULL)
    {
        tree->root = q;
    }
    else
    {
        if(parent->left == p)
            parent->left = q;
        else
            parent->right = q;
    }
    set_parent(parent,q);
    set_parent(q,p);

    p->right = q->left;
    if(q->left)
        set_parent(p,q->left);
    q->left = p;

}

static void rotate_right(struct rbtree_node *node, struct rbtree *tree)
{
    struct rbtree_node *p = node;
    struct rbtree_node *q = node->left; /* can't be NULL */
    struct rbtree_node *parent = get_parent(p);

    if (!is_root(p)) {
        if (parent->left == p)
            parent->left = q;
        else
            parent->right = q;
    } else
        tree->root = q;
    set_parent(parent, q);
    set_parent(q, p);

    p->left = q->right;
    if (p->left)
        set_parent(p, p->left);
    q->right = p;
}



struct rbtree* rbtree_init(rbtree_cmp_fn_t compare)
{
    struct rbtree* tree = malloc(sizeof(struct rbtree));
    if(tree == NULL)
        return NULL;
    else
    {
        tree->root = NULL;
        tree->compare = compare;
    }
    
    return tree;
}
struct rbtree_node* __rbtree_insert(struct rbtree_node* node,struct rbtree *tree)
{
    struct rbtree_node* samenode=NULL;
    struct rbtree_node*parent=NULL;

    samenode = do_lookup(node->key,tree,&parent);
    if(samenode != NULL)
        return samenode;

    node->left = node->right = NULL;
    set_color(RB_RED,node);
    set_parent(parent,node);

    if(parent == NULL)
        tree->root = node;
    else
    {
        set_child(tree,parent,node);
    }

    while((parent = get_parent(node)) != NULL && parent->color == RB_RED)
    {
        struct rbtree_node* grandpa = get_parent(parent);//grandpa must be existed 
        //because root is black ,and parent is red,
        //parent can not be root of tree. and parent is red,so grandpa must be black
        if(parent == grandpa->left)
        {
            struct rbtree_node* uncle = grandpa->right;
            if(uncle && get_color(uncle) == RB_RED)
            {
                set_color(RB_RED,grandpa);
                set_color(RB_BLACK,parent);
                set_color(RB_BLACK,uncle);
                node = grandpa;
            }
            else
            {
                if(node == parent->right )
                {
                    rotate_left(parent,tree);
                    node = parent;
                    parent = get_parent(parent);
                }
                set_color(RB_BLACK,parent);
                set_color(RB_RED,grandpa);
                rotate_right(grandpa,tree);
            }

        }
        else
        {
            struct rbtree_node* uncle = grandpa->left;
            if(uncle && uncle->color == RB_RED)
            {
                set_color(RB_RED,grandpa);
                set_color(RB_BLACK,parent);
                set_color(RB_BLACK,uncle);
                node = grandpa;
            }
            else
            {
                if(node == parent->left)
                {
                    rotate_right(parent,tree);
                    node = parent;
                    parent = get_parent(node);
                }
                set_color(RB_BLACK, parent);
                set_color(RB_RED, grandpa);
                rotate_left(grandpa, tree);
            }
        }
    }

    set_color(RB_BLACK,tree->root);
    return NULL;
}

int  rbtree_insert(struct rbtree *tree, void*  key,void* data)
{
    struct rbtree_node * node = rbtree_createnode(key,data);
    struct rbtree_node* samenode = NULL;
    if(node == NULL)
        return -1;
    else
        samenode = __rbtree_insert(node,tree);
    if(samenode != NULL)
        return -2;
    return 0;
}


void replace_node(struct rbtree* t, rbtree_node *oldn, rbtree_node* newn) 
{
    if (oldn->parent == NULL)
    {
        t->root = newn;
    }
    else
    {
        if (oldn == oldn->parent->left)
            oldn->parent->left = newn;
        else
            oldn->parent->right = newn;
    }
    if (newn != NULL)
    {
        newn->parent = oldn->parent;
    }
}

void delete_case1(struct rbtree* tree, struct rbtree_node* node)
{
    if(node->parent == NULL)
        return ;
    else
        delete_case2(tree,node);
}

void delete_case2(struct rbtree* tree, struct rbtree_node* node)
{
    if(get_color(sibling(node)) == RB_RED)
    {
        node->parent->color = RB_RED;
        sibling(node)->color = RB_BLACK;
        if(node == node->parent->left)
        {
            rotate_left(node->parent,tree);
        }
        else
        {
            rotate_right(node->parent,tree);
        }
    }
    delete_case3(tree,node);
}

void delete_case3(struct rbtree* tree,struct rbtree_node* node)
{
    if(node->parent->color == RB_BLACK &&
            get_color(sibling(node)) == RB_BLACK &&
            get_color(sibling(node)->right) == RB_BLACK &&
            get_color(sibling(node)->left) == RB_BLACK)
    {
        sibling(node)->color = RB_RED;
        delete_case1(tree, node->parent);
    }
    else
    {
        delete_case4(tree, node);
    }

}

void delete_case4(struct rbtree* t, struct rbtree_node* n) 
{
    if (get_color(n->parent) == RB_RED &&
            get_color(sibling(n)) ==RB_BLACK &&
            get_color(sibling(n)->left) ==RB_BLACK &&
            get_color(sibling(n)->right) == RB_BLACK)
    {
        sibling(n)->color =RB_RED; //sibling's two son is black ,so it can changed to red
        n->parent->color = RB_BLACK;
    }
    else
        delete_case5(t, n);
}

void delete_case5(struct rbtree *t, rbtree_node *n)
{
    if (n == n->parent->left &&
            get_color(sibling(n)) ==RB_BLACK &&
            get_color(sibling(n)->left) == RB_RED &&
            get_color(sibling(n)->right) == RB_BLACK)
    {
        sibling(n)->color = RB_RED;
        sibling(n)->left->color =RB_BLACK;
        rotate_right(sibling(n),t);
    }
    else if (n == n->parent->right &&
            get_color(sibling(n)) == RB_BLACK &&
            get_color(sibling(n)->right) == RB_RED &&
            get_color(sibling(n)->left) == RB_BLACK)
    {
        sibling(n)->color = RB_RED;
        sibling(n)->right->color = RB_BLACK;
        rotate_left(sibling(n),t);
    }
    delete_case6(t, n);
}

void delete_case6(struct rbtree*  t, rbtree_node* n)
{
    sibling(n)->color = get_color(n->parent);
    n->parent->color = RB_BLACK;
    if (n == n->parent->left) 
    {
        assert (get_color(sibling(n)->right) == RB_RED);
        sibling(n)->right->color = RB_BLACK;
        rotate_left(n->parent,t);
    }
    else
    {
        assert (get_color(sibling(n)->left) == RB_RED);
        sibling(n)->left->color = RB_BLACK;
        rotate_right( n->parent,t);
    }
}
void __rbtree_remove(struct rbtree_node* node,struct rbtree* tree)
{
    struct rbtree_node *left = node->left;
    struct rbtree_node* right = node->right;
    struct rbtree_node* child = NULL;
    if(left != NULL && right != NULL )
    {
        struct rbtree_node* next = get_min(right);
        node->key = next->key;
        node->data = next->data;
        node = next;
    }

    assert(node->left == NULL || node->right == NULL);
    child = (node->right == NULL ? node->left : node->right);
    if(get_color(node) == RB_BLACK)
    {
        set_color(get_color(child),node);
        delete_case1(tree,node);
    }
    replace_node(tree,node,child);
    if(node->parent == NULL && child != NULL)//node is root,root should be black
        set_color(RB_BLACK,child);
    free(node);
}

int  rbtree_remove(struct rbtree* tree,void *key)
{
    struct rbtree_node* node = do_lookup(key,tree,NULL);
    if(node == NULL)
        return -1;
    else
        __rbtree_remove(node,tree);
    return 0;
}
