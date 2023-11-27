#include "blockgroup.h"

#include <set>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

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
    
    void BlockGroup::updateBounds() {
        bottomleft = {INT_MAX, INT_MAX};
        topright = {INT_MIN, INT_MIN};
        
        for (auto p : pairs) {
            bottomleft.first = min(bottomleft.first, p.first);
            bottomleft.second = min(bottomleft.second, p.second);
            topright.first = max(topright.first, p.first);
            topright.second = max(topright.second, p.second);
        }
            
        boundingbox = {topright.first - bottomleft.first + 1, topright.second - bottomleft.second + 1};
    }
    
    // Utility Functions
    
    bool BlockGroup::contains(pair<int, int> p) {
        return pairs.find(p) != pairs.end();
    }
    
    void BlockGroup::add(pair<int, int> p) {
        if (contains(p)) return;
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
        n = p.size();
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
    
    bool BlockGroup::containsbb(BlockGroup b) { // Can the bounding box contain that belonging to region b?
        if (boundingbox.first < b.boundingbox.first) return false;
        if (boundingbox.second < b.boundingbox.second) return false;
        return true;
    }
    
    bool BlockGroup::directoverlay(BlockGroup b) { // Does this region contain region b in terms of absolute coordinates?
    if (n < b.n) return false;
        for (pair<int, int> p : b.pairs) {
            if (!contains(p)) return false;
        }
        return true;
    }
    
    vector<pair<int, int>> BlockGroup::fixedoverlay(BlockGroup b) { // Does this region contain region b? Return the offset if yes, INT_MIN if no.
        // This only affects one rotation.
        if (n < b.n) return vector<pair<int, int>>();
        // the starting offset is the absolute difference in bounding boxes
        // the ending offset is the width of the smaller bounding box in the larger one
        
        int dx = bottomleft.first - b.bottomleft.first;
        int dy = bottomleft.second - b.bottomleft.second;
        
        BlockGroup test = b.clone();
        test.move({dx, dy});
        
        // cout << "MOVED " << endl;
        // disp();
        // test.disp();
        
        int width = abs(boundingbox.first - b.boundingbox.first);
        int height = abs(boundingbox.second - b.boundingbox.second);
        
        vector<pair<int, int>> res;
        // cout << "EFFECTIVE BOX " << width << " " << height << endl;
        for (int i = 0; i <= width; i++) {
            for (int j = 0; j <= height; j++) {
                // test.disp();
                if (directoverlay(test)) res.push_back({i, j});
                test.move({0, 1});
            }
            
            test.move({1, -1 * height - 1});
        }
        
        return res;
    }
    
    vector<vector<pair<int, int>>> BlockGroup::overlay(BlockGroup b) {
        vector<vector<pair<int, int>>> res(4, vector<pair<int, int>>());
        res[0] = fixedoverlay(b);
        if (!b.oriented) {
            BlockGroup test = b.clone();
            test.normalize();
            for (int i = 0; i < 3; i++) {
                test.rotate(1);
                test.normalize();
                res[i + 1] = (fixedoverlay(test));
            }
        }
        
        return res;
    }
    
    // Now for the real thing
    
    bool BlockGroup::dfsUtil(BlockGroup region, vector<BlockGroup>& v, int index) {
        // cout << index << " ";
        // region.disp();
        if ((size_t)index >= v.size()) return region.n == 0;
        
        BlockGroup group = v[index].clone();
        group.normalize();
        vector<vector<pair<int, int>>> options = region.overlay(group);
        
        bool res = false;
        for (int i = 0; i < 4; i++) {
            group.move(region.bottomleft);
            for (auto op : options[i]) {
                group.move(op);
                region.removeRegion(group);;
                res |= dfsUtil(region, v, index + 1);
                if (res) return true;
                region.addRegion(group);
                group.invmov(op);
                
            }
            if (group.oriented) break;
            group.rotate(1);
            group.normalize();
        }
        
        return res;
    }
    
    bool BlockGroup::solve(vector<BlockGroup> v) {
        int diff = n;
        for (auto i : v) {
            if (i.sub) diff += i.n;
            else diff -= i.n;
        }
        if (diff != 0) return false;
        return dfsUtil(clone(), v, 0);
    }
