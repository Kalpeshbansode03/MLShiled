#include <iostream>
#include <vector>
#include <queue>  // Include the <queue> header for queue functionality
#include <stack>  // Include the <stack> header for stack functionality
#include <omp.h>   // Include the <omp.h> header for OpenMP directives

using namespace std;

struct TreeNode {
    int data;
    vector<TreeNode*> children;

    TreeNode(int data) : data(data) {}
};

TreeNode* createTree(int numNodes) {
    if (numNodes <= 0) {
        cerr << "Error: Invalid number of nodes. Please enter a positive integer." << endl;
        return nullptr;
    }

    int data;
    cout << "Enter data for root node: ";
    if (!(cin >> data)) {
        cerr << "Error: Invalid input for root data. Please enter an integer." << endl;
        return nullptr;
    }
    TreeNode* root = new TreeNode(data);

    // Create a queue object of type TreeNode*
    queue<TreeNode*> q;
    q.push(root);

    while (!q.empty() && numNodes > 1) {
        int numChildren;
        cout << "Enter number of children for node " << q.front()->data << ": ";
        if (!(cin >> numChildren)) {
            cerr << "Error: Invalid input for number of children. Please enter an integer." << endl;
            return nullptr;  // Handle error gracefully, clean up memory if needed
        }

        for (int i = 0; i < numChildren && numNodes > 1; i++) {
            cout << "Enter data for child " << i + 1 << " of node " << q.front()->data << ": ";
            if (!(cin >> data)) {
                cerr << "Error: Invalid input for child data. Please enter an integer." << endl;
                return nullptr;  // Handle error gracefully, clean up memory if needed
            }
            TreeNode* child = new TreeNode(data);
            q.front()->children.push_back(child);
            q.push(child);
            numNodes--;
        }

        q.pop();
    }

    return root;
}

// Recursive DFS helper function (not parallelized)
void dfsRecursive(TreeNode* node) {
    cout << node->data << " ";
    for (TreeNode* child : node->children) {
        dfsRecursive(child);
    }
}

// Parallel DFS using a stack
void parallelDFS(TreeNode* root) {
    if (!root) {
        return;
    }

    stack<TreeNode*> s;
    s.push(root);

    #pragma omp parallel
    while (!s.empty()) {
        #pragma omp for nowait  // Explore children of nodes in parallel
        for (int i = 0; i < s.size(); i++) {
            TreeNode* current = s.top();
            s.pop();

            cout << current->data << " ";  // Visit current node

            // Push children in reverse order for pre-order traversal
            for (int j = current->children.size() - 1; j >= 0; j--) {
                s.push(current->children[j]);
            }
        }
    }
}

int main() {
    int numNodes;
    cout << "Enter the number of nodes in the tree: ";
    if (!(cin >> numNodes)) {
        cerr << "Error: Invalid input for number of nodes. Please enter an integer." << endl;
        return 1;
    }

    TreeNode* root = createTree(numNodes);

    if (!root) {
        cerr << "Error: Tree creation failed." << endl;
        return 1;
    }

    cout << "Parallel DFS traversal (pre-order): ";
    parallelDFS(root);
    cout << endl;

    // Optional: Recursive DFS for comparison
    cout << "Recursive DFS traversal (pre-order): ";
    dfsRecursive(root);
    cout << endl;

    return 0;
}

