#ifndef BLOCKGROUP_H
#define BLOCKGROUP_H

#include <set>
#include <iostream>
#include <vector>
#include <string>
#include <climits>

#include "object.h"

using std::set;
using std::pair;
using std::vector;
using std::string;

class BlockGroup : public Object {
    public:
    bool oriented;
    bool sub;
    set<pair<int, int>> pairs;
    int n;
    
    pair<int, int> bottomleft;
    pair<int, int> topright;
    pair<int, int> boundingbox;
    
    const pair<int, int> NIL = {INT_MIN, INT_MIN};
    
    BlockGroup(bool orientation, bool subtractive, vector<pair<int, int>> v, Color c);
    
    BlockGroup(bool orientation, bool subtractive, vector<pair<int, int>> v);
    
    // Utility Functions
    
    bool contains(pair<int, int> p);
    
    void add(pair<int, int> p);
    
    void remove(pair<int, int> p);
    
    void reset(vector<pair<int, int>>& p);
    
    BlockGroup clone();
    
    // General Functions
    
    void rotate(int x);
    
    void move(pair<int, int> p);
    
    void invmov(pair<int, int> p);
    
    void normalize();
    
    void removeRegion(BlockGroup x);
    
    void addRegion(BlockGroup x);
    
    string to_string();
    
    void disp();
    
    // Region testing
    
    bool directoverlay(BlockGroup b);
    
    vector<vector<bool>> generateMatrix(vector<BlockGroup>& blocks);
    
    bool solve(vector<BlockGroup>& blocks);
};

#endif
