#ifndef GRID_H
#define GRID_H

#include <vector>
#include <set>
#include <string>
#include <iostream>
#include "object.h"
#include "util.h"
using std::vector;
using std::string;
using std::set;

class Grid {
    public:
    int n;
    int m;
    vector<vector<Object*>> board;
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
    
    Grid(vector<vector<Object*>> v);
    
    void defaultGrid();
    
    void drawLine(pair<int, int> a, pair<int, int> b);
    
    virtual ~Grid();
    
    
    string to_string();
    
    void disp();
    
    // The verification algorithm
    
    bool inside(pair<int, int> p);
    
    bool ver(int sx, int sy);
    
    bool check();
};

#endif