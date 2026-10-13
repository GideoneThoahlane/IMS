#ifndef TREE_H
#define TREE_H

#include <iostream>
#include <string>
#include "Item.h"

using namespace std;

class TreeNode {
public:
    Item data;
    TreeNode* left;
    TreeNode* right;

    TreeNode(Item itemVal);
};

class Tree {
private:
    TreeNode* root;
    int count;

    TreeNode* insertRecursive(TreeNode* node, Item item);
    TreeNode* searchRecursive(TreeNode* node, string itemId);
    TreeNode* findMin(TreeNode* node);
    TreeNode* removeRecursive(TreeNode* node, string itemId, bool& deleted);

    void inorderRecursive(TreeNode* node);
    void preorderRecursive(TreeNode* node);
    void postorderRecursive(TreeNode* node);
    void destroyRecursive(TreeNode* node);
    int getHeightRecursive(TreeNode* node);

public:
    Tree();
    ~Tree();

    void insert(Item item);
    bool remove(string itemId);
    Item* search(string itemId);

    int size();
    bool isEmpty();
    int getHeight();
    void clear();

    void displayInorder();
    void displayPreorder();
    void displayPostorder();
    void displayLevelOrder();
};

#endif
