#include <vector>
#include <set>
#include <map>
#include <queue>
#include <random>

#include "grid.h"

using std::vector;
using std::set;
using std::make_pair;
using std::queue;
using std::mt19937;

// not really feeling like putting splitting header + implementation this time ... might do that later ...

class RandGrid { // RandGrid generates 4x4 (internally 9x9) puzzle grids.
    public:
    vector<int> dx;
    vector<int> dy;
    
    vector<set<pair<int, int>>> possiblePaths;
    vector<set<pair<int, int>>> gridRegions;
    
    pair<int, int> start;
    pair<int, int> end;
    
    mt19937 gen;

    bool singlepath;
    
    RandGrid() {
        gen = mt19937(time(0));
        dx = vector<int>({01, 00, -1, 00});
        dy = vector<int>({00, 01, 00, -1});
        possiblePaths.clear();
        
        start = {8, 0}; // The grid is still double in size however points with both coordinates odd cannot be traversed.
        end = {0, 8};

        singlepath = true;
        
        // pathfind();
    }
    
    RandGrid(pair<int, int> a, pair<int, int> b) {
        dx = vector<int>({01, 00, -1, 00});
        dy = vector<int>({00, 01, 00, -1});
        possiblePaths.clear();
        
        start = a;
        end = b;
        
        // pathfind();
    }
    
    void reset(pair<int, int> a, pair<int, int> b) {
        start = a;
        end = b;
        pathfind();
    }
    
    int randint(int b) { // [0, b) https://stackoverflow.com/questions/5008804/generating-a-random-integer-from-a-range
        
        int pow = 1;
        while (pow < b) pow = pow<<1;
        
        int res = INT_MAX;
        while (res >= b) res = gen() % pow;
        return res;
    }
    
    // Generate a random grid containing two pairs of stars (two colors).
    
