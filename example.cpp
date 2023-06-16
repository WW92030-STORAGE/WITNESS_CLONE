#include <bits/stdc++.h> 
#define pii pair<int, int>

#include "object.h"
#include "miscsymbols.h"
#include "blockgroup.h"
#include "util.h"
#include "grid.h"
#include "solver.h"

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
    
    Grid grid = Grid(v);
    grid.defaultGrid();
    
    Solver sx = Solver(grid);
    
    sx.solve();
    sx.disp();
    
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
    
    sx.set(grid2);
    sx.solve();
    sx.disp();
    
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
    
    sx.set(grid3);
    sx.solve();
    sx.disp();
    
    return 0;
}



