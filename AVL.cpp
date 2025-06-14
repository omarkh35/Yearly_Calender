#include <iostream>
#include <queue>
#include <algorithm>
#include <climits>
using namespace std;

// Node class for AVL Tree
class AVLNode {
public:
    int data;           // Value stored in the node
    AVLNode* left;      // Pointer to the left child
    AVLNode* right;     // Pointer to the right child
    int height;         // Height of the node for balancing

    // Constructor
    AVLNode(int value) {
        data = value;
        left = nullptr;
        right = nullptr;
        height = 1;  // New node is initially at height 1
    }
};

// AVL Tree class
class AVLTree {
private:
    AVLNode* root;

    // Helper methods
    int getHeight(AVLNode* node);
    int getBalanceFactor(AVLNode* node);
    AVLNode* rightRotate(AVLNode* y);
    AVLNode* leftRotate(AVLNode* x);
    AVLNode* insertNode(AVLNode* node, int value);
    AVLNode* deleteNode(AVLNode* root, int value);
    AVLNode* minValueNode(AVLNode* node);
    void inOrderTraversal(AVLNode* node);
    void preOrderTraversal(AVLNode* node);
    void postOrderTraversal(AVLNode* node);
    AVLNode* searchNode(AVLNode* node, int value);
    void destroyTree(AVLNode* node);
    bool isAVL(AVLNode* node);
    AVLNode* findLCARecursive(AVLNode* node, int n1, int n2);

public:
    // Constructor and Destructor
    AVLTree() {
        root = nullptr;
    }

    ~AVLTree() {
        destroyTree(root);
    }

    // Public interface methods
    void insert(int value);
    void remove(int value);
    bool search(int value);
    void inOrder();
    void preOrder();
    void postOrder();
    void levelOrder();
    bool isEmpty();
    bool isAVL();
    AVLNode* findLCA(int n1, int n2);
    int distanceFromRoot(int value);
    int findDistance(int n1, int n2);
};

// Get height of a node (nullptr nodes have height 0)
int AVLTree::getHeight(AVLNode* node) {
    if (node == nullptr) {
        return 0;
    }
    return node->height;
}

// Get balance factor of a node
int AVLTree::getBalanceFactor(AVLNode* node) {
    if (node == nullptr) {
        return 0;
    }
    return getHeight(node->left) - getHeight(node->right);
}

// Right rotation
AVLNode* AVLTree::rightRotate(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;

    // Perform rotation
    x->right = y;
    y->left = T2;

    // Update heights
    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;

    // Return new root
    return x;
}

// Left rotation
AVLNode* AVLTree::leftRotate(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;

    // Perform rotation
    y->left = x;
    x->right = T2;

    // Update heights
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;

    // Return new root
    return y;
}

// Insert a node into the AVL tree
AVLNode* AVLTree::insertNode(AVLNode* node, int value) {
    // 1. Perform standard BST insertion
    if (node == nullptr) {
        return new AVLNode(value);
    }

    if (value < node->data) {
        node->left = insertNode(node->left, value);
    } else if (value > node->data) {
        node->right = insertNode(node->right, value);
    } else {
        // Duplicate values not allowed
        return node;
    }

    // 2. Update height of this ancestor node
    node->height = 1 + max(getHeight(node->left), getHeight(node->right));

    // 3. Get the balance factor to check if this node became unbalanced
    int balance = getBalanceFactor(node);

    // 4. If unbalanced, there are 4 cases

    // Left Left Case
    if (balance > 1 && value < node->left->data) {
        return rightRotate(node);
    }

    // Right Right Case
    if (balance < -1 && value > node->right->data) {
        return leftRotate(node);
    }

    // Left Right Case
    if (balance > 1 && value > node->left->data) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // Right Left Case
    if (balance < -1 && value < node->right->data) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    // Return the unchanged node pointer
    return node;
}

// Find the node with minimum value in a subtree
AVLNode* AVLTree::minValueNode(AVLNode* node) {
    AVLNode* current = node;

    // Find the leftmost leaf node
    while (current && current->left != nullptr) {
        current = current->left;
    }

    return current;
}

