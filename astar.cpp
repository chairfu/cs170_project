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
            
            int properPos = 1 + (node.grid.size() * i + j);
            if (currVal == 0) {currVal = node.grid.size() * node.grid.size();}

            if (currVal != properPos) {
                int yTrue = currVal / node.grid.size(); //eg 4 / 3 = 1, 8 / 3 = 2, 1 / 3 = 0
                int xTrue = (currVal - 1) % node.grid.size(); //eg 4 - 1 % 3 = 0, 8 - 1 % 3 = 1 

                outOfPlace += abs(i - yTrue) + abs(j - xTrue);
            }
        }
    }

    return outOfPlace;

}

void expand(const Node& node, priority_queue<Node, vector<Node>, greater<Node>>& nodes, unordered_set<Node, GridHash>& visited, const int searchType) {

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
            newNode.cost = (searchType > 1) ? newNode.depth + manhattan(newNode) : newNode.cost + misplacedTile(newNode);
        }

        if (!visited.count(newNode)) {
            nodes.push(newNode);
            visited.insert(newNode);
        }

    }

}

Node search (const Node& initialState, const int searchType) {
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
            expand(currNode, nodes, visited, searchType);
        }
    }

    Node failNode;
    failNode.cost = -1;
    return failNode;

}

int main () {

    //the interface begins...

    cout << "Hi! I'm Willa's CS170 project! ٩(◕‿◕｡)۶" << endl;
    cout << "If Willa did a good job, I can solve any solvable 8 puzzle. You can pick a puzzle for me to solve if you don't believe me: " << endl;

    int puzzleType;

    cout << "Select 0 for trivial" << endl;
    cout << "Select 1 for super easy" << endl;
    cout << "Select 2 for easy" << endl;
    cout << "Select 3 for medium" << endl;
    cout << "Select 4 for tough" << endl;
    cout << "Select 5 for difficult" << endl;
    cout << "Select 6 for really difficult" << endl;
    cout << "Select 7 for really SUPER difficult" << endl;

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
        default:
            break;
    }

    cout << "And how would you like me to solve this puzzle?" << endl;

    int searchType;

    cout << "Select 0 for Uniform Cost Search" << endl;
    cout << "Select 1 for A* with Misplaced Tile" << endl;
    cout << "Select 2 for A* with Manhattan Distance" << endl;

    cin >> searchType;

    Node solved;
    solved = search(puzzle, searchType);

    if (solved.cost >= 0) {
        cout << "Success!" << endl;
        printNode(solved);
    }

}