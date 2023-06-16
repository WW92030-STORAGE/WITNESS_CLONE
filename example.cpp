#include <bits/stdc++.h> 
using namespace std;
#define pii pair<int, int>

#include "object.h"

#include "miscsymbols.h"
#include "blockgroup.h"

#include "util.h"

#include "grid.h"

int main()
{
    // EXAMPLE I
    // https://windmill.thefifthmatt.com/build/CAUSAggDEgIoBRIJCAkiBQgBEgEBEgASBAgIEAESAigHEgQICzABEgASBAgIEAESAigFEgIIBA==_0
    
    vector<vector<Object*>> v(5, vector<Object*>(5));
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) v[i][j] = new Object();
    }
    v[0][0] = new Endpoint(true);
    v[4][4] = new Endpoint(false);
    v[1][1] = new BlockGroup(1, 0, vector<pii>(1, {0, 0}));
    v[3][3] = new Star(BLACK);
    v[2][0] = new Dot();
    v[3][1] = new Triangle(1);
    v[1][3] = new Star(BLACK);
    
    
    v[0][0]->isPathOccupied = true;
    v[1][0]->isPathOccupied = true;
    v[2][0]->isPathOccupied = true;
    v[2][1]->isPathOccupied = true;
    v[2][2]->isPathOccupied = true;
    v[1][2]->isPathOccupied = true;
    v[0][2]->isPathOccupied = true;
    v[0][3]->isPathOccupied = true;
    v[0][4]->isPathOccupied = true;
    v[1][4]->isPathOccupied = true;
    v[2][4]->isPathOccupied = true;
    v[3][4]->isPathOccupied = true;
    v[4][4]->isPathOccupied = true;
    
    Grid grid = Grid(v);
    grid.defaultGrid();
    
    grid.disp();
    
    cout << grid.ver(0, 0) << endl;
    
    // EXAMPLE II
    // https://windmill.thefifthmatt.com/build/CAcSAigGEgIIBBIAEgQIBxAFEgASCggJIgYIARICAQESABIECAcQARICKAkSBAgHEAESABIJCAkiBQgBEgEBEgASBAgHEAESAigJEgQIBxACEgASBAgHEAESABICCAoSABICCAMSAigG_0
    
    
    v = vector<vector<Object*>>(7, vector<Object*>(7));
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 7; j++) v[i][j] = new Object();
    }
    v[6][0] = new Endpoint(true);
    v[0][6] = new Endpoint(false);
    v[1][3] = new BlockGroup(1, 0, vector<pii>({{0, 0}, {0, 1}}));
    v[3][3] = new BlockGroup(1, 0, vector<pii>({{0, 0}}));
    v[5][5] = new Cancel();
    v[1][5] = new Blob(BLACK);
    v[3][5] = new Blob(BLACK);
    v[3][1] = new Blob(BLACK);
    v[5][3] = new Blob(BLACK);
    v[5][1] = new Blob(WHITE);
    v[1][1] = new Blob(YELLOW);
    
    Grid grid2 = Grid(v);
    grid2.defaultGrid();
    
    grid2.drawLine({6, 0}, {2, 0});
    grid2.drawLine({2, 0}, {2, 2});
    grid2.drawLine({2, 2}, {4, 2});
    grid2.drawLine({4, 2}, {4, 4});
    grid2.drawLine({4, 4}, {0, 4});
    grid2.drawLine({0, 4}, {0, 6});
    
    grid2.disp();
    
    cout << grid2.ver(6, 0) << endl;
    
    // EXAMPLE III
    // https://windmill.thefifthmatt.com/build/CAkSAigIEggIBBoECAIQABICKAMSAggKEgASBAgLMAESAigfEgwICSIICAISBAEBAQESABICCAoSAigHEgIIBhICKAcSAggKEgASCwgJIgcIAxIDAQEBEgIoBRICCAMSAigI_0
    
    v = vector<vector<Object*>>(9, vector<Object*>(9));
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) v[i][j] = new Object();
    }
    
    v[8][0] = new Endpoint(true);
    v[0][8] = new Endpoint(false);
    v[5][1] = new BlockGroup(1, 0, vector<pii>({{0, 0}, {0, 1}, {1, 0}, {1, 1}}));
    v[7][3] = new BlockGroup(1, 0, vector<pii>({{0, 0}, {0, 1}, {0, 2}}));
    v[6][2] = new Dot();
    v[5][3] = new Cancel();
    v[7][1] = new Cancel();
    
    v[1][3] = new Cancel();
    v[1][5] = new Triangle(1);
    
    Grid grid3 = Grid(v);
    grid3.defaultGrid();
    
    grid3.drawLine({8, 0}, {4, 0});
    grid3.drawLine({4, 0}, {4, 4});
    grid3.drawLine({4, 4}, {8, 4});
    grid3.drawLine({8, 4}, {8, 8});
    grid3.drawLine({8, 8}, {0, 8});
    
    grid3.disp();
    cout << grid3.ver(8, 0) << endl;
    return 0;
}


/*

The board is represented as a (2n+1)x(2n+1) grid of objects. One is the entrance and one is the exit.
Objects are the base class for everything in the puzzle. Can be empty, path, start, end, dot, or symbol.
Even indexes denote lines that the path can travel on. Cuts obstruct the path.
Entirely odd indices denote spaces where symbols reside.

*/
