from object import Object, Color
from miscsymbols import Endpoint, Dot, Blob, Star, Triangle, Cancel
from blockgroup import BlockGroup
from grid import Grid
import util

import random
import sys
from queue import Queue

BLOCKS_REGIONS = 3
STARS_REGIONS = 3
NUM_ATTEMPTS = 256 # How many attempts RNG-based things attempt to run

class RandGrid: # Right now always generates 4x4 grids.
    dx = [1, 0, -1, 0]
    dy = [0, 1, 0, -1]
    possiblePaths = [] # Vector of sets representing paths
    gridRegions = [] # Vector of sets representing regions

    start = (8, 0)
    end = (0, 8)
    size = (9, 9)

    singlepath = False

    COLORS = [Color.RED, Color.GREEN, Color.BLUE, Color.CYAN, Color.MAGENTA, Color.YELLOW, Color.WHITE]

    def objGrid(self):
        res = []
        for i in range(self.size[0]):
            row = []
            for i in range(self.size[1]):
                row.append(Object())
            res.append(row)
        
        res[self.start[0]][self.start[1]] = Endpoint(True)
        res[self.end[0]][self.end[1]] = Endpoint(False)
        return res

    def __init__(self, a = (8, 0), b = (0, 8), sz = (9, 9)):
        self.start = a
        self.end = b
        self.size = sz

    def resetColors(self):
        self.COLORS = [Color.RED, Color.GREEN, Color.BLUE, Color.CYAN, Color.MAGENTA, Color.YELLOW, Color.WHITE]
    
    def reset(self, a = (8, 0), b = (0, 8)):
        self.start = a
        self.end = b
        self.pathfind()
    
    def randStars(self):
        random.shuffle(self.COLORS)

        board = self.objGrid()

        pos = set()
        while len(pos) < 8:
            xpos = random.randint(0, self.size[0] // 2 - 1) * 2 + 1
            ypos = random.randint(0, self.size[1] // 2 - 1) * 2 + 1
            pos.add((xpos, ypos))
        
        index = 0
        for p in pos:
            board[p[0]][p[1]] = Star(self.COLORS[index % 2])
            index = index + 1

        grid = Grid(board)
        grid.defaultGrid()
        return grid
    
    # Region Finding

    def getRegions(self, path):
        self.gridRegions.clear()
        vis = set()

        for i in range(self.size[0]):
            for j in range(self.size[1]):
                src = (i, j)
                if src in vis or src in path:
                    continue
                area = set()
                q = Queue()
                q.put(src)
                while q.qsize() > 0:
                    now = q.get()
                    for d in range(4):
                        next = (now[0] + self.dx[d], now[1] + self.dy[d])
                        if not self.inside(next):
                            continue
                        if next in path or next in vis:
                            continue

                        area.add(next)
                        vis.add(next)
                        q.put(next)
                self.gridRegions.append(area)

    # Pathfinding

    parent = dict()

    def inside(self, p):
        if (p[0] < 0 or p[1] < 0):
            return False
        if (p[0] >= self.size[0]):
            return False
        if (p[1] >= self.size[1]):
            return False
        return True
    
    def dfs(self, src, prev, numPaths):
        if self.singlepath and len(self.possiblePaths) > numPaths:
            return

        if (src == self.end):
            self.parent[src] = prev
            res = set()
            thing = (src[0], src[1])
            if not thing in self.parent:
                return

            while thing in self.parent and self.parent[thing] != thing:
                res.add((thing[0], thing[1]))
                thing = self.parent[thing]
            res.add((thing[0], thing[1]))
            self.possiblePaths.append(res)
            self.parent.pop(src)
            return

        self.parent[src] = prev

        offset = random.randint(0, 3)
        for ii in range(4):
            i = (ii + offset) % 4
            next = (src[0] + self.dx[i], src[1] + self.dy[i])
            if not self.inside(next):
                continue
            if next[0] % 2 == 1 and next[1] % 2 == 1:
                continue
            if next in self.parent:
                continue
            self.dfs(next, src, numPaths)
        
        if src in self.parent:
            self.parent.pop(src)

    def pathfind(self):
        self.possiblePaths.clear()
        self.parent.clear()
        self.parent[self.start] = self.start
        self.dfs(self.start, self.start, 2)
        random.shuffle(self.possiblePaths)
    
    def __str__(self):
        return str(len(self.possiblePaths)) + " / " + str(len(self.gridRegions))

    def visualize(self, x = 0):
        if x < 0 or x >= len(self.possiblePaths):
            return
        
        path = self.possiblePaths[x]
        self.getRegions(path)

        res = ""
        
        for i in range(self.size[0]):
            for j in range(self.size[1]):
                src = (i, j)
                if src in path:
                    res = res + "#"
                else:
                    bad = True
                    for reg in range(len(self.gridRegions)):
                        region = self.gridRegions[reg]
                        if src in region:
                            bad = False
                            res = res + chr(65 + reg)
                            break
                    
                    if bad:
                        res = res + "."
            res = res + "\n"
        
        print(res)

