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
    
    pair<int, int> begin;
    
    Grid(vector<vector<Object*>> v) { // Once a grid is created it cannot be changed (except to change path/occupied status).
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
                // if (instanceof<Cancel>(board[i][j])) cancels.insert({i, j});
            }
        }
    }
    
    void defaultGrid() {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                if (i % 2 == 0 || j % 2 == 0) (board[i][j])->isPath = true;
            }
        }
    }
    
    void drawLine(pair<int, int> a, pair<int, int> b) {
        if (a.first == b.first) {
            if (a.second > b.second) swap(a, b);
            for (int i = a.second; i <= b.second; i++) board[a.first][i]->isPathOccupied = true;
        }
        else if (a.second == b.second) {
            if (a.first > b.first) swap(a, b);
            for (int i = a.first; i <= b.first; i++) board[i][a.second]->isPathOccupied = true;
        }
    }
    
    virtual ~Grid() {
        
    }
    
    
    string to_string() {
        string s = "";
        for (auto i : board) {
            for (auto j : i) {
                char open = j->isPathOccupied ? '[' : '_';
                char close = j->isPathOccupied ? ']' : '_';
                s = s + open + get_type(j) + close + " ";
            }
            s = s + "\n";
        }
        return s;
    }
    
    void disp() {
        cout << to_string() << endl;
    }
    
    // The verification algorithm
    
    bool inside(pair<int, int> p) {
        if (p.first < 0 || p.second < 0) return false;
        if (p.first >= board.size() || p.second >= board[p.first].size()) return false;
        return true;
    }
    
    bool ver(int sx, int sy) {
        // The algorithm works in four stages: THE FOX / THE WOLF / THE DRUDE / THE SERGAL
        
        // The first section denoted THE FOX begins by handling the more trivial matters.
        // Just as foxes are easily recognizable, the tasks for this section are relatively self-explanatory and easy to check.
        // Namely, whether the path goes from a start to an end, whether all dots are covered, and whether triangles are solved.
        // Violations do not automatically return False. Instead, the locations of violated symbols are put into a set.
        // This is arguably the most important section because it establishes the trajectory of the path.
        
        const int dx[4] = {01, 00, -1, 00};
        const int dy[4] = {00, 01, 00, -1};
        if (!isStartingPoint(board[sx][sy])) return false;
        Object* o = board[sx][sy];
        if (!o->isPathOccupied) return false;
        
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
        // Namely, the blobs and the stars. Blobs cannot exist with any other color (except NIL)
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
                
                if (ding.find(cur->color) == ding.end()) ding.insert({cur->color, vector<Object*>()});
                (*(ding.find(cur->color))).second.push_back(cur);
                
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
            
            cout << "TETRIS LOCATIONS" << endl;
            for (auto i : collected) cout << i.first << " " << i.second << endl;
            
            cout << "REGION" << endl;
            for (auto i : region) cout << i.first << " " << i.second << endl;
            
            vector<pair<int, int>> regionvec;
            for (auto i : region) regionvec.push_back(make_pair((i.first)>>1, -1 * (i.second)>>1));
            
            BlockGroup testregion = BlockGroup(1, 0, regionvec);
            vector<BlockGroup> pieces;
            for (auto i : collected) {
                Object* o = board[i.first][i.second];
                if (!instanceof<BlockGroup>(o)) continue;
                BlockGroup* bg = dynamic_cast<BlockGroup*>(o);
                pieces.push_back(*bg);
            }
            if (testregion.solve(pieces)) cout << "VALID";
            else {
                for (auto i : collected) violations.insert(i);
            }
        }
        
        // END
        
        cout << "NET VIOLATIONS\n"; 
        for (auto i : violations) cout << i.first << " " << i.second << endl;
        
        return true;
    }
    
    
    
    bool check() {
        return ver(begin.first, begin.second);
    }
};