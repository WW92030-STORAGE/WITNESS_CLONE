from object import Object, Color
from miscsymbols import Endpoint, Dot, Blob, Star, Triangle, Cancel
from blockgroup import BlockGroup
from grid import Grid
import util

import random
import sys
import math
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
            for j in range(self.size[1]):
                row.append(Object())
            res.append(row)
        
        for i in range(self.size[0]):
            for j in range(self.size[1]):
                if i % 2 == 0 or j % 2 == 0:
                    res[i][j].isPath = True
        
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

    def randMaze(self, numCuts):
        if (self.singlepath):
            self.pathfind()
        
        path = self.possiblePaths[random.randint(0, len(self.possiblePaths) - 1)]
        
        things = set()
        
        count = 0
        while len(things) < numCuts and count < NUM_ATTEMPTS * NUM_ATTEMPTS:
            pos = (random.randint(0, self.size[0] - 1), random.randint(0, self.size[1] - 1))
            if pos in path:
                continue
            if (pos[0] % 2 == 1) != (pos[1] % 2 == 1):
                things.add(pos)
            count = count + 1
        
        grid = Grid(self.objGrid())
        for pos in things:
            grid.board[pos[0]][pos[1]].isPath = False
        
        return grid
    
    def blobRegionScale(self,x):
        return x * math.log(x) / math.log(2)
    
    def blobPathScale(self, x):
        return 2 * (12 + random.randint(0, 7))
    
    def minRegionSize(self, x):
        return 16 / x

    def randBlobs(self, numBlobs, numCols, numCuts):
        path = self.possiblePaths[random.randint(0, len(self.possiblePaths) - 1)]
        self.getRegions(path)
        attempts = 0
        while attempts < NUM_ATTEMPTS:
            attempts = attempts + 1
            if self.singlepath:
                self.pathfind()
            path = self.possiblePaths[random.randint(0, len(self.possiblePaths) - 1)]
            self.getRegions(path)

            if len(self.gridRegions) < self.blobRegionScale(numCols):
                continue
            
            minsize = sys.maxsize
            for i in self.gridRegions:
                minsize = min(minsize, len(i))
            
            if minsize < self.minRegionSize(len(self.gridRegions)):
                continue
            break
        
        v = self.objGrid()

        things = set()
        count = 0
        while len(things) < numCuts and count < NUM_ATTEMPTS * NUM_ATTEMPTS:
            pos = (random.randint(0, self.size[0] - 1), random.randint(0, self.size[1] - 1))
            if pos in path:
                continue
            if (pos[0] % 2) != (pos[1] % 2):
                things.add(pos)
            count = count + 1
        
        for i in things:
            v[i[0]][i[1]].isPath = False
        count = 0
        things.clear()
        random.shuffle(self.COLORS)
        perm = []
        for i in range(len(self.gridRegions)):
            perm.append(i)
        random.shuffle(perm)

        index = 0
        while len(things) < numBlobs and count < NUM_ATTEMPTS * NUM_ATTEMPTS:
            subcount = 0
            while subcount < NUM_ATTEMPTS:
                x = 1 + 2 * random.randint(0, self.size[0] // 2 - 1)
                y = 1 + 2 * random.randint(0, self.size[1] // 2 - 1)
                if not (x, y) in self.gridRegions[index]:
                    things.add((x, y))
                    break
                subcount = subcount + 1
            
            index = (index + 1) % len(self.gridRegions)
            count = count + 1
        
        count = 0
        while len(things) < numBlobs and count < NUM_ATTEMPTS * NUM_ATTEMPTS:
            x = 1 + 2 * random.randint(0, self.size[0] // 2 - 1)
            y = 1 + 2 * random.randint(0, self.size[1] // 2 - 1)
            things.add((x, y))
            count = count + 1
        
        for i in things:
            reg = -1
            for index in range(len(self.gridRegions)):
                if reg >= 0:
                    break
                if i in self.gridRegions[index]:
                    reg = index
            color = Color.GREY
            index = perm[reg] % numCols
            if (index < len(self.COLORS)):
                color = self.COLORS[index]
            v[i[0]][i[1]] = Blob(color)
        
        return Grid(v)
        




    
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

