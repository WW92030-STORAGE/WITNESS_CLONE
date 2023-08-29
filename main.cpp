#include <vector>
#include <iostream>
#include <chrono>

#define pii pair<int, int>

#include "object.h"
#include "miscsymbols.h"
#include "blockgroup.h"
#include "util.h"
#include "grid.h"
#include "solver.h"
#include "randgrid.h"

#include "witnessgame.h"

int main()
{
    Solver sx;
    RandGrid random;
    vector<vector<Object*>> v;
    
    random.pathfind();
    
    /*
    
    WitnessGame game = WitnessGame(random.randMaze());
    std::cout << game.origin.first << " " << game.origin.second << "\n";
    
    while (true) {
        game.grid.disp();
        char c;
        std::cin >> c;
        if (c == 'X') game.clear();
        else game.processInput(c);
        
        if (game.reachedEnd()) {
            if (game.grid.ver(game.origin.first, game.origin.second)) {
                cout << "YOU WIN!!!!\n";
                game.grid.disp();
                return 0;
            }
            else {
                cout << "YOU LOSE!!!!\n";
                game.clear();
                game.grid.disp();
            }
        }
    }
    
    */
    
    /*
    
    // EXAMPLE VIII
    // https://windmill.thefifthmatt.com/cmkhg78
    
    v = vector<vector<Object*>>(11, vector<Object*>(11));
    for (int i = 0; i < 11; i++) {
        for (int j = 0; j < 11; j++) v[i][j] = new Object();
    }
    
    v[10][0] = new Endpoint(true);
    v[0][10] = new Endpoint(false);
    
    v[1][1] = new Triangle(1);
    v[3][1] = new Triangle(1);
    v[5][1] = new Triangle(2);
    v[7][1] = new Triangle(2);
    v[9][1] = new Triangle(2);
    v[1][3] = new Triangle(3);
    v[3][3] = new Triangle(2);
    v[5][3] = new Triangle(2);
    v[7][3] = new Triangle(2);
    v[9][3] = new Triangle(3);
    v[1][5] = new Triangle(1);
    v[3][5] = new Triangle(1);
    v[5][5] = new Triangle(2);
    v[7][5] = new Triangle(2);
    v[9][5] = new Triangle(2);
    v[1][7] = new Triangle(1);
    v[3][7] = new Triangle(1);
    v[5][7] = new Triangle(3);
    v[7][7] = new Triangle(2);
    v[9][7] = new Triangle(2);
    v[1][9] = new Triangle(3);
    v[3][9] = new Triangle(2);
    v[5][9] = new Triangle(2);
    v[7][9] = new Triangle(2);
    v[9][9] = new Triangle(3);
    
    cout << "VECTOR SET\n";
    
    Grid grid8 = Grid(v);
    grid8.defaultGrid();
    
    cout << "GRID CREATED\n";
    
    sx.set(grid8);
    sx.solve();
    sx.disp();
    
    // EXAMPLE VI
    // https://windmill.thefifthmatt.com/build/CAkSAigIEggIBBoECAIQABICKBUSDAgJIggIAhIEAQEBABICKAMSDAgJIggIAhIEAQEAARICKAYSAggFEgIoDxICCAUSAigOEgIIAxICKAg=_0
    
    */
    
    /*
    
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
    
    */
    
    for (int i = 0; i < 16; i++) {
        auto start = std::chrono::high_resolution_clock::now();
        // sx.set(random.randBlocks(4, 0));
        Grid grid = random.randBlobs(9, 3, 0);
        sx.set(grid);
        // sx.set(random.randTriangles(6 + random.randint(4), 0));
        continue;
        sx.solve();
        if (sx.solution.size() == 0) cout << "ERROR!!!!!!!!!!!\n";
        else sx.disp();
        
        auto finish = std::chrono::high_resolution_clock::now();
        auto lol = std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count();
        std::cout << lol / 1000000.0 << "ms\n";
    }
    std::cout << "!!!!\n";

    // TODO - sx attempts to delete the grid stored internally (which deletes the objects inside problem is they have already been deleted).
    // For now these 2 lines will mitigate this issue (since now there is a grid to destroy).

    Grid grid = Grid();
    sx.set(grid);

    return 0;
}



