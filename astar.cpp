#include <vector>
#include <queue>

using namespace std;

vector<vector<int>> goal_state = {
    {1, 2, 3},
    {4, 5, 6},
    {7, 8, 9}
};

bool search (const vector<vector<int>>& initialState, int searchType) {
    queue<vector<vector<int>>> nodes;
    nodes.push(initialState);

    while (!nodes.empty()) {
        vector<vector<int>> currNode = nodes.front();

        if (currNode == goal_state) {
            return true;
        }
        else {
            
        }
    }

    return false;

}

int main () {




}