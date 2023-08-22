#include <queue>
#include <map>
#include "grid.h"
#include "object.h"
#include "util.h"

using std::queue;
using std::map;
using std::max;
using std::cout;
using std::endl;
using std::pair;
using std::make_pair;

Grid::Grid(vector<vector<Object*>>& v) { // Once a grid is created it cannot be changed unless changes are consistent across all aspects.
        n = v.size();
        m = 0;
        for (auto i : v) m = max((int)(i.size()), m);
        if (n % 2 == 0) n++;
        if (m % 2 == 0) m++;
        
        board = vector<vector<Object*>>(n, vector<Object*>(m, new Object()));
        
        starts = set<pair<int, int>>();
        ends = set<pair<int, int>>();
        
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                if (j < v[i].size()) board[i][j] = v[i][j];
                if (isStartingPoint(board[i][j])) {
                    begin = {i, j};
                    starts.insert({i, j});
                }
                if (isEndingPoint(board[i][j])) ends.insert({i, j});
                if (instanceof<Dot>(board[i][j])) dots.insert({i, j});
                if (instanceof<Blob>(board[i][j])) blobs.insert({i, j});
                if (instanceof<Star>(board[i][j])) stars.insert({i, j});
                if (instanceof<Triangle>(board[i][j])) triangles.insert({i, j});
                if (instanceof<BlockGroup>(board[i][j])) blocks.insert({i, j});
                if (instanceof<Cancel>(board[i][j])) cancels.insert({i, j});
            }
        }
    }
    
    void Grid::defaultGrid() {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                if (i % 2 == 0 || j % 2 == 0) (board[i][j])->isPath = true;
            }
        }
    }
    
    void Grid::drawLine(pair<int, int> a, pair<int, int> b) {
        if (a.first == b.first) {
            if (a.second > b.second) swap(a, b);
            for (int i = a.second; i <= b.second; i++) board[a.first][i]->isPathOccupied = true;
        }
        else if (a.second == b.second) {
            if (a.first > b.first) swap(a, b);
            for (int i = a.first; i <= b.first; i++) board[i][a.second]->isPathOccupied = true;
        }
    }
    
    void Grid::drawPath(vector<pair<int, int>> v) {
        if (v.size() < 2) return;
        for (int i = 1; i < v.size(); i++) drawLine(v[i - 1], v[i]);
    }
    
    Grid::Grid() {
        
    }
    
    Grid::~Grid() {
        
    }
    
    
    string Grid::to_string() {
        string s = "";
        for (auto i : board) {
            for (auto j : i) {
                char open = j->isPathOccupied ? '[' : (j->isPath ? '+' : '_');
                char close = j->isPathOccupied ? ']' : (j->isPath ? '+' : '_');
                s = s + open + get_type(j) + close + " ";
            }
            s = s + "\n";
        }
        return s.substr(0, s.length() - 1);
    }
    
    void Grid::disp() {
        cout << to_string() << endl;
    }
    
    // The verification algorithm
    
    bool Grid::inside(pair<int, int> p) {
        if (p.first < 0 || p.second < 0) return false;
        if (p.first >= board.size() || p.second >= board[p.first].size()) return false;
        return true;
    }
    
    bool Grid::ver(int sx, int sy) {
        // cout << "VERIFYING GRID" << endl;
        // The algorithm works in four stages: THE FOX / THE WOLF / THE DRUDE / THE PHOENIX
        
        // The first section denoted THE FOX begins by handling the more trivial matters.
        // Just as foxes are easily recognizable (and a common Fursona species), the tasks for this section are relatively self-explanatory and easy to check.
        // Namely, whether the path goes from a start to an end, whether all dots are covered, and whether triangles are solved.
        // Violations do not automatically return False. Instead, the locations of violated symbols are put into a set.
        // However, this is arguably the most important section because it establishes the trajectory of the path.
        
        const int dx[4] = {01, 00, -1, 00};
        const int dy[4] = {00, 01, 00, -1};
        if (!isStartingPoint(board[sx][sy])) return false;
        Object* o = board[sx][sy];
        if (!o->isPathOccupied) return false;
        
        // cout << "BASIC CHECK COMPLETED";
        
        set<pair<int, int>> vis;
        queue<pair<int, int>> q;
        q.push({sx, sy});
        
        bool reachedend = false;
        
        while (q.size() > 0) {
            pair<int, int> p = q.front();
            q.pop();
            vis.insert(p);
            for (int i = 0; i < 4; i++) {
                pair<int, int> next = {p.first + dx[i], p.second + dy[i]};
                if (!inside(next)) continue;
                Object* n = board[next.first][next.second];
                if (isEndingPoint(n)) reachedend = true;
                if (!n->isPath || !n->isPathOccupied) continue;
                if (vis.find(next) != vis.end()) continue;
                vis.insert(next);
                q.push(next);
            }
        }
        
        // for (auto i : vis) cout << i.first << " " << i.second << endl;
        
        if (!reachedend) return false;
        
        // cout << "REACHED END" << endl;
        
        set<pair<int, int>> violations;
        
        for (auto i : dots) {
            Object* o = board[i.first][i.second];
            if (!o->isPathOccupied) violations.insert(i);
        }
        
        for (auto i : triangles) {
            Object* o = board[i.first][i.second];
            if (!instanceof<Triangle>(o)) continue;
            int target = (dynamic_cast<Triangle*>(o))->x;
            int count = 0;
            for (int ii = 0; ii < 4; ii++) {
                int xp = i.first + dx[ii];
                int yp = i.second + dy[ii];
                if (!inside({xp, yp})) continue;
                Object* o2 = board[xp][yp];
                if (o2->isPath && o2->isPathOccupied) count++;
            }
            
            if (count != target) violations.insert(i);
        }
        
        // In the same way that wolves are largely territorial animals...
        // The second section denoted THE WOLF ensures that colors and territories are properly partitioned.
        // Namely, the blobs and the stars. Blobs cannot exist with any other color blob.
        // And stars, while they can exist with other colors, must exist with exactly one other of their color.
        // Although dots can be set as color, they do not affect the results.
        // When we traverse this time, the dx/dy effect is doubled.
        // However at each step we must also check the in between to make sure we do not hit the path.
        // The SET only contains locations of non-path coordinates.
        // Violations are measured as the lesser violation. Thus if a region contains 2 blue and 3 white dots
        // Then the 2 blue dots are marked as violation. Cancellation symbols will also ``seek'' the blue dots.
        
        map<Color, vector<Object*>> ding; // List of colors
        map<Color, int> selectedcolors;
        set<pair<int, int>> collected;
        
        vis.clear();
        
        for (auto ii : blobs) {
            if (vis.find(ii) != vis.end()) continue;
            ding.clear();
            selectedcolors.clear();
            collected.clear();
            queue<pair<int, int>> q;
            q.push(ii);
            while (q.size() > 0) {
                pair<int, int> now = q.front();
                q.pop();
                vis.insert(now);
                
                // cout << now.first << " / " << now.second << endl;
                
                Object* cur = board[now.first][now.second];
                
                if (instanceof<Blob>(cur)) {
                if (ding.find(cur->color) == ding.end()) ding.insert({cur->color, vector<Object*>()});
                (*(ding.find(cur->color))).second.push_back(cur);
                }
                
                if (instanceof<Blob>(cur)) {
                    collected.insert(now);
                    if (selectedcolors.find(cur->color) == selectedcolors.end()) selectedcolors.insert({cur->color, 0});
                    (*(selectedcolors.find(cur->color))).second++;
                }
                
                for (int i = 0; i < 4; i++) {
                    pair<int, int> mid = {now.first + dx[i], now.second + dy[i]};
                    pair<int, int> next = {now.first + dx[i] * 2, now.second + dy[i] * 2};
                    if (!inside(mid) || !inside(next)) continue;
                    Object* between = board[mid.first][mid.second];
                    Object* hit = board[next.first][next.second];
                    if (between->isPathOccupied) continue;
                    if (vis.find(next) != vis.end()) continue;
                    vis.insert(next);
                    q.push(next);
                }
            }
            
            /*
            
            cout << "BLOB FOR BLOB " << ii.first << " " << ii.second << endl;
        
            for (auto i : vis) cout << i.first << " " << i.second << endl;
            
            cout << "COLORS!!!" << endl;
            for (auto i : ding) {
                cout << i.first << endl;
                for (auto x : i.second) cout << get_type(x) << endl;
            }
            
            cout << "BLOB COLORS!!!" << endl;
            for (auto i : selectedcolors) {
                cout << i.first << " = " << i.second << endl;
            }
            
            */
            
            // Now we determine where the violations were.
            // Among all colors that have a blob, the highest one ``wins'' and the others ``lose''.
            // Ties are broken arbitrarily for now.
            
            Color maxcolor = NIL;
            int maxfreq = -1;
            for (auto i : selectedcolors) {
                if (ding.find(i.first) == ding.end()) continue;
                int truefreq = ding.at(i.first).size();
                if (truefreq > maxfreq) { 
                    maxfreq = truefreq;
                    maxcolor = i.first;
                }
            }
            
            bool hasmorecolors = false;
            for (auto i : ding) {
                if (i.first != maxcolor && i.first != NIL) hasmorecolors = true;
            }
            
            // cout << "AND THE WINNING COLOR IS " << maxcolor << endl;
            for (auto i : collected) {
                if (board[i.first][i.second]->color != maxcolor && board[i.first][i.second]->color != NIL) violations.insert(i);
                else if (hasmorecolors) violations.insert(i);
            }
        }
        
        // The other task of THE WOLF is to handle stars. Thankfully, these are easier.
        vis.clear();
        
        for (auto ii : stars) {
            if (vis.find(ii) != vis.end()) continue;
            ding.clear();
            selectedcolors.clear();
            collected.clear();
            queue<pair<int, int>> q;
            q.push(ii);
            while (q.size() > 0) {
                pair<int, int> now = q.front();
                q.pop();
                vis.insert(now);
                
                // cout << now.first << " / " << now.second << endl;
                
                Object* cur = board[now.first][now.second];
                
                if (ding.find(cur->color) == ding.end()) ding.insert({cur->color, vector<Object*>()});
                (*(ding.find(cur->color))).second.push_back(cur);
                
                if (instanceof<Star>(cur)) {
                    collected.insert(now);
                    if (selectedcolors.find(cur->color) == selectedcolors.end()) selectedcolors.insert({cur->color, 0});
                    (*(selectedcolors.find(cur->color))).second++;
                }
                
                for (int i = 0; i < 4; i++) {
                    pair<int, int> mid = {now.first + dx[i], now.second + dy[i]};
                    pair<int, int> next = {now.first + dx[i] * 2, now.second + dy[i] * 2};
                    if (!inside(mid) || !inside(next)) continue;
                    Object* between = board[mid.first][mid.second];
                    Object* hit = board[next.first][next.second];
                    if (between->isPathOccupied) continue;
                    if (vis.find(next) != vis.end()) continue;
                    vis.insert(next);
                    q.push(next);
                }
            }
            
            /*
            
            cout << "STAR FOR STAR " << ii.first << " " << ii.second << endl;
        
            for (auto i : vis) cout << i.first << " " << i.second << endl;
            
            cout << "COLORS!!!" << endl;
            for (auto i : ding) {
                cout << i.first << endl;
                for (auto x : i.second) cout << get_type(x) << endl;
            }
            
            cout << "STAR COLORS!!!" << endl;
            for (auto i : selectedcolors) {
                cout << i.first << " = " << i.second << endl;
            }
            
            */
            
            for (auto i : collected) {
                Color x = board[i.first][i.second]->color;
                if (ding.find(x) == ding.end()) {
                    violations.insert(i);
                    continue;
                }
                if (ding.at(x).size() != 2) violations.insert(i);
            }
        }
        
        // In German folklore, drudes are the malevolent nocturnal spirits associated with nightmares.
        // This section aka. THE DRUDE was an absolute nightmare to implement. I am, of course, referring to polynominos.
        // The algorithm is so weird that it has its own header file called blockgroup.h
        // Simply put, it is brute force. After all, this problem is NP-complete.
        // What happens here is simply a partition of the board and a check.
        
        vis.clear();
        
        set<pair<int, int>> region;
        
        for (auto ii : blocks) {
            if (vis.find(ii) != vis.end()) continue;
            region.clear();
            collected.clear();
            queue<pair<int, int>> q;
            q.push(ii);
            while (q.size() > 0) {
                pair<int, int> now = q.front();
                q.pop();
                vis.insert(now);
                region.insert(now);
                
                // cout << now.first << " / " << now.second << endl;
                
                Object* cur = board[now.first][now.second];
                
                if (instanceof<BlockGroup>(cur)) collected.insert(now);
                
                for (int i = 0; i < 4; i++) {
                    pair<int, int> mid = {now.first + dx[i], now.second + dy[i]};
                    pair<int, int> next = {now.first + dx[i] * 2, now.second + dy[i] * 2};
                    if (!inside(mid) || !inside(next)) continue;
                    Object* between = board[mid.first][mid.second];
                    Object* hit = board[next.first][next.second];
                    if (between->isPathOccupied) continue;
                    if (vis.find(next) != vis.end()) continue;
                    vis.insert(next);
                    q.push(next);
                }
            }
            
            /*
            cout << "TETRIS LOCATIONS" << endl;
            for (auto i : collected) cout << i.first << " " << i.second << endl;
            
            cout << "REGION" << endl;
            for (auto i : region) cout << i.first << " " << i.second << endl;
            */
            
            vector<pair<int, int>> regionvec;
            for (auto i : region) regionvec.push_back(make_pair((i.second)>>1, -1 * (i.first)>>1));
            
            BlockGroup testregion = BlockGroup(1, 0, regionvec);
            vector<BlockGroup> pieces;
            for (auto i : collected) {
                Object* o = board[i.first][i.second];
                if (!instanceof<BlockGroup>(o)) continue;
                BlockGroup* bg = dynamic_cast<BlockGroup*>(o);
                pieces.push_back(*bg);
            }
            if (testregion.solve(pieces));
            else {
                for (auto i : collected) violations.insert(i);
            }
        }
        
        // THE PHOENIX is an immortal creature, one that rises from the ashes once its life is over.
        // Similarly,we must start again from scratch when we encounter a cancellation symbol.
        // The way we do this is by removing it and another symbol then rechecking our current path against the new grid.
        // If there are no cancels the method simply returns what we have now.
        
        if (cancels.size() == 0 || ignored.size() == cancels.size()) {
            // cout << "NET VIOLATIONS - NON-CANCELLED ENDING\n"; 
            // for (auto i : violations) cout << i.first << " " << i.second << endl;
        
            return violations.size() == 0;
        }
        
        // cout << "NET VIOLATIONS - MOVING TO CANCELS..." << endl;
        // for (auto i : violations) cout << i.first << " " << i.second << endl;
        
        if (violations.size() == 0) return false; // There are cancels!!!
        
        vis.clear();
        for (auto ii : cancels) {
            if (ignored.find(ii) != ignored.end()) continue;
            collected.clear();
            queue<pair<int, int>> q;
            q.push(ii);
            while (q.size() > 0) {
                pair<int, int> now = q.front();
                q.pop();
                vis.insert(now);
                region.insert(now);
                
                // cout << now.first << " / " << now.second << endl;
                
                Object* cur = board[now.first][now.second];
                if (isSymbol(cur) && !instanceof<Cancel>(cur)) {
                    if (violations.find(now) != violations.end()) collected.insert(now);
                }
                for (int i = 0; i < 4; i++) {
                    pair<int, int> next = {now.first + dx[i], now.second + dy[i]};
                    if (!inside(next)) continue;
                    Object* hit = board[next.first][next.second];
                    if (hit->isPathOccupied) continue;
                    if (vis.find(next) != vis.end()) continue;
                    vis.insert(next);
                    q.push(next);
                }
            }
            
            ignored.insert(ii);
            (dynamic_cast<Cancel*>(board[ii.first][ii.second]))->ignored = true;
            
            // cout << "SYMBOL LOCATIONS FOR CANCEL " << ii.first << " " << ii.second << endl;
            // cout << "CANCEL STATUS " << cancels.size() << " " << ignored.size() << endl;
            bool retval = false;
            for (auto i : collected) {
                // cout << i.first << " " << i.second << endl;
                Object* o = board[i.first][i.second];
                board[i.first][i.second] = o->clear();
                if (instanceof<Dot>(o)) dots.erase(dots.find(i));
                else if (instanceof<Star>(o)) stars.erase(stars.find(i));
                else if (instanceof<Blob>(o)) blobs.erase(blobs.find(i));
                else if (instanceof<Triangle>(o)) triangles.erase(triangles.find(i));
                else if (instanceof<BlockGroup>(o)) blocks.erase(blocks.find(i));
                // disp();
                // cout << "VERIFYING MODIFIED..." << endl;
                if (ver(sx, sy)) {
                    retval = true;
                }
                // cout << "FINISHED VERIFYING MODIFIED\n";
                board[i.first][i.second] = o;
                if (instanceof<Dot>(o)) dots.insert(i);
                else if (instanceof<Star>(o)) stars.insert(i);
                else if (instanceof<Blob>(o)) blobs.insert(i);
                else if (instanceof<Triangle>(o)) triangles.insert(i);
                else if (instanceof<BlockGroup>(o)) blocks.insert(i);
                if (retval) break;
            }
            
            ignored.erase(ignored.find(ii));
            (dynamic_cast<Cancel*>(board[ii.first][ii.second]))->ignored = false;
            // cout << "FINISHED CANCELLING...\n";
            // disp();
            if (retval) return true;
        }
        
        
        return false;
    }
    
    
    
    bool Grid::check() {
        return ver(begin.first, begin.second);
    }
    
    // This function serves as a basic pruning system for the solver.
    // validateRegion only checks for trivial things: blobs, triangles, dots.
    // Cancels make this always return true.
    // Obviously this is not the complete algorithm but merely a small thing to prune.
    // If the path reaches a wall and it can go both ways, and if both regions fail this test...
    // Then we prune.
    
    // UPDATE - This test now tests blocks as well.
    
    bool Grid::validateRegion(int sx, int sy, vector<pair<int, int>> ban) {
        set<pair<int, int>> banned;
        for (auto i : ban) banned.insert(i);
        
        const int dx[4] = {01, 00, -1, 00};
        const int dy[4] = {00, 01, 00, -1};
        
        set<pair<int, int>> blobs;
        set<pair<int, int>> triangles;
        set<pair<int, int>> dots;
        set<pair<int, int>> cancels;
        set<pair<int, int>> blocks;
        
        set<pair<int, int>> vis;
        queue<pair<int, int>> q;
        q.push({sx, sy});
        
        while (q.size() > 0) {
            pair<int, int> now = q.front();
            q.pop();
            
            vis.insert(now);
            Object* o = board[now.first][now.second];
            
            if (instanceof<Blob>(o)) blobs.insert(now);
            if (instanceof<Triangle>(o)) triangles.insert(now);
            if (instanceof<Dot>(o)) dots.insert(now);
            if (instanceof<Cancel>(o)) return true;
            if (instanceof<BlockGroup>(o)) blocks.insert(now);
            
            for (int i = 0; i < 4; i++) {
                pair<int, int> next = {now.first + dx[i], now.second + dy[i]};
                if (!inside(next)) continue;
                Object* hit = board[next.first][next.second];
                if (hit->isPathOccupied) continue;
                if (banned.find(next) != banned.end()) continue;
                if (vis.find(next) != vis.end()) continue;
                vis.insert(next);
                q.push(next);
            }
        }
        
        set<Color> colors;
        for (auto i : blobs) colors.insert(board[i.first][i.second]->color);
        if (colors.size() > 1) return false;
        
        for (auto i : dots) if (!board[i.first][i.second]->isPathOccupied && banned.find(i) != banned.end()) return false;
        
        for (auto i : triangles) {
            Object* o = board[i.first][i.second];
            if (!instanceof<Triangle>(o)) continue;
            int target = (dynamic_cast<Triangle*>(o))->x;
            
            int cnt = 0;
            for (int d = 0; d < 4; d++) {
                pair<int, int> sus = {i.first + dx[d], i.second + dy[d]};
                if (!inside(sus)) continue;
                if (board[sus.first][sus.second]->isPathOccupied || banned.find(sus) != banned.end()) cnt++;
            }
            if (cnt != target) return false;
        }
        
        // Test blocks
        
        if (blocks.size() <= 0) return true;
        
        vector<pair<int, int>> effectiveRegion;
        for (auto i : vis) {
            if (i.first % 2 == 0 || i.second % 2 == 0) continue;
            effectiveRegion.push_back(make_pair(i.second / 2, -1 * i.first / 2));
        }
        
        vector<BlockGroup> boop;
        for (auto i : blocks) {
            Object* o = board[i.first][i.second];
            if (!instanceof<BlockGroup>(o)) continue;
            boop.push_back(*(dynamic_cast<BlockGroup*>(o)));
        }
        
        BlockGroup bg = BlockGroup(1, 0, effectiveRegion);
        bg.normalize();
        
        if (!bg.solve(boop)) {
            return false;
        }
        
        return true;
    }