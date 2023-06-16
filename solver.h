#ifndef SOLVER_H
#define SOLVER_H

#include <vector>
#include <map>
#include <iostream>

#include "grid.h"
#include "object.h"
#include "util.h"

using std::vector;
using std::map;
using std::cout;
using std::endl;

class Solver {
    public:
    const int dx[4] = {01, 00, -1, 00};
    const int dy[4] = {00, 01, 00, -1};
    vector<pair<int, int>> solution;
    map<pair<int, int>, pair<int, int>> vis;
    
    pair<int, int> origin;
    
    Grid grid;
    
    Solver() {
        grid = Grid();
        solution = vector<pair<int, int>>();
    }
    
    Solver(Grid g) {
        grid = g;
        solution = vector<pair<int, int>>();
    }
    
    void set(Grid g) {
        grid = g;
    }
    
    void path(pair<int, int> src, pair<int, int> prev) {
        // cout << "[" << src.first << " " << src.second << "]\n";
        if (solution.size() > 0) return;
        if ((grid.ends).find(src) != (grid.ends).end()) {
            grid.board[src.first][src.second]->isPathOccupied = true;
            // cout << "ENDPOINT " << src.first << " " << src.second << endl;
            grid.to_string(); // Somehow this is needed. I don't know why.
            
            bool check = grid.ver(origin.first, origin.second);
            // cout << (check ? "PASSED\n" : "FAILED\n");
            if (check) {
                // cout << "SOLUTION FOUND" << endl;
                vis.insert({src, prev});
                // for (auto i : vis) cout << "[" << i.first.first << " " << i.first.second << "] [" << i.second.first << " " << i.second.second << "]\n";
                while (vis.find(src) != vis.end() && vis.at(src) != src) {
                    // cout << src.first << " " << src.second << endl;
                    solution.push_back(src);
                    src = vis.at(src);
                }
                solution.push_back(src);
                reverse(solution.begin(), solution.end());
            }
            
            grid.board[src.first][src.second]->isPathOccupied = false;
            return;
        }
        
        vis.insert({src, prev});
        grid.board[src.first][src.second]->isPathOccupied = true;
        
        for (int i = 0; i < 4; i++) {
            pair<int, int> next = {src.first + dx[i], src.second + dy[i]};
            if (!grid.inside(next)) continue;
            if (!grid.board[next.first][next.second]->isPath) continue;
            if (vis.find(next) != vis.end()) continue;
            path(next, src);
        }
        vis.erase(vis.find(src));
        grid.board[src.first][src.second]->isPathOccupied = false;
    }
    
    vector<pair<int, int>> solve() {
        // cout << "SOLVING" << endl;
        solution.clear();
        for (auto i : grid.starts) {
            origin = i;
            // cout << i.first << " " << i.second << endl;
            vis.clear();
            vis.insert({i, i});
            path(i, i);
            if (solution.size() > 0) break;
        }
        return solution;
    }
    
    string to_string() {
        for (auto i : solution) grid.board[i.first][i.second]->isPathOccupied = true;
        string res = grid.to_string();
        for (auto i : solution) grid.board[i.first][i.second]->isPathOccupied = false;
        return res;
    }
    
    void disp() {
        cout << to_string() << endl;
    }
};



#endif