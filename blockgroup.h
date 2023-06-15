#include <bits/stdc++.h> 
#include <ext/pb_ds/assoc_container.hpp>
using namespace std;
using namespace __gnu_pbds;
#define pii pair<int, int>

class BlockGroup {
    public:
    bool oriented;
    bool sub;
    set<pii> pairs;
    int n;
    
    pii bottomleft;
    pii topright;
    pii boundingbox;
    
    const pii NIL = {INT_MIN, INT_MIN};
    
    BlockGroup(bool orientation, bool subtractive, vector<pii> v) {
        oriented = orientation;
        sub = subtractive;
        pairs = set<pii>();
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
    }
    
    // Utility Functions
    
    bool contains(pii p) {
        return pairs.find(p) != pairs.end();
    }
    
    void add(pii p) {
        pairs.insert(p);
        n++;
        return;
    }
    
    void remove(pii p) {
        if (!contains(p)) return;
        pairs.erase(pairs.find(p));
        n--;
        return;
    }
    
    void reset(vector<pii> p) {
        pairs.clear();
        bottomleft = {INT_MAX, INT_MAX};
        topright = {INT_MIN, INT_MIN};
        for (pii x : p) {
            pairs.insert(x);
            bottomleft.first = min(bottomleft.first, x.first);
            bottomleft.second = min(bottomleft.second, x.second);
            topright.first = max(topright.first, x.first);
            topright.second = max(topright.second, x.second);
        }
        
        boundingbox = {topright.first - bottomleft.first + 1, topright.second - bottomleft.second + 1};
    }
    
    BlockGroup clone() {
        vector<pii> res;
        for (auto i : pairs) res.push_back(make_pair(i.first, i.second));
        return BlockGroup(oriented, sub, res);
    }
    
    // General Functions
    
    void rotate(int x) {
        while (x < 0) x += 1000;
        x %= 4;
        if (x <= 0) return;
        rotate(x - 1);
        vector<pii> res;
        for (auto i : pairs) res.push_back(make_pair(-1 * i.second, i.first));
        
        reset(res);
    }
    
    void move(pii p) {
        vector<pii> res;
        for (auto i : pairs) res.push_back(make_pair(i.first + p.first, i.second + p.second));
        reset(res);
    }
    
    void invmov(pii p) {
        vector<pii> res;
        for (auto i : pairs) res.push_back(make_pair(i.first - p.first, i.second - p.second));
        reset(res);
    }
    
    void normalize() {
        invmov(bottomleft);
    }
    
    void removeRegion(BlockGroup x) {
        for (pii p : x.pairs) remove(p);
    }
    
    void addRegion(BlockGroup x) {
        for (pii p : x.pairs) add(p);
    }
    
    string to_string() {
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
    
    void disp() {
        cout << to_string() << endl;
    }
    
    // Region testing
    
    bool containsbb(BlockGroup b) { // Can the bounding box contain that belonging to region b?
        if (boundingbox.first < b.boundingbox.first) return false;
        if (boundingbox.second < b.boundingbox.second) return false;
        return true;
    }
    
    bool directoverlay(BlockGroup b) { // Does this region contain region b in terms of absolute coordinates?
    if (n < b.n) return false;
        for (pii p : b.pairs) {
            if (!contains(p)) return false;
        }
        return true;
    }
    
    vector<pii> fixedoverlay(BlockGroup b) { // Does this region contain region b? Return the offset if yes, INT_MIN if no.
        // This only affects one rotation.
        if (n < b.n) return vector<pii>();
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
        
        vector<pii> res;
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
    
    vector<vector<pii>> overlay(BlockGroup b) {
        vector<vector<pii>> res(4, vector<pii>());
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
    
    bool dfsUtil(BlockGroup region, vector<BlockGroup> v, int index) {
        // cout << index << " ";
        // region.disp();
        if (index >= v.size()) return region.n == 0;
        
        BlockGroup group = v[index].clone();
        group.normalize();
        vector<vector<pii>> options = region.overlay(group);
        
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
            group.rotate(1);
            group.normalize();
        }
        
        return res;
    }
    
    bool solve(vector<BlockGroup> v) {
        return dfsUtil(clone(), v, 0);
    }
};
