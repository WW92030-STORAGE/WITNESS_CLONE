#include <bits/stdc++.h> 
#define pii pair<int, int>

#include "object.h"
#include "miscsymbols.h"
#include "blockgroup.h"
#include "util.h"
#include "grid.h"
#include "solver.h"
#include "randgrid.h"

#include "witnessgame.h"

#include <chrono>

int main()
{
    Solver sx;
    RandGrid random;
    vector<vector<Object*>> v;
    
    random.pathfind();
    
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
    return 0;
}



