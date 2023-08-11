from object import Object, Color
from miscsymbols import Endpoint, Dot, Blob, Star, Triangle, Cancel
from blockgroup import BlockGroup
from grid import Grid
import util

import math
import sys
import random

class Solver:
    callstopath = 0
    dx = [1, 0, -1, 0]
    dy = [0, 1, 0, -1]
    solution = []
    vis = dict()

    origin = (-1, -1)
    grid = 0

    def __init__(self, g):
        self.grid = g
    
    def set(self, g):
        self.grid = g
    
    def path(self, src, prev):
        self.callstopath = self.callstopath + 1
        if (self.callstopath % 4096 == 0):
            print(str(self.callstopath), "CALLS TO PATH")
        if len(self.solution) > 0:
            return
        
        if src in self.grid.ends:
            self.grid.board[src[0]][src[1]].isPathOccupied = True
            check = self.grid.ver(self.origin)
            if (check):
                self.vis[src] = prev
                thing = src
                while thing in self.vis and self.vis[thing] != thing:
                    self.solution.append(thing)
                    thing = self.vis[thing]
                self.solution.append(thing)
                self.solution.reverse()
            self.grid.board[src[0]][src[1]].isPathOccupied = False
            return
        
        '''
        for ii in range(4):
            x0 = (src[0] + self.dx[(ii + 0) % 4], src[1] + self.dy[(ii + 0) % 4])
            x1 = (src[0] + self.dx[(ii + 1) % 4], src[1] + self.dy[(ii + 1) % 4])
            x2 = (src[0] + self.dx[(ii + 2) % 4], src[1] + self.dy[(ii + 2) % 4])
            x3 = (src[0] + self.dx[(ii + 3) % 4], src[1] + self.dy[(ii + 3) % 4])

            blocked0 = not self.grid.inside(x0)
            blocked1 = (not self.grid.inside(x1))
            blocked1 = blocked1 or (self.grid.board[x1[0]][x1[1]].isPathOccupied)
            blocked2 = not self.grid.inside(x2)
            blocked3 = (not self.grid.inside(x3))
            blocked3 = blocked3 or (self.grid.board[x3[0]][x3[1]].isPathOccupied)

            banned = [src, x0]
            if (blocked0 and (not blocked1) and (not blocked3)):
                r1 = self.grid.validateRegion(x1, banned)
                r3 = self.grid.validateRegion(x3, banned)
                if (not r1) and (not r3):
                    return
                break
        '''

        self.vis[src] = prev
        self.grid.board[src[0]][src[1]].isPathOccupied = True

        offset = random.randint(0, 3)
        for ii in range(4):
            i = (ii + offset) % 4
            next = (src[0] + self.dx[i], src[1] + self.dy[i])
            if not self.grid.inside(next):
                continue
            if not self.grid.board[next[0]][next[1]].isPath:
                continue
            if next in self.vis:
                continue

            self.path(next, src)
        
        self.vis.pop(src)
        self.grid.board[src[0]][src[1]].isPathOccupied = False
    
    def solve(self):
        self.callstopath = 0
        self.solution.clear()
        for i in self.grid.starts:
            self.origin = i
            self.vis.clear()
            self.vis[i] = i
            self.path(i, i)
            if len(self.solution) > 0:
                break
        
        return self.solution
