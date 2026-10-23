#include "Tree.h"
#include <iomanip>
using namespace std;

TreeNode::TreeNode(Item itemVal) {
    data = itemVal;
    left = nullptr;
    right = nullptr;
}

Tree::Tree() {
    root = nullptr;
    count = 0;
}

Tree::~Tree() {
    clear();
}

void Tree::destroyRecursive(TreeNode* node) {
    if (node != nullptr) {
        destroyRecursive(node->left);
        destroyRecursive(node->right);
        delete node;
    }
}

void Tree::clear() {
    destroyRecursive(root);
    root = nullptr;
    count = 0;
}

TreeNode* Tree::insertRecursive(TreeNode* node, Item item) {
    if (node == nullptr) {
        count++;
        return new TreeNode(item);
    }
    if (item.getItemId() < node->data.getItemId()) {
        node->left = insertRecursive(node->left, item);
    } else if (item.getItemId() > node->data.getItemId()) {
        node->right = insertRecursive(node->right, item);
    } else {
        node->data = item;
    }
    return node;
}

void Tree::insert(Item item) {
    root = insertRecursive(root, item);
}

TreeNode* Tree::searchRecursive(TreeNode* node, string itemId) {
    if (node == nullptr || node->data.getItemId() == itemId) {
        return node;
    }
    if (itemId < node->data.getItemId()) {
        return searchRecursive(node->left, itemId);
    }
    return searchRecursive(node->right, itemId);
}

Item* Tree::search(string itemId) {
    TreeNode* res = searchRecursive(root, itemId);
    return (res != nullptr) ? &(res->data) : nullptr;
}

TreeNode* Tree::findMin(TreeNode* node) {
    while (node != nullptr && node->left != nullptr) {
        node = node->left;
    }
    return node;
}

TreeNode* Tree::removeRecursive(TreeNode* node, string itemId, bool& deleted) {
    if (node == nullptr) return nullptr;

    if (itemId < node->data.getItemId()) {
        node->left = removeRecursive(node->left, itemId, deleted);
    } else if (itemId > node->data.getItemId()) {
        node->right = removeRecursive(node->right, itemId, deleted);
    } else {
        deleted = true;
        count--;
        if (node->left == nullptr) {
            TreeNode* temp = node->right;
            delete node;
            return temp;
        } else if (node->right == nullptr) {
            TreeNode* temp = node->left;
            delete node;
            return temp;
        }
        TreeNode* successor = findMin(node->right);
        node->data = successor->data;
        count++;
        node->right = removeRecursive(node->right, successor->data.getItemId(), deleted);
    }
    return node;
}

bool Tree::remove(string itemId) {
    bool deleted = false;
    root = removeRecursive(root, itemId, deleted);
    return deleted;
}

int Tree::size() { return count; }
bool Tree::isEmpty() { return root == nullptr; }

int Tree::getHeightRecursive(TreeNode* node) {
    if (node == nullptr) return 0;
    int lh = getHeightRecursive(node->left);
    int rh = getHeightRecursive(node->right);
    return 1 + (lh > rh ? lh : rh);
}

int Tree::getHeight() {
    return getHeightRecursive(root);
}

void Tree::inorderRecursive(TreeNode* node) {
    if (node != nullptr) {
        inorderRecursive(node->left);
        node->data.display();
        inorderRecursive(node->right);
    }
}

