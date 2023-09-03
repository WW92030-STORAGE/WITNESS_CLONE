#include "solver.h"

#include <iostream>
#include <set>
#include <ctime>
#include <esp_random.h>
using std::set;
using std::pair;

Solver::Solver() {
        solution = vector<pair<int, int>>();
    }
    
    Solver::Solver(Grid& g) {
        grid = g;
        solution = vector<pair<int, int>>();
    }
    
    void Solver::set(Grid& g) {
        grid = g;
        solution.clear();
    }
    
    void Solver::path(pair<int, int> src, pair<int, int> prev) {
        callstopath++;
        // cout << "[" << src.first << " " << src.second << "]\n";
        if (solution.size() > 0) return;
        if ((grid.ends).find(src) != (grid.ends).end()) {
            grid.board[src.first][src.second]->isPathOccupied = true;
            // cout << "ENDPOINT " << src.first << " " << src.second << endl;
            // grid.disp();
            
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
        
        // Basic pruning action
        // This can be toggled by changing the loop constraints.
        
        for (int ii = 0; ii < 4; ii++) {
            pair<int, int> x0 = {src.first + dx[(ii + 0) % 4], src.second + dy[(ii + 0) % 4]};
            pair<int, int> x1 = {src.first + dx[(ii + 1) % 4], src.second + dy[(ii + 1) % 4]};
            // pair<int, int> x2 = {src.first + dx[(ii + 2) % 4], src.second + dy[(ii + 2) % 4]};
            pair<int, int> x3 = {src.first + dx[(ii + 3) % 4], src.second + dy[(ii + 3) % 4]};
            bool blocked0 = !grid.inside(x0);
            bool blocked1 = !grid.inside(x1) || grid.board[x1.first][x1.second]->isPathOccupied;
            // bool blocked2 = !grid.inside(x2);
            bool blocked3 = !grid.inside(x3) || grid.board[x3.first][x3.second]->isPathOccupied;
            
            vector<pair<int, int>> banned({src, x0});
            
            if (blocked0 && !blocked1 && !blocked3) {
                // cout << "BLOCKED!!!  " << src.first << " " << src.second << endl;
                // grid.disp();
                bool r1 = grid.validateRegion(x1.first, x1.second, banned);
                bool r3 = grid.validateRegion(x3.first, x3.second, banned);
                
                if (!r1 && !r3) {
                    // cout << "INVALID" << endl;
                    return;
                }
                break;
            }
        }
        
        vis.insert({src, prev});
        grid.board[src.first][src.second]->isPathOccupied = true;
        
        srand(esp_random());
        int offset = rand() % 4;
        
        for (int ii = 0; ii < 4; ii++) {
            int i = (ii + offset) % 4;
            pair<int, int> next = {src.first + dx[i], src.second + dy[i]};
            if (!grid.inside(next)) continue;
            if (!grid.board[next.first][next.second]->isPath) continue;
            if (vis.find(next) != vis.end()) continue;
            path(next, src);
        }
        vis.erase(vis.find(src));
        grid.board[src.first][src.second]->isPathOccupied = false;
    }
    
    vector<pair<int, int>> Solver::solve() {
        // cout << "SOLVING" << endl;
        callstopath = 0;
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
    
    string Solver::to_string() {
        for (auto i : grid.board) {
            for (auto j : i) j->isPathOccupied = false;
        }
        for (auto i : solution) grid.board[i.first][i.second]->isPathOccupied = true;
        string res = grid.to_string();
        for (auto i : solution) grid.board[i.first][i.second]->isPathOccupied = false;
        return res;
    }
    
    void Solver::disp() {
        cout << to_string() << endl;
        cout << callstopath << " CALLS TO PATH\n\n";
    }
    
    void Solver::activate() {
      for (auto i : grid.board) {
        for (auto j : i) j->isPathOccupied = false;
      }
      for (auto i : solution) grid.board[i.first][i.second]->isPathOccupied = true;
    }

    void Solver::deactivate() {
      for (auto i : grid.board) {
        for (auto j : i) j->isPathOccupied = false;
      }
    }
