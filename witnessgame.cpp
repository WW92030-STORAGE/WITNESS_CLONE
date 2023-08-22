#include <vector>
#include <map>

#include "object.h"
#include "miscsymbols.h"
#include "blockgroup.h"
#include "util.h"
#include "grid.h"
#include "solver.h"
#include "randgrid.h"

#include "witnessgame.h"

WitnessGame::WitnessGame(Grid g) {
    grid = g;
    startpts.clear();
    for (auto i : grid.starts) startpts.push_back(i);
    std::sort(startpts.begin(), startpts.end());
    origin = startpts[0];
    pos = origin;
}

void WitnessGame::reset(Grid g) {
    grid = g;
    startpts.clear();
    for (auto i : grid.starts) startpts.push_back(i);
    std::sort(startpts.begin(), startpts.end());
    origin = startpts[0];
    pos = origin;
}

void WitnessGame::setStart(int x) {
    if (startpts.size() <= 0) return;
    while (x < 0) x += startpts.size();
    origin = startpts[x % startpts.size()];
}

void WitnessGame::processInput(char c) { // Char to dz direction
    int dir = -1;
    for (int i = 0; i < 4; i++) {
        if (c == dz[i]) dir = i;
    }
    if (dir < 0) {
        std::cout << "INVALID DIRECTION\n";
        return;
    }
    std::pair<int, int> between = {pos.first + dx[dir], pos.second + dy[dir]};
    std::pair<int, int> hit = {pos.first + 2 * dx[dir], pos.second + 2 * dy[dir]};
    if (!grid.inside(between) || !grid.inside(hit)) {
        std::cout << "ERROR OUT OF BOUNDS\n";
        return;
    }
    if (!grid.board[between.first][between.second]->isPath || !grid.board[hit.first][hit.second]->isPath) {
        std::cout << "ERROR NOT VALID MOVE\n";
        return;
    }
    
    if (parent.find(pos) != parent.end() && parent.at(pos) == hit) {
        grid.board[pos.first][pos.second]->isPathOccupied = false;
        grid.board[between.first][between.second]->isPathOccupied = false;
        parent.erase(parent.find(pos));
        pos = hit;
        return;
    }
    std::cout << "[" << hit.first << " " << hit.second << "/" << pos.first << " " << pos.second << "]\n";
    parent.insert({hit, pos});
    grid.drawLine(pos, hit);
    pos = hit;
}

bool WitnessGame::reachedEnd() {
    return grid.ends.find(pos) != grid.ends.end();
}

void WitnessGame::clear() {
    startpts.clear();
    for (auto i : grid.starts) startpts.push_back(i);
    std::sort(startpts.begin(), startpts.end());
    origin = startpts[0];
    pos = origin;
    for (int i = 0; i < grid.board.size(); i++) {
        for (int j = 0; j < grid.board[i].size(); j++) grid.board[i][j]->isPathOccupied = false;
    }
}

