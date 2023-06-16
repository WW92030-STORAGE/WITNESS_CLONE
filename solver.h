#ifndef SOLVER_H
#define SOLVER_H

#include <vector>
#include <map>
#include <iostream>
#include <algorithm>

#include "grid.h"
#include "object.h"
#include "util.h"

using std::vector;
using std::map;
using std::cout;
using std::endl;
using std::reverse;

class Solver {
    public:
    const int dx[4] = {01, 00, -1, 00};
    const int dy[4] = {00, 01, 00, -1};
    vector<pair<int, int>> solution;
    map<pair<int, int>, pair<int, int>> vis;
    
    pair<int, int> origin;
    
    Grid grid;
    
    Solver();
    
    Solver(Grid g);
    
    void set(Grid g);
    
    void path(pair<int, int> src, pair<int, int> prev);
    
    vector<pair<int, int>> solve();
    
    string to_string();
    
    void disp();
};



#endif