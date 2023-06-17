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
    vector<vector<Object*>> v;
    Solver sx = Solver(Grid());
    
    // EXAMPLE I
    // https://windmill.thefifthmatt.com/build/CAUSAggDEgIoBRIJCAkiBQgBEgEBEgASBAgIEAESAigHEgQICzABEgASBAgIEAESAigFEgIIBA==_0
    
    v = vector<vector<Object*>>(5, vector<Object*>(5));
    
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
    
    sx.set(grid);
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
    
    // EXAMPLE IV
    // https://windmill.thefifthmatt.com/cgchwd8
    
    v = vector<vector<Object*>>(9, vector<Object*>(9));
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) v[i][j] = new Object();
    }
    
    v[8][0] = new Endpoint(true);
    v[0][8] = new Endpoint(false);
    v[1][3] = new Triangle(2);
    v[1][5] = new Triangle(2);
    v[3][3] = new Triangle(1);
    v[3][5] = new Triangle(1);
    v[5][7] = new Triangle(1);
    v[7][1] = new Triangle(1);
    v[7][3] = new Triangle(2);
    v[7][7] = new Triangle(2);
    
    Grid grid4 = Grid(v);
    grid4.defaultGrid();
    
    sx.set(grid4);
    sx.solve();
    sx.disp();
    cout << "END OF EXAMPLE IV\n";
    
    /*
    
    // EXAMPLE V
    // https://windmill.thefifthmatt.com/build/CAkSAigHEgIIBRIICAQaBAgCEAASAigCEgIIBRIAEgIIBRICKAUSAggFEgIoAxICCAUSAigFEgIIBRICKAMSAggFEgASAggFEgASAggFEgIoBRICCAUSAigFEgIIBRICKAcSAggFEgIoAxICCAUSABICCAUSABICCAUSAigGEgIIAxICKAg=_0
    
    v = vector<vector<Object*>>(9, vector<Object*>(9));
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            v[i][j] = new Object();
            if (i % 2 == 0 || j % 2 == 0) v[i][j]->isPath = true;
        }
    }
    
    v[8][0] = new Endpoint(true);
    v[0][8] = new Endpoint(false);
    
    v[0][7]->isPath = false;
    v[1][2]->isPath = false;
    v[1][4]->isPath = false;
    v[2][1]->isPath = false;
    v[2][5]->isPath = false;
    v[3][2]->isPath = false;
    v[3][6]->isPath = false;
    v[3][8]->isPath = false;
    v[4][1]->isPath = false;
    v[4][7]->isPath = false;
    v[5][4]->isPath = false;
    v[6][3]->isPath = false;
    v[6][7]->isPath = false;
    v[7][2]->isPath = false;
    v[7][0]->isPath = false;
    
    Grid grid5 = Grid(v);
    
    sx.set(grid5);
    sx.solve();
    sx.disp();
    
    */
    
    // EXAMPLE VI
    // https://windmill.thefifthmatt.com/build/CAkSAigIEggIBBoECAIQABICKBUSDAgJIggIAhIEAQEBABICKAMSDAgJIggIAhIEAQEAARICKAYSAggFEgIoDxICCAUSAigOEgIIAxICKAg=_0
    
    v = vector<vector<Object*>>(9, vector<Object*>(9));
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            v[i][j] = new Object();
            if (i % 2 == 0 || j % 2 == 0) v[i][j]->isPath = true;
        }
    }
    
    v[8][0] = new Endpoint(true);
    v[0][8] = new Endpoint(false);
    
    v[6][3]->isPath = false;
    v[4][5]->isPath = false;
    v[3][3] = new BlockGroup(1, 0, vector<pii>({{0, 0}, {0, 1}, {1, 1}}));
    v[3][7] = new BlockGroup(1, 0, vector<pii>({{0, 0}, {0, 1}, {-1, 1}}));
    
    Grid grid6 = Grid(v);
    
    sx.set(grid6);
    sx.solve();
    sx.disp();
    
    return 0;
}



