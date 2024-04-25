#include <iostream>
#include <vector>
#include <queue>
#include <omp.h>

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

void parallelBFS(TreeNode* root) {
    if (!root) {
        return;
    }

    queue<TreeNode*> q;
    q.push(root);

    #pragma omp parallel
    while (!q.empty()) {
        int levelSize = q.size();  // Calculate level size at the beginning of the loop

        // Optional: Offload processing of children to separate tasks
        #pragma omp for nowait
        for (int i = 0; i < levelSize; i++) {
            TreeNode* current = q.front();
            q.pop();

            #pragma omp task
            {
                cout << current->data << " ";
                for (TreeNode* child : current->children) {
                    q.push(child);
                }
            }
        }

        #pragma omp taskwait  // Wait for all tasks within a level before proceeding
        cout << endl;       // Print newline after tasks complete
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

    cout << "Parallel BFS traversal: ";
    parallelBFS(root);

    return 0;
}

