#include <vector>
#include <queue>
#include <iostream>

using namespace std;

vector<vector<int>> goal_state = {
    {1, 2, 3},
    {4, 5, 6},
    {7, 8, 0}
};

//our operators are:
    //move blank up
    //move blank down
    //move blank right
    //move blank left
vector<vector<int>> operators = {
    {1, 0},
    {-1, 0},
    {0, 1},
    {0, -1}
};

struct Node {//this is mostly for readability
    vector<vector<int>> grid;
};

void printNode(const Node& node) {
    for (int i = 0; i < node.grid.size(); i++){
        for (int j = 0; j < node.grid[0].size(); ++j){
            cout << node.grid[i][j] << " ";
        }
        cout << endl;
    }

    cout << endl;
}

vector<Node> expand(const Node& node) {

    vector<Node> expandedNodes;
    vector<int> blankCoords;

    for (int i = 0; i < node.grid.size(); i++){
        for (int j = 0; j < node.grid[0].size(); ++j){
            if (node.grid[i][j] == 0){
                blankCoords = {i, j};
                break;
            }
        }
    }

    for (int i = 0; i < operators.size(); ++i){

        Node newNode;
        newNode.grid = node.grid;

        int verticalMove = operators[i][1] + blankCoords[1];
        int horizontalMove = operators[i][0] + blankCoords[0];

        cout << "horizontal move: " << horizontalMove << endl;
        cout << "vertical move: " << verticalMove << endl;

        if (i < 2 && horizontalMove < node.grid.size() && horizontalMove >= 0){
            //i < 2 means up/down operator
            //2nd condition makes sure we're below upper bound
            //3rd condition makes sure we're above lower bound
            swap(newNode.grid[blankCoords[0]][blankCoords[1]], newNode.grid[horizontalMove][blankCoords[1]]);
            cout << "moved up or down" << endl;
        }
        else if (i >= 2 && verticalMove < newNode.grid[0].size() && verticalMove >= 0) {
            swap(newNode.grid[blankCoords[0]][blankCoords[1]], newNode.grid[blankCoords[0]][verticalMove]);
            cout << "moved left or right" << endl;
        }
        else {
            continue;
        }

        expandedNodes.push_back(newNode);
        printNode(newNode);
    }

    return expandedNodes;

}

//can we combine expand and queue later?
void queueingFunction(queue<Node>& nodes, vector<Node> expandedNodes) {
    for (int i = 0; i < expandedNodes.size(); i++) {
        nodes.push(expandedNodes[i]);
    }
}

bool search (const Node& initialState) {
    queue<Node> nodes;
    nodes.push(initialState);

    while (!nodes.empty()) {
        Node currNode;
        currNode.grid = nodes.front().grid;
        nodes.pop();

        if (currNode.grid == goal_state) {
            return true;
        }
        else {
            vector<Node> expandedNodes = expand(currNode);
            queueingFunction(nodes, expandedNodes);
        }
    }

    return false;

}

int main () {

    Node tester;
    tester.grid = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 0, 8}
    };

    bool hi = search(tester);

    if (hi) {
        cout << "success!" << endl;
    }


}