#ifndef GRID_H
#define GRID_H

#include <vector>
#include <set>
#include <string>
#include <iostream>
#include <memory>
#include "object.h"
#include "util.h"
using std::vector;
using std::string;
using std::set;

class Grid {
    public:
    int n;
    int m;
    vector<vector<std::shared_ptr<Object>>> board;
    set<pair<int, int>> starts;
    set<pair<int, int>> ends;
    set<pair<int, int>> dots;
    set<pair<int, int>> triangles;
    set<pair<int, int>> blobs;
    set<pair<int, int>> stars;
    set<pair<int, int>> blocks;
    set<pair<int, int>> cancels;
    set<pair<int, int>> ignored; // ignored cancel operations
    
    pair<int, int> begin;
    
    Grid();
    
    Grid(vector<vector<std::shared_ptr<Object>>>& v);
    
    void defaultGrid();
    
    void drawLine(pair<int, int> a, pair<int, int> b);
    
    void drawPath(vector<pair<int, int>> v);
    
    virtual ~Grid();
    
    string to_string();
    
    void disp();
    
    // The verification algorithm
    
    bool inside(pair<int, int> p);
    
    bool ver(int sx, int sy);
    
    bool check();
    
    bool validateRegion(int sx, int sy, vector<pair<int, int>> ban);
};

#endif