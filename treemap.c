#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
    TreeMap * new = (TreeMap *)malloc(sizeof(TreeMap));
    new->root = NULL;
    new->current = NULL;
    new->lower_than = lower_than;
    return new;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
    if (tree == NULL || tree->root == NULL){
        tree->root = createTreeNode(key, value);
        tree->current = tree->root;
        tree->current->parent = NULL;
    }
    if (searchTreeMap(tree, key) != NULL){
        return;
    }

    else{
        TreeNode * aux = tree->root;
        TreeNode * parent = NULL;
        
        while (aux != NULL){
            parent = aux;
            if (tree->lower_than(key, aux->pair->key) == 1){
                if (aux->left == NULL){
                    aux->left = createTreeNode(key, value);
                    aux->left->parent = parent;
                    break;
                }
                aux = aux->left;
            }
            else if (tree->lower_than(aux->pair->key, key) == 1){
                if (aux->right == NULL){
                    aux->right = createTreeNode(key, value);
                    aux->right->parent = parent;
                    break;
                }
                aux = aux->right; 
            }
        }
        
        if (aux->left == NULL || aux->right == NULL){
            if (tree->lower_than(key, aux->pair->key) == 1){
                aux->left = createTreeNode(key, value);
                aux->left->parent = parent;
                tree->current = aux->left;
                return;
            }
            else{
                aux->right = createTreeNode(key, value);
                aux->right->parent = parent;
                tree->current = aux->right;
                return;
            }
        }
    }
}

TreeNode * minimum(TreeNode * x){
    while (x->left != NULL){
        x = x->left;
    }
    return x;
}


void removeNode(TreeMap * tree, TreeNode* node)
{
    if (node == NULL || tree == NULL) return;
    if (node->left == NULL && node->right == NULL)
    {
        if (node->parent->left == node) node->parent->left = NULL;
        else node->parent->right = NULL;
    }

    else{
        if (node->left != NULL && node->right == NULL)
        {
            if (node->parent->left == node)
            {
                node->parent->left = node->left;
                node->left->parent = node->parent;
            }

            else
            {
                node->parent->right = node->left;
                node->left->parent = node->parent;
            }
        }

        else if(node->left == NULL && node->right != NULL)
        {
            if (node->parent->left == node)
            {
                node->parent->left = node->right;
                node->right->parent = node->parent;
            }

            else
            {
                node->parent->right = node->right;
                node->right->parent = node->parent;
            }
        }

        else
        {
            TreeNode *min = minimum(node->right);
            node->pair->key = min->pair->key;
            node->pair->value = min->pair->value;
            removeNode(tree, min);
        }
    }
    return;
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}




Pair * searchTreeMap(TreeMap * tree, void* key) {
    if (tree == NULL || tree->root == NULL) return NULL;
    TreeNode * node = tree->root;
    while (node != NULL){
        if (is_equal(tree,node->pair->key,key)){
            tree->current = node;
            return node->pair;
        }
        else{
            if (tree->lower_than(node->pair->key,key) == 1)
                node = node->right;
            else
                node = node->left;
        }
    }
    return NULL;
}
    


Pair * upperBound(TreeMap * tree, void* key) {
    return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
    TreeNode *nodeMin = minimum(tree->root);
    tree->current = nodeMin;
    return nodeMin->pair;
}

Pair * nextTreeMap(TreeMap * tree) {
    TreeNode *aux = tree->current;
    if (aux->right != NULL){
        tree->current = minimum(aux->right);
        return tree->current->pair;
    }
    else{
        while (aux->parent != NULL && aux->parent->right == aux){
            aux = aux->parent;
        }
        tree->current = aux->parent;
        
    }
    return tree->current->pair;
}
