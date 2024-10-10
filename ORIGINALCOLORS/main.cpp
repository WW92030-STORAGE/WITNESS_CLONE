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
    //vector<vector<Entity*>> v;
    
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
    
    for (int i = 0; i < 16; i++) {
        auto start = std::chrono::high_resolution_clock::now();
        // sx.set(random.randBlocks(4, 0));
        Grid grid = random.randBlobs(9, 3, 0);
        sx.set(grid);
        // sx.set(random.randTriangles(6 + random.randint(4), 0));
        sx.solve();
        if (sx.solution.size() == 0) cout << "ERROR!!!!!!!!!!!\n";
        else sx.disp();
        
        auto finish = std::chrono::high_resolution_clock::now();
        auto lol = std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count();
        std::cout << lol / 1000000.0 << "ms\n";
    }
    std::cout << "!!!!\n";

    */

    vector<vector<std::shared_ptr<Entity>>> v;

    int errors = 0;
    int denom = 0;

    for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
    for (int k = 0; k < 4; k++) {
    for (int l = 0; l < 4; l++) {

        denom++;

        auto start = std::chrono::high_resolution_clock::now();
        v = vector<vector<std::shared_ptr<Entity>>>(9, vector<std::shared_ptr<Entity>>(9));
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                v[i][j] = std::shared_ptr<Entity>(new Entity());
                if (i % 2 == 0 || j % 2 == 0) v[i][j]->isPath = true;
            }
        }
    
        v[8][0] = std::shared_ptr<Endpoint>(new Endpoint(true));
        v[0][8] = std::shared_ptr<Endpoint>(new Endpoint(false));

        /*

        v[(i % 2) * 2 + 1][(i / 2) * 2 + 1] = std::shared_ptr<Triangle>(new Triangle(2));
        v[(j % 2) * 2 + 1 + 4][(j / 2) * 2 + 1] = std::shared_ptr<Triangle>(new Triangle(2));
        v[(k % 2) * 2 + 1][(k / 2) * 2 + 1 + 4] = std::shared_ptr<Triangle>(new Triangle(2));
        v[(l % 2) * 2 + 1 + 4][(l / 2) * 2 + 1 + 4] = std::shared_ptr<Triangle>(new Triangle(2));

        */

        v[(i) * 2 + 1][1] = std::shared_ptr<Triangle>(new Triangle(1));
        v[(j) * 2 + 1][3] = std::shared_ptr<Triangle>(new Triangle(1));
        v[(k) * 2 + 1][5] = std::shared_ptr<Triangle>(new Triangle(1));
        v[(l) * 2 + 1][7] = std::shared_ptr<Triangle>(new Triangle(3));

        Grid grid(v);
        sx.set(grid);
        sx.solve();
        if (sx.solution.size() == 0) {
            cout << "ERROR!!!!!!!!!!!\n";
            errors++;
        }
        // else sx.disp();
        else cout << "SOLUTION FOUND...\n";
        continue;

        auto finish = std::chrono::high_resolution_clock::now();
        auto lol = std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count();
        std::cout << lol / 1000000.0 << "ms\n";
    }
    }
    }
    }

    cout << errors << "/" << denom << " INVALID PUZZLES\n";

    return 0;
}



