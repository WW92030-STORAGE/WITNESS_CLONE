#include <bits/stdc++.h> 
#include <ext/pb_ds/assoc_container.hpp>
using namespace std;
using namespace __gnu_pbds;
#define pii pair<int, int>
#define pb push_back

#include "object.h"
#include "blockgroup.h"

int main()
{
    // EXAMPLE I
    
    vector<pii> puz = vector<pii>({{0, 0}, {0, 1}, {0, 2}, {1, 2}});
    vector<pii> d1 = vector<pii>({{0, 0}, {0, 1}}); // vertical 1x2
    vector<pii> d2 = vector<pii>({{0, 0}, {1, 0}}); // horizontal 2x1
    
    BlockGroup puzzle = BlockGroup(1, 0, puz); // gamma shaped tetromino
    vector<BlockGroup> poly = vector<BlockGroup>({BlockGroup(1, 0, d1), BlockGroup(0, 0, d1)}); // 0 = non fixed rotation
    
    cout << puzzle.solve(poly) << endl;
    
    // EXAMPLE II
    
    vector<pii> bigregion = vector<pii>({{6, 0}, {6, 1}, {6, 2}, {7, 0}, {7, 1}, {7, 2}, {8, 2}, {8, 1}, {8, 0}});
    vector<pii> smallregion = vector<pii>({{10, 10}, {11, 10}, {11, 11}});
    vector<pii> single = vector<pii>({{1000, 1000}});
    
    BlockGroup big = BlockGroup(1, 0, bigregion); // 3x3 region with center removed
    BlockGroup small = BlockGroup(0, 0, smallregion); // L-shaped tri-nomino
    BlockGroup cell = BlockGroup(1, 0, single); // one cell
    vector<BlockGroup> test = vector<BlockGroup>({small, small, cell, cell, cell});
    
    cout << big.solve(test) << endl;
    
    // EXAMPLE III
    
    BlockGroup puzzle3 = BlockGroup(1, 0, puz); // use the tetromino from the first one
    poly = vector<BlockGroup>({BlockGroup(1, 0, d1), BlockGroup(1, 0, d1)});
    
    cout << puzzle3.solve(poly) << endl;
    
    // EXAMPLE IV - TETROMINOS
    
    vector<pii> L = vector<pii>({{0, 0}, {0, 1}, {0, 2}, {-1, 2}});
    vector<pii> J = vector<pii>({{0, 0}, {0, 1}, {0, 2}, {1, 2}});
    vector<pii> O = vector<pii>({{0, 0}, {0, 1}, {1, 0}, {1, 1}});
    vector<pii> I = vector<pii>({{0, 0}, {0, 1}, {0, 2}, {0, 3}});
    vector<pii> T = vector<pii>({{0, 0}, {0, 1}, {-1, 1}, {1, 1}});
    vector<pii> S = vector<pii>({{0, 0}, {0, 1}, {1, 1}, {1, 2}});
    vector<pii> Z = vector<pii>({{0, 0}, {0, 1}, {-1, 1}, {-1, 2}});
    
    BlockGroup L_ = BlockGroup(0, 0, L);
    BlockGroup J_ = BlockGroup(0, 0, J);
    BlockGroup O_ = BlockGroup(0, 0, O);
    BlockGroup I_ = BlockGroup(0, 0, I);
    BlockGroup T_ = BlockGroup(0, 0, T);
    BlockGroup S_ = BlockGroup(0, 0, S);
    BlockGroup Z_ = BlockGroup(0, 0, Z);
    
    vector<pii> grid; // 7x3 grid
    
    pii banned = {4, 1};
    
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 3; j++) {
            pii p = {i, j};
            if (p == banned) continue;
            grid.push_back(p);
        }
    }
    
    BlockGroup puzzle4 = BlockGroup(1, 0, grid);
    vector<BlockGroup> tetra({L_, O_, I_, T_, S_});
    cout << puzzle4.solve(tetra) << endl;
    
    // EXAMPLE 5 - UNIT CELLS
    
    vector<pii> grid2;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            pii p = {i, j};
            grid2.push_back(p);
        }
    }
    
    BlockGroup puzzle5 = BlockGroup(1, 0, grid2);
    
    vector<BlockGroup> units(64, cell);
    cout << puzzle5.solve(units) << endl;
    
    return 0;
}

/*

EXAMPLE 1 (Different characters = different constituent regions)

OOO OOO
OOO OOO
OOO OOO

XXX
XXX
XXX

XXX
XXX
XXX

EXAMPLE 2

OOO OOO XXX
OOO OOO XXX
OOO OOO XXX

OOO ... ###
OOO ... ###
OOO ... ###

/// ### ###
/// ### ###
/// ### ###

EXAMPLE 3 (RETURNS FALSE - X = UNCOVERED / ! = EXCESS)

### ###
### ###
### ###

OOO !!!
OOO !!!
OOO !!!

XXX
XXX
XXX

*/
