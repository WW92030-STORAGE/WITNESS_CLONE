from object import Object, Color
from miscsymbols import Endpoint, Dot, Blob, Star, Triangle, Cancel
from blockgroup import BlockGroup
from grid import Grid
import util

class WitnessGame:
	grid = 0
	parent = dict()
	pos = (0, 0)
	origin = (0, 0)
	startingPoints = []

	dy = [1, 0, -1, 0]
	dx = [0, 1, 0, -1]
	dz = ['R', 'D', 'L', 'U']
	
	def __init__(self, g):
		self.grid = g
		self.parent = dict()

		self.startingPoints = []
		for p in g.starts:
			self.startingPoints.append(p)
		self.startingPoints.sort()
		self.origin = self.startingPoints[0]
		self.pos = self.origin
	
	def reset(self, g):
		self.grid = g
		self.parent = dict()

		self.startingPoints = []
		for p in g.starts:
			self.startingPoints.append(p)
		self.startingPoints.sort()
		self.origin = self.startingPoints[0]
		self.pos = self.origin
	
	def setStart(self, i):
		if len(self.startingPoints) <= 0:
			return 
		while i < 0:
			i = i + len(self.startingPoints)
		self.origin = self.startingPoints[i % len(self.startingPoints)]
	
	def processInput(self, c):
		dir = -1
		for i in range(4):
			if (c == self.dz[i]):
				dir = i
		
		if dir < 0:
			print("INVALID DIRECTION")
			return
		
		between = (self.pos[0] + self.dx[dir], self.pos[1] + self.dy[dir])
		hit = (self.pos[0] + 2 * self.dx[dir], self.pos[1] + 2 * self.dy[dir])
		if (not self.grid.inside(between)) or (not self.grid.inside(hit)):
			print("ERROR OUT OF BOUNDS")
			return
		
		if (not self.grid.get(between).isPath) or (not self.grid.get(hit).isPath):
			print("ERROR INVALID MOVE")
			return
		
		if self.pos in self.parent:
			if self.parent[self.pos] == hit:
				self.grid.board[self.pos[0]][self.pos[1]].isPathOccupied = False
				self.grid.board[between[0]][between[1]].isPathOccupied = False
				self.parent.pop(self.pos)
				self.pos = hit
				return
		
		print(hit, "/", self.pos)
		self.parent[hit] = self.pos
		self.grid.drawLine(self.pos, hit)
		self.pos = hit

	def reachedEnd(self):
		return self.pos in self.grid.ends

	def clear(self):
		for i in range(len(self.grid.board)):
			for j in range(len(self.grid.board[i])):
				self.grid.board[i][j].isPathOccupied = False
		
		self.grid.violations.clear()
		self.pos = self.origin

