#include <vector>
#include <queue>
#include <iostream>

using namespace std;

struct Node {//this is mostly for readability
    vector<vector<int>> grid;
    int cost = 0;
    int depth = 0;

    bool operator>(const Node& rhs) const
    {
        return cost > rhs.cost;
    }

    

};

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


void printNode(const Node& node) {
    for (int i = 0; i < node.grid.size(); i++){
        for (int j = 0; j < node.grid[0].size(); ++j){
            cout << node.grid[i][j] << " ";
        }
        cout << endl;
    }

    cout << "cost: " << node.cost << endl;
    cout << "depth: " << node.depth << endl;
    cout << endl;
}

int misplacedTile(const Node& node) {

    int outOfPlace = 0;

    for (int i = 0; i < node.grid.size(); ++i){
        for (int j = 0; j < node.grid[0].size(); ++j) {
            
            int properPos = 1 + (node.grid.size() * i + j);
            if (properPos == 9) {
                properPos = 0;
            }

            if (node.grid[i][j] != properPos) {
                outOfPlace++;
            }
        }
    }

    return outOfPlace;

}

void expand(const Node& node, priority_queue<Node, vector<Node>, greater<Node>>& nodes) {

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
        newNode.depth = node.depth + 1;

        int verticalMove = operators[i][1] + blankCoords[1];
        int horizontalMove = operators[i][0] + blankCoords[0];

        if (i < 2 && horizontalMove < node.grid.size() && horizontalMove >= 0){
            swap(newNode.grid[blankCoords[0]][blankCoords[1]], newNode.grid[horizontalMove][blankCoords[1]]);
        }
        else if (i >= 2 && verticalMove < newNode.grid[0].size() && verticalMove >= 0) {
            swap(newNode.grid[blankCoords[0]][blankCoords[1]], newNode.grid[blankCoords[0]][verticalMove]);
        }
        else {
            continue;
        }

        newNode.cost = misplacedTile(newNode);

        nodes.push(newNode);
        printNode(newNode);
    }

}

Node search (const Node& initialState) {
    priority_queue<Node, vector<Node>, greater<Node>> nodes;
    nodes.push(initialState);

    while (!nodes.empty()) {
        Node currNode;
        currNode.grid = nodes.top().grid;
        nodes.pop();

        if (currNode.grid == goal_state) {
            return currNode;
        }
        else {
            expand(currNode, nodes);
        }
    }

    Node failNode;
    failNode.cost = -1;
    return failNode;

}

int main () {

    Node tester;
    tester.grid = {
        {1, 2, 3},
        {5, 0, 6},
        {4, 7, 8}
    };

    Node hi = search(tester);

    if (hi.cost >= 0) {
        cout << "success!" << endl;
        cout << "at depth: " << hi.depth << endl;
    }


}