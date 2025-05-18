#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <fstream>
#include <string>
using namespace std;

// --------- Banker's Algorithm Logic ---------
bool runBankers() {
    ifstream input("banker_input.txt");
    int n, m;
    input >> n >> m;
    vector<vector<int>> alloc(n, vector<int>(m));
    vector<vector<int>> max(n, vector<int>(m));
    vector<int> avail(m);

    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
            input >> alloc[i][j];

    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
            input >> max[i][j];

    for (int i = 0; i < m; ++i)
        input >> avail[i];

    vector<int> work = avail;
    vector<bool> finish(n, false);
    vector<int> safeSeq;

    while (safeSeq.size() < n) {
        bool found = false;
        for (int i = 0; i < n; ++i) {
            if (!finish[i]) {
                bool canFinish = true;
                for (int j = 0; j < m; ++j) {
                    if (max[i][j] - alloc[i][j] > work[j]) {
                        canFinish = false;
                        break;
                    }
                }
                if (canFinish) {
                    for (int j = 0; j < m; ++j)
                        work[j] += alloc[i][j];
                    finish[i] = true;
                    safeSeq.push_back(i);
                    found = true;
                }
            }
        }
        if (!found) break;
    }

    if (safeSeq.size() == n) {
        cout << "No Deadlock Detected (Banker's Algorithm)\n";
        cout << "Safe Sequence: ";
        for (int id : safeSeq)
            cout << "P" << id << " ";
        cout << endl;
        return true;
    } else {
        cout << "Deadlock Detected (Banker's Algorithm)\n";
        return false;
    }
}

// --------- RAG Cycle Detection Logic ---------
unordered_map<string, vector<string>> graph;
unordered_set<string> visited, recStack;

bool isCyclic(string node) {
    if (recStack.count(node)) return true;
    if (visited.count(node)) return false;

    visited.insert(node);
    recStack.insert(node);

    for (const string& neighbor : graph[node]) {
        if (isCyclic(neighbor))
            return true;
    }

    recStack.erase(node);
    return false;
}

bool runRAG() {
    ifstream input("graph.txt");
    string from, to;
    while (input >> from >> to)
        graph[from].push_back(to);

    for (auto& node : graph) {
        if (!visited.count(node.first)) {
            if (isCyclic(node.first)) {
                cout << "Deadlock Detected (RAG)\n";
                return true;
            }
        }
    }
    cout << "No Deadlock Detected (RAG)\n";
    return false;
}

// --------- Main Control Logic ---------
int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Error: No algorithm specified. Use 'banker' or 'rag'\n";
        return 1;
    }

    string method = argv[1];
    if (method == "banker") {
        runBankers();
    } else if (method == "rag") {
        runRAG();
    } else {
        cout << "Invalid method. Use 'banker' or 'rag'\n";
    }

    return 0;
}
