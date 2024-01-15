#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <queue>
#include <map>

using namespace std;

class TreeNode
{
public:
    string value;
    TreeNode *left;
    TreeNode *right;

    TreeNode(string val) : value(val), left(nullptr), right(nullptr) {}
};

// Function to build the tree from inorder and preorder
TreeNode *buildTreeInPre(vector<string> &inorder, vector<string> &preorder, int inStart, int inEnd, int &preIndex)
{
    if (inStart > inEnd)
        return nullptr;

    TreeNode *root = new TreeNode(preorder[preIndex++]);

    int inIndex = distance(inorder.begin(), find(inorder.begin(), inorder.end(), root->value));

    root->left = buildTreeInPre(inorder, preorder, inStart, inIndex - 1, preIndex);
    root->right = buildTreeInPre(inorder, preorder, inIndex + 1, inEnd, preIndex);

    return root;
}

// Function to build the tree from inorder and postorder
TreeNode *buildTreeInPost(vector<string> &inorder, vector<string> &postorder, int inStart, int inEnd, int &postIndex)
{
    if (inStart > inEnd)
        return nullptr;

    TreeNode *root = new TreeNode(postorder[postIndex--]);

    int inIndex = distance(inorder.begin(), find(inorder.begin(), inorder.end(), root->value));

    root->right = buildTreeInPost(inorder, postorder, inIndex + 1, inEnd, postIndex);
    root->left = buildTreeInPost(inorder, postorder, inStart, inIndex - 1, postIndex);

    return root;
}

map<string, int> treeToArray(TreeNode *root)
{
    map<string, int> nodePositions;
    if (root == nullptr)
        return nodePositions;

    queue<pair<TreeNode *, int>> q;
    q.push({root, 1}); // root is at position 1

    while (!q.empty())
    {
        auto node = q.front().first;
        auto pos = q.front().second;
        q.pop();

        // Add the node's value and position to the map
        nodePositions[node->value] = pos;

        if (node->left != nullptr)
            q.push({node->left, 2 * pos}); // left child is at position 2n

        if (node->right != nullptr)
            q.push({node->right, 2 * pos + 1}); // right child is at position 2n+1
    }

    return nodePositions;
}

// Function to print the value and position of each node
void printNodesInLevelOrder(const map<string, int> &nodePositions)
{
    // sorting it->second from minimum to maximum
    vector<int> v;
    for (auto it = nodePositions.begin(); it != nodePositions.end(); it++)
        v.push_back(it->second);

    sort(v.begin(), v.end());

    // print the nodes in level order
    for (int i = 0; i < v.size(); i++)
    {
        for (auto it = nodePositions.begin(); it != nodePositions.end(); it++)
            if (it->second == v[i])
                cout << it->first << ":" << it->second << endl;
    }
}

// Function to read the traversal type and the values from a line
void InputLine(const string &line, string &traversalType, vector<string> &values)
{
    istringstream iss(line);
    string temp;

    // Read traversal type
    getline(iss, temp, ':');
    traversalType = temp;

    // Read the rest of the line into temp
    getline(iss, temp);
    string valuesString = temp;

    // Extract values and remove '(' and ')'
    values.clear();
    size_t start = valuesString.find_first_of('(');
    size_t end;
    while (start != string::npos)
    {
        end = valuesString.find_first_of(')', start);
        values.push_back(valuesString.substr(start + 1, end - start - 1));
        start = valuesString.find_first_of('(', end);
    }
}

int main(int argc, char **argv)
{
    ifstream InFile(argv[1]);
    string line;
    while (getline(InFile, line) && !line.empty())
    {
        // Separate the traversal type and the values from the first line
        string traversaltype1;
        vector<string> order1;
        InputLine(line, traversaltype1, order1);

        // Read and separate the traversal type and the values from the second line
        getline(InFile, line);
        string traversaltype2;
        vector<string> order2;
        InputLine(line, traversaltype2, order2);

        if (traversaltype2 == "Postorder")
        {
            int postIndex = order2.size() - 1;
            TreeNode *root2 = buildTreeInPost(order1, order2, 0, order1.size() - 1, postIndex);

            // Convert the tree to an array and print the results
            map<string, int> array = treeToArray(root2);
            printNodesInLevelOrder(array);
        }
        if (traversaltype2 == "Preorder")
        {
            int preIndex = 0;
            TreeNode *root1 = buildTreeInPre(order1, order2, 0, order1.size() - 1, preIndex);

            // Convert the tree to an array and print the results
            map<string, int> array = treeToArray(root1);
            printNodesInLevelOrder(array);
        }
        if (traversaltype2 == "Inorder")
        {
            if (traversaltype1 == "Postorder")
            {
                int postIndex = order1.size() - 1;
                TreeNode *root2 = buildTreeInPost(order2, order1, 0, order2.size() - 1, postIndex);

                // Convert the tree to an array and print the results
                map<string, int> array = treeToArray(root2);
                printNodesInLevelOrder(array);
            }
            else if (traversaltype1 == "Preorder")
            {
                int preIndex = 0;
                TreeNode *root1 = buildTreeInPre(order2, order1, 0, order2.size() - 1, preIndex);

                // Convert the tree to an array and print the results
                map<string, int> array = treeToArray(root1);
                printNodesInLevelOrder(array);
            }
        }
    }

    return 0;
}