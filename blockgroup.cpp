#include "blockgroup.h"
#include "cover.h"

#include <set>
#include <iostream>
#include <vector>
#include <string>
#include <map>

using std::set;
using std::cout;
using std::endl;
using std::pair;
using std::make_pair;
using std::vector;
using std::string;
using std::min;
using std::max;

BlockGroup::BlockGroup(bool orientation, bool subtractive, vector<pair<int, int>> v, Color c) : Object() {
        oriented = orientation;
        sub = subtractive;
        pairs = set<pair<int, int>>();
        n = v.size();
        bottomleft = {INT_MAX, INT_MAX};
        topright = {INT_MIN, INT_MIN};
        for (int i = 0; i < n; i++) {
            pairs.insert(make_pair(v[i].first, v[i].second));
            bottomleft.first = min(bottomleft.first, v[i].first);
            bottomleft.second = min(bottomleft.second, v[i].second);
            topright.first = max(topright.first, v[i].first);
            topright.second = max(topright.second, v[i].second);
        }
        
        boundingbox = {topright.first - bottomleft.first + 1, topright.second - bottomleft.second + 1};
        
        color = c;
    }
    
    BlockGroup::BlockGroup(bool orientation, bool subtractive, vector<pair<int, int>> v) : Object() {
        oriented = orientation;
        sub = subtractive;
        pairs = set<pair<int, int>>();
        n = v.size();
        bottomleft = {INT_MAX, INT_MAX};
        topright = {INT_MIN, INT_MIN};
        for (int i = 0; i < n; i++) {
            pairs.insert(make_pair(v[i].first, v[i].second));
            bottomleft.first = min(bottomleft.first, v[i].first);
            bottomleft.second = min(bottomleft.second, v[i].second);
            topright.first = max(topright.first, v[i].first);
            topright.second = max(topright.second, v[i].second);
        }
        
        boundingbox = {topright.first - bottomleft.first + 1, topright.second - bottomleft.second + 1};
        
        color = YELLOW;
    }
    
    // Utility Functions
    
    bool BlockGroup::contains(pair<int, int> p) {
        return pairs.find(p) != pairs.end();
    }
    
    void BlockGroup::add(pair<int, int> p) {
        pairs.insert(p);
        n++;
        return;
    }
    
    void BlockGroup::remove(pair<int, int> p) {
        if (!contains(p)) return;
        pairs.erase(pairs.find(p));
        n--;
        return;
    }
    
    void BlockGroup::reset(vector<pair<int, int>>& p) {
        pairs.clear();
        bottomleft = {INT_MAX, INT_MAX};
        topright = {INT_MIN, INT_MIN};
        for (pair<int, int> x : p) {
            pairs.insert(x);
            bottomleft.first = min(bottomleft.first, x.first);
            bottomleft.second = min(bottomleft.second, x.second);
            topright.first = max(topright.first, x.first);
            topright.second = max(topright.second, x.second);
        }
        
        boundingbox = {topright.first - bottomleft.first + 1, topright.second - bottomleft.second + 1};
    }
    
    BlockGroup BlockGroup::clone() {
        vector<pair<int, int>> res;
        for (auto i : pairs) res.push_back(make_pair(i.first, i.second));
        return BlockGroup(oriented, sub, res);
    }
    
    // General Functions
    
    void BlockGroup::rotate(int x) {
        while (x < 0) x += 1000;
        x %= 4;
        if (x <= 0) return;
        rotate(x - 1);
        vector<pair<int, int>> res;
        for (auto i : pairs) res.push_back(make_pair(-1 * i.second, i.first));
        
        reset(res);
    }
    
    void BlockGroup::move(pair<int, int> p) {
        vector<pair<int, int>> res;
        for (auto i : pairs) res.push_back(make_pair(i.first + p.first, i.second + p.second));
        reset(res);
    }
    
    void BlockGroup::invmov(pair<int, int> p) {
        vector<pair<int, int>> res;
        for (auto i : pairs) res.push_back(make_pair(i.first - p.first, i.second - p.second));
        reset(res);
    }
    
    void BlockGroup::normalize() {
        invmov(bottomleft);
    }
    
    void BlockGroup::removeRegion(BlockGroup x) {
        for (pair<int, int> p : x.pairs) remove(p);
    }
    
    void BlockGroup::addRegion(BlockGroup x) {
        for (pair<int, int> p : x.pairs) add(p);
    }
    
    string BlockGroup::to_string() {
        string res = "[O = ";
        res = res.append(oriented ? "1" : "0");
        res = res + " S = ";
        res = res.append(sub ? "1" : "0");
        res = res + " N = ";
        res = res.append(std::to_string(n));
        res = res + "] = [";
        for (auto i : pairs) {
            res = res + "<";
            res = res.append(std::to_string(i.first));
            res = res + " ";
            res = res.append(std::to_string(i.second));
            res = res + ">";
        }
        return res + "]";
    }
    
    void BlockGroup::disp() {
        cout << to_string() << endl;
    }
    
    // Region testing
    
    bool BlockGroup::directoverlay(BlockGroup b) { // Does this region contain region b in terms of absolute coordinates?
    if (n < b.n) return false;
        for (pair<int, int> p : b.pairs) {
            if (!contains(p)) return false;
        }
        return true;
    }
    
    vector<vector<bool>> BlockGroup::generateMatrix(vector<BlockGroup>& blocks) { // Translates a region and polys into a matrix
        normalize();
        for (int i = 0; i < blocks.size(); i++) blocks[i].normalize();
        
        vector<vector<bool>> res;
        
        std::map<pair<int, int>, int> regionIndex;
        
        for (auto p : pairs) regionIndex.insert({p, regionIndex.size()});
        
        int drudes = blocks.size();
        int regsz = max((int)regionIndex.size(), n);
        if (regionIndex.size() != n) {
            for (int i = 0; i < 1024; i++) cout << "BAD REGION CALCS!!!";
        }
        
        vector<bool> row;
        
        for (int index = 0; index < blocks.size(); index++) {
            BlockGroup block = blocks[index].clone();
            
            for (int rot = 0; rot < ((block.oriented) ? 1 : 4); rot++) {
            
            for (int x = bottomleft.first; x <= topright.first; x++) {
                for (int y = bottomleft.second; y <= topright.second; y++) {
                    row = vector<bool> (drudes + regsz, false);
                    row[index] = true; // Index of the block in question
                    
                    block.move({x, y});
                    if (directoverlay(block)) {
                        for (auto p : block.pairs) {
                            int posindex = (regionIndex.find(p) != regionIndex.end()) ? regionIndex.at(p) : 0;
                            row[drudes + posindex] = true;
                        }
                        res.push_back(row);
                    }
                    
                    
                    
                    block.normalize();
                }
            }
            
            block.rotate(1);
            
            }
        }
        
        return res;
    }
    
    bool BlockGroup::solve(vector<BlockGroup>& blocks) {
        vector<vector<bool>> mat = generateMatrix(blocks);
        if (mat.size() == 0) return false;
        dlx.translate(mat);
        return dlx.solve(0);
    }
