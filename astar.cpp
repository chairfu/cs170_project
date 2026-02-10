#include <vector>
#include <queue>
#include <iostream>
#include <unordered_set>

using namespace std;

struct Node {
    vector<vector<int>> grid;
    int cost = 0;
    int depth = 0;

    bool operator>(const Node& rhs) const {
        return (cost > rhs.cost);
    }

    bool operator==(const Node& rhs) const {
        return (grid == rhs.grid);
    }
};

struct GridHash
{// I DON"T KNOW HOW TO MAKE HASH FUNCTIONS THIS CAME FROM STACK OVERFLOW!!
//source: https://stackoverflow.com/questions/20511347/a-good-hash-function-for-a-vector
  size_t operator()(const Node& node) const {
    size_t seed = node.grid.size();
    for(auto& i : node.grid) {
        for (int j = 0; j < node.grid[0].size(); ++j) {
        seed ^= i[j] + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
    }
  return seed;
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
            //if (node.grid[i][j] == 9) {cout << 0 << " ";}
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

int manhattan(const Node& node){

    int outOfPlace = 0;

    for (int i = 0; i < node.grid.size(); ++i){
        for (int j = 0; j < node.grid[0].size(); ++j) {

            int currVal = node.grid[i][j];
            
            int properPos = 1 + (node.grid.size() * i + j); //note: this can't be zero!
            //potentially change this later to include grids of greater sizes
            if (currVal == 0) {currVal = 9;}

            if (currVal != properPos) {
                int yTrue = currVal / node.grid.size(); //eg 4 / 3 = 1, 8 / 3 = 2, 1 / 3 = 0
                int xTrue = (currVal - 1) % node.grid.size(); //eg 4 - 1 % 3 = 0, 8 - 1 % 3 = 1 

                outOfPlace += abs(i - yTrue) + abs(j - xTrue);
            }
        }
    }

    return outOfPlace;

}

void expand(const Node& node, priority_queue<Node, vector<Node>, greater<Node>>& nodes, unordered_set<Node, GridHash>& visited) {

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

        newNode.cost = newNode.depth + misplacedTile(newNode);
        //newNode.cost = newNode.depth + manhattan(newNode);
        if (!visited.count(newNode)) {
            nodes.push(newNode);
            visited.insert(newNode);
        }

    }

}

Node search (const Node& initialState) {
    priority_queue<Node, vector<Node>, greater<Node>> nodes;
    nodes.push(initialState);

    unordered_set<Node, GridHash> visited;

    while (!nodes.empty()) {
        Node currNode;
        currNode = nodes.top();
        nodes.pop();

        if (currNode.grid == goal_state) {
            return currNode;
        }
        else {
            cout << "expanding node: " << endl;
            printNode(currNode);
            expand(currNode, nodes, visited);
        }
    }

    Node failNode;
    failNode.cost = -1;
    return failNode;

}

int main () {

    Node tester;
    tester.grid = {
        {1, 6, 7},
        {5, 0, 3},
        {4, 8, 2}
    };

    Node hi = search(tester);


    if (hi.cost >= 0) {
        cout << "success!" << endl;
        printNode(hi);
    }


}