//void Tree::displayInorder() {
//    cout << "\n--- BST Inorder Traversal (Sorted Item Keys) ---\n";
//    cout << "Item ID\tName\t\t\tCategory\t\tStock\tPrice\tLocation\n";
//    cout << "----------------------------------------------------------------------------------------------------\n";
//    inorderRecursive(root);
//}
//
//void Tree::preorderRecursive(TreeNode* node) {
//    if (node != nullptr) {
//        node->data.display();
//        preorderRecursive(node->left);
//        preorderRecursive(node->right);
//    }
//}
//
//void Tree::displayPreorder() {
//    cout << "\n--- BST Preorder Traversal (Root-Left-Right) ---\n";
//    cout << "Item ID\tName\t\t\tCategory\t\tStock\tPrice\tLocation\n";
//    cout << "----------------------------------------------------------------------------------------------------\n";
//    preorderRecursive(root);
//}
//
//void Tree::postorderRecursive(TreeNode* node) {
//    if (node != nullptr) {
//        postorderRecursive(node->left);
//        postorderRecursive(node->right);
//        node->data.display();
//    }
//}
//
//void Tree::displayPostorder() {
//    cout << "\n--- BST Postorder Traversal (Left-Right-Root) ---\n";
//    cout << "Item ID\tName\t\t\tCategory\t\tStock\tPrice\tLocation\n";
//    cout << "----------------------------------------------------------------------------------------------------\n";
//    postorderRecursive(root);
//}
//
//void Tree::displayLevelOrder() {
//    cout << "\n--- BST Level-Order Traversal (Breadth-First BFS) ---\n";
//    cout << "Item ID\tName\t\t\tCategory\t\tStock\tPrice\tLocation\n";
//    cout << "----------------------------------------------------------------------------------------------------\n";
//    if (root == nullptr) return;
//
//    // Simple queue array of TreeNode pointers
//    TreeNode* q[100];
//    int front = 0;
//    int rear = 0;
//
//    q[rear++] = root;
//
//    while (front < rear) {
//        TreeNode* curr = q[front++];
//        curr->data.display();
//
//        if (curr->left != nullptr && rear < 100) q[rear++] = curr->left;
//        if (curr->right != nullptr && rear < 100) q[rear++] = curr->right;
//    }
//}

void Tree::displayInorder() {
    cout << "\n--- BST Inorder Traversal (Sorted Item Keys) ---\n";
    cout << "----------------------------------------------------------------------------------------------------\n";
    cout << left
         << setw(12) << "Item ID"
         << setw(25) << "Name"
         << setw(24) << "Category"
         << setw(8)  << "Stock"
         << setw(12) << "Price"
         << "Location\n";
    cout << "----------------------------------------------------------------------------------------------------\n";

    inorderRecursive(root);
}

void Tree::preorderRecursive(TreeNode* node) {
    if (node != nullptr) {
        node->data.display();
        preorderRecursive(node->left);
        preorderRecursive(node->right);
    }
}

void Tree::displayPreorder() {
    cout << "\n--- BST Preorder Traversal (Root-Left-Right) ---\n";
    cout << "----------------------------------------------------------------------------------------------------\n";
    cout << left
         << setw(12) << "Item ID"
         << setw(25) << "Name"
         << setw(24) << "Category"
         << setw(8)  << "Stock"
         << setw(12) << "Price"
         << "Location\n";
    cout << "----------------------------------------------------------------------------------------------------\n";

    preorderRecursive(root);
}

void Tree::postorderRecursive(TreeNode* node) {
    if (node != nullptr) {
        postorderRecursive(node->left);
        postorderRecursive(node->right);
        node->data.display();
    }
}

void Tree::displayPostorder() {
    cout << "\n--- BST Postorder Traversal (Left-Right-Root) ---\n";
    cout << "----------------------------------------------------------------------------------------------------\n";
    cout << left
         << setw(12) << "Item ID"
         << setw(25) << "Name"
         << setw(24) << "Category"
         << setw(8)  << "Stock"
         << setw(12) << "Price"
         << "Location\n";
    cout << "----------------------------------------------------------------------------------------------------\n";

    postorderRecursive(root);
}

void Tree::displayLevelOrder() {
    cout << "\n--- BST Level-Order Traversal (Breadth-First BFS) ---\n";
    cout << "----------------------------------------------------------------------------------------------------\n";
    cout << left
         << setw(12) << "Item ID"
         << setw(25) << "Name"
         << setw(24) << "Category"
         << setw(8)  << "Stock"
         << setw(12) << "Price"
         << "Location\n";
    cout << "----------------------------------------------------------------------------------------------------\n";

    if (root == nullptr) {
        cout << "Tree is empty.\n";
        return;
    }

    TreeNode* q[100];
    int front = 0;
    int rear = 0;

    q[rear++] = root;

    while (front < rear) {
        TreeNode* curr = q[front++];
        curr->data.display();

        if (curr->left != nullptr && rear < 100)
            q[rear++] = curr->left;

        if (curr->right != nullptr && rear < 100)
            q[rear++] = curr->right;
    }
}