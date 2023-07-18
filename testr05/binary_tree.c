#include <stdlib.h>

#include "binary_tree.h"

KeyValPair *key_val_pair_construct(void *key, void *value)
{
    KeyValPair *kvp = (KeyValPair *)malloc(sizeof(KeyValPair));

    kvp->key = key;
    kvp->value = value;

    return kvp;
}

void key_val_pair_destroy(KeyValPair *kvp)
{
    free(kvp);
}

Node *node_construct(void *key, void *value, Node *left, Node *right)
{
    Node *node = (Node *)malloc(sizeof(Node));

    node->key = key;
    node->value = value;
    node->left = left;
    node->right = right;

    return node;
}

void node_destroy(Node *node)
{
    free(node);
}

BinaryTree *binary_tree_construct(
    CmpFn cmp_fn, KeyDestroyFn key_destroy_fn,
    ValDestroyFn val_destroy_fn)
{
    BinaryTree *bt = (BinaryTree *)malloc(sizeof(BinaryTree));

    bt->root = NULL;
    bt->cmp_fn = cmp_fn;
    bt->key_destroy_fn = key_destroy_fn;
    bt->val_destroy_fn = val_destroy_fn;

    return bt;
}

void binary_tree_add(BinaryTree *bt, void *key, void *value)
{
    Node *current = bt->root, *prev = NULL;

    int flagLeft = 0;

    while (current != NULL)
    {
        if (bt->cmp_fn(key, current->key) == 0)
        {
            bt->key_destroy_fn(key);
            bt->val_destroy_fn(current->value);
            current->value = value;
            return;
        }
        if (bt->cmp_fn(key, current->key) < 0)
        {   
            prev = current;
            current = current->left;
            flagLeft = 1;
        }
        else
        {
            prev = current;
            current = current->right;
            flagLeft = 0;
        }
    }

    Node *node = node_construct(key, value, NULL, NULL);

    if (bt->root == NULL)
    {
        bt->root = node;
        return;
    }

    if (flagLeft)
        prev->left = node;
    else 
        prev->right = node;
}

int binary_tree_empty(BinaryTree *bt)
{
    if (bt->root == NULL) return 1;

    return 0;
}

void binary_tree_remove(BinaryTree *bt, void *key);

KeyValPair *binary_tree_min(BinaryTree *bt)
{
    Node *current = bt->root;

    while (current->left != NULL)
        current = current->left;

    KeyValPair *kvp = key_val_pair_construct(current->key, current->value);

    return kvp;
}

KeyValPair *binary_tree_max(BinaryTree *bt)
{
    Node *current = bt->root;

    while (current->right != NULL)
        current = current->right;

    KeyValPair *kvp = key_val_pair_construct(current->key, current->value);

    return kvp;
}

KeyValPair *binary_tree_pop_min(BinaryTree *bt);

KeyValPair *binary_tree_pop_max(BinaryTree *bt);

void *binary_tree_get(BinaryTree *bt, void *key)
{
    Node *current = bt->root;

    while (current != NULL)
    {
        if (bt->cmp_fn(key, current->key) == 0)
            return current->value;
        
        if (bt->cmp_fn(key, current->key) < 0)
            current = current->left;
        else
            current = current->right;
    }

    return NULL;
}

void binary_tree_destroy(BinaryTree *bt)
{
    Node *root = bt->root;

    binary_tree_destroy_recursive(root, bt->key_destroy_fn, bt->val_destroy_fn);

    free(bt);
}

void binary_tree_destroy_recursive(
    Node *node, KeyDestroyFn destroy_key, ValDestroyFn destroy_val)
{
    if (node == NULL)
        return;

    binary_tree_destroy_recursive(node->left, destroy_key, destroy_val);
    binary_tree_destroy_recursive(node->right, destroy_key, destroy_val);
    
    destroy_key(node->key);
    destroy_val(node->value);
    node_destroy(node);
}