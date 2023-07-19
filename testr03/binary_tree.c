#include <stdlib.h>

#include "binary_tree.h"
#include "stack.h"
#include "queue.h"

void _transplant(
    BinaryTree *bt, Node *u, Node *v, Node *u_parent, Node *v_parent)
{
    if (u_parent == NULL)
        bt->root = v;
    else if (u == u_parent->left)
        u_parent->left = v;
    else
        u_parent->right = v;

    if (v != NULL)
        v_parent = u_parent;
}

void _inorder(Node *current, Vector *v)
{
    if (current == NULL)
        return;
    
    KeyValPair *pair = key_val_pair_construct(current->key, current->value);

    _inorder(current->left, v);
    vector_push_back(v, pair);
    _inorder(current->right, v);
}

void _preorder(Node *current, Vector *v)
{
    if (current == NULL)
        return;

    KeyValPair *pair = key_val_pair_construct(current->key, current->value);

    vector_push_back(v, pair);
    _preorder(current->left, v);
    _preorder(current->right, v);
}

void _postorder(Node *current, Vector *v)
{
    if (current == NULL)
        return;

    KeyValPair *pair = key_val_pair_construct(current->key, current->value);

    _postorder(current->left, v);
    _postorder(current->right, v);
    vector_push_back(v, pair);
}

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

void binary_tree_remove(BinaryTree *bt, void *key)
{
    Node *z = bt->root, *z_parent = NULL, *y = NULL, *y_parent = NULL;

    while (z != NULL)
    {
        if (bt->cmp_fn(key, z->key) == 0)
        {
            break;
        }
        if (bt->cmp_fn(key, z->key) < 0)
        {
            z_parent = z;
            z = z->left;
        }
        else
        {
            z_parent = z;
            z = z->right;
        }
    }

    if (z->left == NULL)
        _transplant(bt, z, z->right, z_parent, z);
    else if (z->right == NULL)
        _transplant(bt, z, z->left, z_parent, z);
    else
    {
        y = z->right;
        y_parent = z;

        while (y->left != NULL)
        {
            y_parent = y;

            y = y->left;
        }

        if (y_parent != z)
        {
            _transplant(bt, y, y->right, y_parent, y);
            y->right = z->right;
        }
        _transplant(bt, z, y, z_parent, y_parent);
        y->left = z->left;
    }

    bt->key_destroy_fn(z->key);
    bt->val_destroy_fn(z->value);
    node_destroy(z);
}

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

KeyValPair *binary_tree_pop_min(BinaryTree *bt)
{
    Node *current = bt->root, *current_parent = NULL;

    while (current->left != NULL)
    {
        current_parent = current;
        current = current->left;
    }

    if (current_parent == NULL)
        bt->root = current->right;
    else
        current_parent->left = current->right;

    KeyValPair *pair = key_val_pair_construct(current->key, current->value);
    node_destroy(current);

    return pair;
}

KeyValPair *binary_tree_pop_max(BinaryTree *bt)
{
    Node *current = bt->root, *current_parent = NULL;

    while (current->right != NULL)
    {
        current_parent = current;
        current = current->right;
    }

    if (current_parent == NULL)
        bt->root = current->left;
    else
        current_parent->right = current->left;

    KeyValPair *pair = key_val_pair_construct(current->key, current->value);
    node_destroy(current);

    return pair;
}

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

Vector *binary_tree_inorder_traversal(BinaryTree *bt)
{
    Vector *v = vector_construct();

    Stack *stack = stack_construct();

    Node *current = bt->root;

    while (1)
    {
        while (current != NULL)
        {
            stack_push(stack, current);
            current = current->left;
        }

        if (stack_empty(stack)) break;
        else
        {
            current = stack_pop(stack);

            KeyValPair *pair = key_val_pair_construct(current->key, current->value);
            vector_push_back(v, pair);

            current = current->right;
        }
    }

    stack_destroy(stack);

    return v;
}

Vector *binary_tree_preorder_traversal(BinaryTree *bt)
{
    Vector *v = vector_construct();

    Stack *stack = stack_construct();

    stack_push(stack, bt->root);

    while (!stack_empty(stack))
    {
        Node *current = stack_pop(stack);

        if (current != NULL)
        {
            KeyValPair *pair = key_val_pair_construct(current->key, current->value);
            vector_push_back(v, pair);

            stack_push(stack, current->right);
            stack_push(stack, current->left);
        }
    }

    stack_destroy(stack);

    return v;
}

Vector *binary_tree_postorder_traversal(BinaryTree *bt)
{
    Vector *v = vector_construct();

    Stack *q1 = stack_construct(), *q2 = stack_construct();

    stack_push(q1, bt->root);

    while (!stack_empty(q1))
    {
        Node *current = stack_pop(q1);

        if (current != NULL)
        {
            stack_push(q1, current->left);
            stack_push(q1, current->right);
            stack_push(q2, current);   
        }           
    }

    while (!stack_empty(q2))
    {
        Node *current = stack_pop(q2);

        KeyValPair *pair = key_val_pair_construct(current->key, current->value);
        vector_push_back(v, pair);
    }

    stack_destroy(q1);
    stack_destroy(q2);

    return v;
}

Vector *binary_tree_levelorder_traversal(BinaryTree *bt)
{
    Vector *v = vector_construct();

    Queue *queue = queue_construct();

    queue_push(queue, bt->root);

    while (!queue_empty(queue))
    {
        Node *current = queue_pop(queue);

        if (current != NULL)
        {
            KeyValPair *pair = key_val_pair_construct(current->key, current->value);
            vector_push_back(v, pair);

            queue_push(queue, current->left);
            queue_push(queue, current->right);
        }
    }

    queue_destroy(queue);

    return v;
}

Vector *binary_tree_inorder_traversal_recursive(BinaryTree *bt)
{
    Vector *v = vector_construct();

    Node *root = bt->root;

    _inorder(root, v);

    return v;
}

Vector *binary_tree_preorder_traversal_recursive(BinaryTree *bt)
{
    Vector *v = vector_construct();

    Node *root = bt->root;

    _preorder(root, v);

    return v;   
}

Vector *binary_tree_postorder_traversal_recursive(BinaryTree *bt)
{
    Vector *v = vector_construct();

    Node *root = bt->root;

    _postorder(root, v);

    return v;
}