// Delete a node from the AVL tree
AVLNode* AVLTree::deleteNode(AVLNode* root, int value) {
    // 1. Perform standard BST delete
    if (root == nullptr) {
        return root;
    }

    // If the value to be deleted is smaller than the root's value,
    // then it lies in left subtree
    if (value < root->data) {
        root->left = deleteNode(root->left, value);
    }
    // If the value to be deleted is greater than the root's value,
    // then it lies in right subtree
    else if (value > root->data) {
        root->right = deleteNode(root->right, value);
    }
    // If value is same as root's value, then this is the node to be deleted
    else {
        // Node with only one child or no child
        if ((root->left == nullptr) || (root->right == nullptr)) {
            AVLNode* temp = root->left ? root->left : root->right;

            // No child case
            if (temp == nullptr) {
                temp = root;
                root = nullptr;
            } else {
                // One child case
                *root = *temp; // Copy the contents of the non-empty child
            }

            delete temp;
        } else {
            // Node with two children: Get the inorder successor (smallest
            // in the right subtree)
            AVLNode* temp = minValueNode(root->right);

            // Copy the inorder successor's data to this node
            root->data = temp->data;

            // Delete the inorder successor
            root->right = deleteNode(root->right, temp->data);
        }
    }

    // If the tree had only one node then return
    if (root == nullptr) {
        return root;
    }

    // 2. Update height of the current node
    root->height = 1 + max(getHeight(root->left), getHeight(root->right));

    // 3. Get the balance factor of this node to check whether
    // this node became unbalanced
    int balance = getBalanceFactor(root);

    // 4. If unbalanced, there are 4 cases

    // Left Left Case
    if (balance > 1 && getBalanceFactor(root->left) >= 0) {
        return rightRotate(root);
    }

    // Left Right Case
    if (balance > 1 && getBalanceFactor(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    // Right Right Case
    if (balance < -1 && getBalanceFactor(root->right) <= 0) {
        return leftRotate(root);
    }

    // Right Left Case
    if (balance < -1 && getBalanceFactor(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

// Search for a value in the AVL tree
AVLNode* AVLTree::searchNode(AVLNode* node, int value) {
    // Base cases: node is null or the value is found
    if (node == nullptr || node->data == value) {
        return node;
    }

    // If value is less than node's data, search in left subtree
    if (value < node->data) {
        return searchNode(node->left, value);
    }

    // If value is greater than node's data, search in right subtree
    return searchNode(node->right, value);
}

// In-order traversal
void AVLTree::inOrderTraversal(AVLNode* node) {
    if (node != nullptr) {
        inOrderTraversal(node->left);
        cout << node->data << " ";
        inOrderTraversal(node->right);
    }
}

// Pre-order traversal
void AVLTree::preOrderTraversal(AVLNode* node) {
    if (node != nullptr) {
        cout << node->data << " ";
        preOrderTraversal(node->left);
        preOrderTraversal(node->right);
    }
}

// Post-order traversal
void AVLTree::postOrderTraversal(AVLNode* node) {
    if (node != nullptr) {
        postOrderTraversal(node->left);
        postOrderTraversal(node->right);
        cout << node->data << " ";
    }
}

// Destroy the tree (free memory)
void AVLTree::destroyTree(AVLNode* node) {
    if (node != nullptr) {
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }
}

// Check if the tree is a valid AVL tree
bool AVLTree::isAVL(AVLNode* node) {
    if (node == nullptr) {
        return true;
    }

    // Get the height balance
    int balance = getBalanceFactor(node);

    // If balance factor is not -1, 0, or 1, then tree is not balanced
    if (balance < -1 || balance > 1) {
        return false;
    }

    // Check if left and right subtrees are AVL trees
    return isAVL(node->left) && isAVL(node->right);
}

// Find the lowest common ancestor
AVLNode* AVLTree::findLCARecursive(AVLNode* node, int n1, int n2) {
    if (node == nullptr) {
        return nullptr;
    }

    // If both values are less than current node, LCA is in left subtree
    if (node->data > n1 && node->data > n2) {
        return findLCARecursive(node->left, n1, n2);
    }

    // If both values are greater than current node, LCA is in right subtree
    if (node->data < n1 && node->data < n2) {
        return findLCARecursive(node->right, n1, n2);
    }

    // If one value is less and other is greater, current node is the LCA
    return node;
}

// Public interface methods

// Insert a value into the AVL tree
void AVLTree::insert(int value) {
    root = insertNode(root, value);
}

// Remove a value from the AVL tree
void AVLTree::remove(int value) {
    root = deleteNode(root, value);
}

// Search for a value in the AVL tree
bool AVLTree::search(int value) {
    return searchNode(root, value) != nullptr;
}

// In-order traversal
void AVLTree::inOrder() {
    cout << "In-order Traversal: ";
    inOrderTraversal(root);
    cout << endl;
}

// Pre-order traversal
void AVLTree::preOrder() {
    cout << "Pre-order Traversal: ";
    preOrderTraversal(root);
    cout << endl;
}

// Post-order traversal
void AVLTree::postOrder() {
    cout << "Post-order Traversal: ";
    postOrderTraversal(root);
    cout << endl;
}

// Level-order traversal (breadth-first)
void AVLTree::levelOrder() {
    if (root == nullptr) {
        cout << "Tree is empty" << endl;
        return;
    }

    cout << "Level-order Traversal: ";

    queue<AVLNode*> q;
    q.push(root);

    while (!q.empty()) {
        AVLNode* current = q.front();
        q.pop();

        cout << current->data << " ";

        if (current->left != nullptr) {
            q.push(current->left);
        }

        if (current->right != nullptr) {
            q.push(current->right);
        }
    }

    cout << endl;
}

// Check if the tree is empty
bool AVLTree::isEmpty() {
    return root == nullptr;
}

// Check if the tree is a valid AVL tree
bool AVLTree::isAVL() {
    return isAVL(root);
}

// Find the lowest common ancestor of two nodes
AVLNode* AVLTree::findLCA(int n1, int n2) {
    return findLCARecursive(root, n1, n2);
}

// Calculate distance from root to a node
int AVLTree::distanceFromRoot(int value) {
    AVLNode* current = root;
    int distance = 0;

    while (current != nullptr) {
        if (current->data == value) {
            return distance;
        }

        distance++;

        if (value < current->data) {
            current = current->left;
        } else {
            current = current->right;
        }
    }

    return -1;  // Value not found
}

// Find distance between two nodes
int AVLTree::findDistance(int n1, int n2) {
    // Find the LCA
    AVLNode* lca = findLCA(n1, n2);

    if (lca == nullptr) {
        return -1;
    }

    // Calculate distances from LCA to each node
    int d1 = distanceFromRoot(n1) - distanceFromRoot(lca->data);
    int d2 = distanceFromRoot(n2) - distanceFromRoot(lca->data);

    return d1 + d2;
}

// Function to display the menu
void displayMenu() {
    cout << "\n===== AVL Tree Operations =====\n";
    cout << "1. Insert a value\n";
    cout << "2. Delete a value\n";
    cout << "3. Search for a value\n";
    cout << "4. In-order traversal\n";
    cout << "5. Pre-order\n";
    cout << "5. Pre-order traversal\n";
    cout << "6. Post-order traversal\n";
    cout << "7. Level-order traversal\n";
    cout << "8. Check if tree is a valid AVL tree\n";
    cout << "9. Find lowest common ancestor (LCA)\n";
    cout << "10. Find distance between two nodes\n";
    cout << "11. Exit\n";
    cout << "Enter your choice: ";
}

// Main function with interactive menu
int main() {
    AVLTree avl;
    int choice, value, n1, n2;

    cout << "Welcome to the AVL Tree Program!\n";

    do {
        displayMenu();
        cin >> choice;

        switch (choice) {
            case 1:  // Insert
                cout << "Enter value to insert: ";
                cin >> value;
                avl.insert(value);
                cout << value << " inserted into the AVL tree.\n";
                break;

            case 2:  // Delete
                if (avl.isEmpty()) {
                    cout << "Tree is empty.\n";
                } else {
                    cout << "Enter value to delete: ";
                    cin >> value;
                    if (avl.search(value)) {
                        avl.remove(value);
                        cout << value << " deleted from the AVL tree.\n";
                    } else {
                        cout << value << " not found in the AVL tree.\n";
                    }
                }
                break;

            case 3:  // Search
                if (avl.isEmpty()) {
                    cout << "Tree is empty.\n";
                } else {
                    cout << "Enter value to search: ";
                    cin >> value;
                    if (avl.search(value)) {
                        cout << value << " found in the AVL tree.\n";
                    } else {
                        cout << value << " not found in the AVL tree.\n";
                    }
                }
                break;

            case 4:  // In-order traversal
                if (avl.isEmpty()) {
                    cout << "Tree is empty.\n";
                } else {
                    avl.inOrder();
                }
                break;

            case 5:  // Pre-order traversal
                if (avl.isEmpty()) {
                    cout << "Tree is empty.\n";
                } else {
                    avl.preOrder();
                }
                break;

            case 6:  // Post-order traversal
                if (avl.isEmpty()) {
                    cout << "Tree is empty.\n";
                } else {
                    avl.postOrder();
                }
                break;

            case 7:  // Level-order traversal
                if (avl.isEmpty()) {
                    cout << "Tree is empty.\n";
                } else {
                    avl.levelOrder();
                }
                break;

            case 8:  // Check if valid AVL
                if (avl.isEmpty()) {
                    cout << "Tree is empty.\n";
                } else {
                    if (avl.isAVL()) {
                        cout << "The tree is a valid AVL tree.\n";
                    } else {
                        cout << "The tree is not a valid AVL tree.\n";
                    }
                }
                break;

            case 9:  // Find LCA
                if (avl.isEmpty()) {
                    cout << "Tree is empty.\n";
                } else {
                    cout << "Enter first value: ";
                    cin >> n1;
                    cout << "Enter second value: ";
                    cin >> n2;

                    AVLNode* lca = avl.findLCA(n1, n2);
                    if (lca != nullptr) {
                        cout << "Lowest Common Ancestor of " << n1 << " and " << n2 << " is: " << lca->data << endl;
                    } else {
                        cout << "One or both values not found in the AVL tree.\n";
                    }
                }
                break;

            case 10:  // Find distance
                if (avl.isEmpty()) {
                    cout << "Tree is empty.\n";
                } else {
                    cout << "Enter first value: ";
                    cin >> n1;
                    cout << "Enter second value: ";
                    cin >> n2;

                    int distance = avl.findDistance(n1, n2);
                    if (distance != -1) {
                        cout << "Distance between " << n1 << " and " << n2 << " is: " << distance << endl;
                    } else {
                        cout << "One or both values not found in the AVL tree.\n";
                    }
                }
                break;

            case 11:  // Exit
                cout << "Exiting program. Goodbye!\n";
                break;

            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 11);

    return 0;
}

