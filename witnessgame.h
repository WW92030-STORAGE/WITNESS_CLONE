#ifndef WITNESSGAME_H
#define WITNESSGAME_H

#include <vector>
#include <map>

#include "object.h"
#include "miscsymbols.h"
#include "blockgroup.h"
#include "util.h"
#include "grid.h"
#include "solver.h"
#include "randgrid.h"

class WitnessGame {
    public:
    Grid grid;
    std::vector<std::pair<int, int>> startpts;
    std::pair<int, int> origin;
    std::pair<int, int> pos;
    std::map<std::pair<int, int>, std::pair<int, int>> parent;
    
    const int dy[4] = {01, 00, -1, 00};
    const int dx[4] = {00, 01, 00, -1};
    const char dz[4] = {'R', 'D', 'L', 'U'};
    
    WitnessGame(Grid& g); // Init the grid
    void reset(Grid& g); // Change the grid and reset the game
    void setStart(int x); // Set the starting point to an available one
    
    void processInput(char c); // Char is converted to direction based on dz
    bool reachedEnd(); // Have we reached an end?
    void clear(); // Clear the drawn path from the grid
};

#endif