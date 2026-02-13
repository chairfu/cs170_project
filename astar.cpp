#include <vector>
#include <queue>
#include <iostream>
#include <unordered_set>
#include <algorithm>

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
{// Note: This hash function was not created by me! I got it from a stack overflow thread.
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

const vector<vector<int>> goal_state = {
    {1, 2, 3},
    {4, 5, 6},
    {7, 8, 0}
};

const vector<vector<int>> trivial = {
    {1, 2, 3},
    {4, 5, 6},
    {7, 8, 0}
};

const vector<vector<int>> super_easy = {
    {1, 2, 3},
    {4, 5, 6},
    {0, 7, 8}
};

const vector<vector<int>> easy = {
    {1, 2, 3},
    {5, 0, 6},
    {4, 7, 8}
};

const vector<vector<int>> medium = {
    {1, 3, 6},
    {5, 0, 2},
    {4, 7, 8}
};

const vector<vector<int>> tough = {
    {1, 3, 6},
    {5, 0, 7},
    {4, 8, 2}
};

const vector<vector<int>> difficult = {
    {1, 6, 7},
    {5, 0, 3},
    {4, 8, 2}
};

const vector<vector<int>> really_difficult = {
    {7, 1, 2},
    {4, 8, 5},
    {6, 3, 0}
};

const vector<vector<int>> good_luck = {
    {0, 7, 2},
    {4, 6, 1},
    {3, 5, 8}
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

vector<vector<int>> custom_grid() {

    cout << "What size do you want the puzzle to be? (eg 8, 15, 24...)" << endl;
    int puzzleSize;
    cin >> puzzleSize;

    int rowLength = sqrt(puzzleSize + 1);
    cout << rowLength << endl;

    vector<vector<int>> grid;

    for (int i = 0; i < rowLength; ++i) {
        cout << "Enter row " << i + 1 << endl;
        cout << "Confirm each element by selecting 'enter', and type '0' to indicate the blank" << endl;
        vector<int> row(rowLength);
        for (int j = 0; j < rowLength; ++j) {
            cin >> row[j];
        }
        grid.push_back(row);
    }

    Node test;
    test.grid = grid;
    printNode(test);
    return grid;

}

int misplacedTile(const Node& node) {

    int outOfPlace = 0;

    for (int i = 0; i < node.grid.size(); ++i){
        for (int j = 0; j < node.grid[0].size(); ++j) {
            
            int properPos = 1 + (node.grid.size() * i + j);
            if (node.grid[i][j] == 0 || properPos == 9) {continue;}

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
            
            int properPos = 1 + (node.grid.size() * i + j);
            if (currVal == 0 || properPos == 9) {continue;}

            if (currVal != properPos) {
                int yTrue = (currVal - 1) / node.grid.size();
                int xTrue = (currVal - 1) % node.grid.size();

                outOfPlace += abs(i - yTrue) + abs(j - xTrue);
            }
        }
    }

    return outOfPlace;

}

void expand(const Node& node, priority_queue<Node, vector<Node>, greater<Node>>& nodes, const int searchType, int& nodesExpanded) {

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

        if (searchType == 0) {
            newNode.cost = newNode.depth;
        }
        else {
            newNode.cost = (searchType == 2) ? newNode.depth + manhattan(newNode) : newNode.depth + misplacedTile(newNode);
        }

            nodes.push(newNode);
            nodesExpanded++;

    }

}

Node search (const Node& initialState, const int searchType, int& nodesExpanded, int& queueSize) {
    priority_queue<Node, vector<Node>, greater<Node>> nodes;
    nodes.push(initialState);

    unordered_set<Node, GridHash> visited;

    while (!nodes.empty()) {
        Node currNode;
        currNode = nodes.top();
        nodes.pop();

        if (visited.count(currNode)) {continue;}
        visited.insert(currNode);

        queueSize = max(queueSize, int(nodes.size()));

        if (currNode.grid == goal_state) {
            return currNode;
        }
        else {
            cout << "expanding node: " << endl;
            printNode(currNode);
            expand(currNode, nodes, searchType, nodesExpanded);
        }
    }

    Node failNode;
    failNode.cost = -1;
    return failNode;

}

int main () {

    cout << "Hi! I'm Willa's CS170 project!" << endl;
    cout << "What kind of puzzle would you like me to solve?" << endl;

    int puzzleType;
    int quit = 0;

    while (quit != 1) {    

        cout << "Select 0 for trivial" << endl;
        cout << "Select 1 for super easy" << endl;
        cout << "Select 2 for easy" << endl;
        cout << "Select 3 for medium" << endl;
        cout << "Select 4 for tough" << endl;
        cout << "Select 5 for difficult" << endl;
        cout << "Select 6 for really difficult" << endl;
        cout << "Select 7 for really SUPER difficult" << endl;
        cout << "Select 8 to make your own puzzle" << endl;

        cin >> puzzleType;
        Node puzzle;

        switch (puzzleType) {
            case 0:
                puzzle.grid = trivial;
                break;
            case 1:
                puzzle.grid = super_easy;
                break;
            case 2:
                puzzle.grid = easy;
                break;
            case 3:
                puzzle.grid = medium;
                break;
            case 4:
                puzzle.grid = tough;
                break;
            case 5:
                puzzle.grid = difficult;
                break;
            case 6:
                puzzle.grid = really_difficult;
                break;
            case 7:
                puzzle.grid = good_luck;
                break;
            case 8:
                puzzle.grid = custom_grid();
                break;
            default:
                break;
        }

        cout << "And how would you like me to solve this puzzle?" << endl;

        int searchType;

        cout << "Select 0 for Uniform Cost Search" << endl;
        cout << "Select 1 for A* with Misplaced Tile" << endl;
        cout << "Select 2 for A* with Manhattan Distance" << endl;

        cin >> searchType;

        int nodesExpanded = 0;
        int maxQueueSize = 0;

        Node solved;
        solved = search(puzzle, searchType, nodesExpanded, maxQueueSize);

        if (solved.cost >= 0) {
            cout << "Success!" << endl;
            printNode(solved);

            cout << "Nodes expanded: " << nodesExpanded << endl;
            cout << "Max nodes in queue: " << maxQueueSize << endl;
        }

        cout << "Would you like to search again? Press 0 if yes, 1 if you'd like to exit: " << endl;
        cin >> quit;

    }

}