    Grid randStars() {
        vector<vector<Object*>> v = vector<vector<Object*>>(9, vector<Object*>(9));
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                v[i][j] = new Object();
            }
        }

        set<pair<int, int>> ps;
        set<pair<int, int>> ps2;
        while (ps.size() < 8) {
            pair<int, int> p = make_pair(randint(4), randint(4));
            ps.insert(p);
        }

        int index = 0;
        for (auto i : ps) {
            if (index & 1) v[i.first * 2 + 1][i.second * 2 + 1] = new Star(RED);
            else v[i.first * 2 + 1][i.second * 2 + 1] = new Star(CYAN);
            index++;
        }
        
        v[start.first][start.second] = new Endpoint(true);
        v[end.first][end.second] = new Endpoint(false);

        Grid grid = Grid(v);
        grid.defaultGrid();
    
        return grid;
    }
    
    // A hamiltonian path has 25 vertices and thus 24 edges.
    // Theree are 20 + 20 = 40 edges available.
    
    Grid randMaze() { // Generates a grid containing 12 cutoffs in (1<<16) attempts to get the edges.
        set<pair<int, int>> path = possiblePaths[randint(possiblePaths.size())];
        
        vector<vector<Object*>> v(9, vector<Object*>(9));
        
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                v[i][j] = new Object();
                if (i % 2 == 0 || j % 2 == 0) v[i][j]->isPath = true;
            }
        }
        
        set<pair<int, int>> cuts;
        
        int count = 0;
        while (cuts.size() < 12 && count < (1<<16)) {
            int x = randint(9);
            int y = randint(9);
            if (path.find({x, y}) != path.end()) continue;
            if ((x % 2 == 0) ^ (y % 2 == 0)) cuts.insert({x, y});
            count++;
        }
        
        for (auto i : cuts) v[i.first][i.second]->isPath = false;
        
        v[start.first][start.second] = new Endpoint(true);
        v[end.first][end.second] = new Endpoint(false);
        
        return Grid(v);
    }
    
    Grid randBlobs(int numBlobs, int numCols, int numCuts) { // Generates a grid containing up to numBlobs blobs. This grid also has up to numCuts cuts.
        set<pair<int, int>> path = possiblePaths[randint(possiblePaths.size())];
        getRegions(path);
        while (gridRegions.size() < numCols) {
          pathfind();
          path = possiblePaths[randint(possiblePaths.size())];
          getRegions(path);
        }
        
        vector<vector<Object*>> v(9, vector<Object*>(9));
        
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                v[i][j] = new Object();
                if (i % 2 == 0 || j % 2 == 0) v[i][j]->isPath = true;
            }
        }
        
        set<pair<int, int>> things;
        
        int count = 0;
        while (things.size() < numCuts && count < (1<<16)) {
            int x = randint(9);
            int y = randint(9);
            if (path.find({x, y}) != path.end()) continue;
            if ((x % 2 == 0) ^ (y % 2 == 0)) things.insert({x, y});
            count++;
        }
        
        for (auto i : things) v[i.first][i.second]->isPath = false;
        
        v[start.first][start.second] = new Endpoint(true);
        v[end.first][end.second] = new Endpoint(false);
        
        // Generate the blobs.

        things.clear();
        
        count = 0;
        
        while (things.size() < numBlobs && count < (1<<16)) {
            int x = 1 + 2 * randint(4);
            int y = 1 + 2 * randint(4);
            
            things.insert({x, y});
            
            count++;
        }
        
        vector<Color> colors({RED, GREEN, BLUE, YELLOW, CYAN, MAGENTA, WHITE});
        
        std::random_shuffle(colors.begin(), colors.end());
        
        vector<int> perm(gridRegions.size());
        for (int i = 0; i < perm.size(); i++) perm[i] = i;
        
        std::random_shuffle(perm.begin(), perm.end());
        
        for (auto i : things) {
            int reg = -1;
            for (int index = 0; index < gridRegions.size(); index++) {
                if (reg >= 0) break;
                if (gridRegions[index].find(i) != gridRegions[index].end()) reg = index;
            }
            v[i.first][i.second] = new Blob(colors[perm[reg] % numCols]);
        }
        
        // Return the grid.
        
        Grid grid = Grid(v);
        
        return Grid(v);
    }

    Grid randTriangles(int numTriangles, int numCuts) { // Generates a grid with up to numTriangles triangles and up to numCuts cuts
        set<pair<int, int>> path = possiblePaths[randint(possiblePaths.size())];
        
        vector<vector<Object*>> v(9, vector<Object*>(9));
        
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                v[i][j] = new Object();
                if (i % 2 == 0 || j % 2 == 0) v[i][j]->isPath = true;
            }
        }
        
        set<pair<int, int>> things;
        
        int count = 0;
        while (things.size() < numCuts && count < (1<<16)) {
            int x = randint(9);
            int y = randint(9);
            if (path.find({x, y}) != path.end()) continue;
            if ((x % 2 == 0) ^ (y % 2 == 0)) things.insert({x, y});
            count++;
        }
        
        for (auto i : things) v[i.first][i.second]->isPath = false;
        
        v[start.first][start.second] = new Endpoint(true);
        v[end.first][end.second] = new Endpoint(false);
        
        // Get the triangles
        
        things.clear();
        
        count = 0;
        
        while (things.size() < numTriangles && count < (1<<16)) {
            int x = 1 + 2 * randint(4);
            int y = 1 + 2 * randint(4);
            
            things.insert({x, y});
            
            count++;
        }
        
        for (auto i : things) {
            count = 0;
            for (int d = 0; d < 4; d++) {
                int xp = i.first + dx[d];
                int yp = i.second + dy[d];
                if (path.find({xp, yp}) != path.end()) count++;
            }
            if (count > 0) v[i.first][i.second] = new Triangle(count);
        }
        
        // Return the grid
        
        Grid grid = Grid(v);
        
        return Grid(v);
    }

    Grid randBlocks(int numPoly, int numBlocks) { // Generates a grid with up to numPoly regions represented as blocks.
        // Each generated region is a block unless over numBlocks in which it gets separated into ceil(size / numBlocks) blocks (individual blocks can have more than numBlocks cells).
        // Even with numPoly() the upper bound is gridRegions.size() / 2 as a safety measure.
        
        int chosenPath = randint(possiblePaths.size());
        set<pair<int, int>> path = possiblePaths[chosenPath];
        getRegions(path);
        
        while (gridRegions.size() < numPoly) { // At most 2 blocks per region
            pathfind();
            path = possiblePaths[randint(possiblePaths.size())];
            getRegions(path);
        }
        
        vector<vector<Object*>> v(9, vector<Object*>(9));
        
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) v[i][j] = new Object();
        }

        v[start.first][start.second] = new Endpoint(true);
        v[end.first][end.second] = new Endpoint(false);
        
        vector<pair<int, int>> points; // Points in a region, compressed into block form
        vector<pair<int, int>> gridpoints; // Points in a region, in raw
        set<pair<int, int>> things; // multipurpose set
        vector<pair<int, int>> thingsvec; // multipurpose vector

        vector<vector<pair<pair<int, int>, BlockGroup*>>> groupies(gridRegions.size(), vector<pair<pair<int, int>, BlockGroup*>>()); // List of blocks and their placements (inner index) by region (outer index)

        // For now every region is a block

        int regionCount = 0;
        for (auto region : gridRegions) {
          points.clear();
          gridpoints.clear();
          things.clear();
          thingsvec.clear();
          for (auto p : region) {
            if (p.first % 2 == 0 || p.second % 2 == 0) continue;
            pair<int, int> thing = make_pair(p.first>>1, p.second>>1);
            points.push_back(thing);
            gridpoints.push_back(p);
          }

          int subs = std::ceil((double)(gridpoints.size()) / (double)(numBlocks));

          vector<vector<pair<int, int>>> subregions(subs, vector<pair<int, int>>()); // List of subregions (inner index), for each region (outer index)

          while (things.size() < subs) {
            things.insert(points[randint(points.size())]);
          }

          for (auto i : things) thingsvec.push_back(i);

          for (auto point : points) {
            int mindist = INT_MAX;
            int minindex = 0;
            for (int i = 0; i < thingsvec.size(); i++) {
              int mandist = abs(point.first - thingsvec[i].first) + abs(point.second - thingsvec[i].second);
              if (mandist < mindist) {
                mindist = mandist;
                minindex = i;
              }
            }

            subregions[minindex].push_back(point);
          }

          things.clear();
          thingsvec.clear();
          while (things.size() < subs) {
            things.insert(gridpoints[randint(points.size())]);
          }
          for (auto i : things) thingsvec.push_back(i);

          int thesize = std::min(thingsvec.size(), subregions.size());

          for (int i = 0; i < thesize; i++) groupies[regionCount].push_back({thingsvec[i], new BlockGroup(randint(4) == 0 ? 0 : 1, 0, subregions[i])});

          regionCount++;
        }

        vector<bool> bitmask(gridRegions.size(), false);
        for (int i = 0; i < numPoly && i < gridRegions.size() / 2; i++) bitmask[i] = true;
        std::random_shuffle(bitmask.begin(), bitmask.end());

        for (auto i : bitmask) cout << i << " ";
        cout << endl;

        for (int i = 0; i < gridRegions.size(); i++) {
          if (!bitmask[i]) continue;
          for (auto p : groupies[i]) v[p.first.first][p.first.second] = p.second;
        }
        
        Grid grid = Grid(v);
        grid.defaultGrid();
        return grid;
    }
    
    // Get all regions for a path
    
    void getRegions(set<pair<int, int>> path) {
        gridRegions.clear();
        
        set<pair<int, int>> vis;
        set<pair<int, int>> area;
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                pair<int, int> src = {i, j};
                if (vis.find(src) != vis.end()) continue;
                if (path.find(src) != path.end()) continue;
                set<pair<int, int>> area;
                
                queue<pair<int, int>> q;
                q.push(src);
                while (q.size() > 0) {
                    pair<int, int> now = q.front();
                    q.pop();
                    
                    for (int d = 0; d < 4; d++) {
                        pair<int, int> next = {now.first + dx[d], now.second + dy[d]};
                        if (!inside(next)) continue;
                        if (path.find(next) != path.end()) continue;
                        if (vis.find(next) != vis.end()) continue;
                        
                        area.insert(next);
                        vis.insert(next);
                        q.push(next);
                    }
                }
                
                gridRegions.push_back(area);
            }
        }
        
        // cout << gridRegions.size() << " REGIONS FOUND" << endl;
    }
    
    // Get all paths.
    
    std::map<pair<int, int>, pair<int, int>> parent;
    
    bool inside(pair<int, int> x) {
        if (x.first < 0 || x.second < 0) return false;
        if (x.first >= 9 || x.second >= 9) return false;
        return true;
    }
    
    void dfs(pair<int, int> src, pair<int, int> prev) {
      if (singlepath && possiblePaths.size() > 2) return;
        if (src == end) {
            parent.insert({src, prev});
            set<pair<int, int>> res;
            pair<int, int> thing = make_pair(src.first, src.second);
            if (parent.find(thing) == parent.end()) return;
            while (parent.at(thing) != thing) {
                if (parent.find(thing) == parent.end()) return;
                res.insert(make_pair(thing.first, thing.second));
                thing = parent.at(thing);
            }
            res.insert(make_pair(thing.first, thing.second));
            possiblePaths.push_back(res);
            parent.erase(parent.find(src));
            return;
        }
        
        parent.insert({src, prev});

        int offset = randint(4);
        for (int ii = 0; ii < 4; ii++) {
            int i = (ii + offset) % 4;
            pair<int, int> next = {src.first + dx[i], src.second + dy[i]};
            if (!inside(next)) continue;
            if ((next.first % 2 == 1) && (next.second % 2 == 1)) continue;
            if (parent.find(next) != parent.end()) continue;
            dfs(next, src);
        }
        
        parent.erase(parent.find(src));
    }

    void pathfind() {
        possiblePaths.clear();
        parent.clear();
        parent.insert({start, start});
        dfs(start, start);
    }
    
    void disp() {
        cout << possiblePaths.size() << " PATHS FOUND\n";
        cout << gridRegions.size() << " REGIONS FOUND\n";
    }
    
    void visualize(int x) {
        if (x < 0 || x >= possiblePaths.size()) return;
        getRegions(possiblePaths[x]);
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                if (possiblePaths[x].find({i, j}) != possiblePaths[x].end()) cout << "#";
                else {
                    bool bad = true;
                    for (int reg = 0; reg < gridRegions.size(); reg++) {
                        if (gridRegions[reg].find({i, j}) != gridRegions[reg].end()) {
                            cout << (char)(reg + 'A');
                            bad = false;
                            break;
                        }
                    }
                    if (bad) cout << ".";
                }
            }
            cout << endl;
        }
    }
};
