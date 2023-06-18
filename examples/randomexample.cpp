#include <bits/stdc++.h> 
#define pii pair<int, int>

#include "object.h"
#include "miscsymbols.h"
#include "blockgroup.h"
#include "util.h"
#include "grid.h"
#include "solver.h"
#include "randgrid.h"

int main()
{
    RandGrid random = RandGrid();
    random.disp();
    
    random.visualize(random.randint(random.possiblePaths.size()));
    
    Grid grid = random.randTriangles(8, 2);
    
    grid.disp();
    cout << endl;
    
    Solver solver = Solver(grid);
    solver.solve();
    
    solver.disp();
    
    return 0;
}



