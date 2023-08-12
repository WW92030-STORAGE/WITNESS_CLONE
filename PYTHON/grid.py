from object import Object, Color
from miscsymbols import Endpoint, Dot, Blob, Star, Triangle, Cancel
from blockgroup import BlockGroup
import util

from queue import Queue

class Grid:
	n = 0
	m = 0
	board = []
	starts = {}
	ends = {}
	dots = {}
	triangles = {}
	blobs = {}
	stars = {}
	blocks = {}
	cancels = {}
	ignored = {}
	
	violations = {}
	
	begin = (-1, -1)
	
	def __init__(self, matrix):
		self.n = len(matrix)
		self.m = len(matrix[0])
		
		if self.n % 2 == 0:
			self.n = self.n + 1
		if self.m % 2 == 0:
			self.m = self.m + 1
		
		self.board = []
		
		for i in range(self.n):
			self.board.append([])
			for j in range(self.m):
				self.board[i].append(Object())
		
		self.starts = set()
		self.ends = set()
		
		self.blobs = set()
		self.stars = set()
		self.dots = set()
		self.blocks = set()
		self.ignored = set()
		self.cancels = set()
		self.triangles = set()
		
		for i in range(self.n):
			for j in range(self.m):
				if j < len(matrix[i]):
					self.board[i][j] = matrix[i][j]
				
				if (util.isStartingPoint(self.board[i][j])):
					self.begin = (i, j)
					self.starts.add((i, j))
				if (util.isEndingPoint(self.board[i][j])):
					self.ends.add((i, j))
				if (isinstance(self.board[i][j], Blob)):
					self.blobs.add((i, j))
				if (isinstance(self.board[i][j], Dot)):
					self.dots.add((i, j))
				if (isinstance(self.board[i][j], Star)):
					self.stars.add((i, j))
				if (isinstance(self.board[i][j], Triangle)):
					self.triangles.add((i, j))
				if (isinstance(self.board[i][j], BlockGroup)):
					self.blocks.add((i, j))
				if (isinstance(self.board[i][j], Cancel)):
					self.cancels.add((i, j))
	
	def defaultGrid(self):
		for i in range(self.n):
			for j in range(self.m):
				if (i % 2 == 0) or (j % 2 == 0):
					(self.board[i][j]).isPath = True
	
	def drawLine(self, a, b):
		if (a[0] != b[0] and a[1] != b[1]):
			return
		
		for i in range(min(a[0], b[0]), max(a[0], b[0]) + 1):
			for j in range(min(a[1], b[1]), max(a[1], b[1]) + 1):
				(self.board[i][j]).isPathOccupied = True
	
	def drawPath(self, path):
		for i in range(1, len(path)):
			self.drawLine(path[i - 1], path[i])
	
	def __str__(self):
		res = ""
		for i in range(self.n):
			if (i > 0):
				res = res + "\n"
			for j in range(self.m):
				if (j > 0):
					res = res + " "
				open = "_"
				close = "_"
				if (self.board[i][j]).isPath:
					if (self.board[i][j]).isPathOccupied:
						open = "["
						close = "]"
					else:
						open = close = "+"
				res = res + open + util.getType(self.board[i][j]) + close
				
		return res
	
	# Verification Algorithm
	
	def inside(self, p):
		if (p[0] < 0) or (p[1] < 0):
			return False
		if (p[0] >= len(self.board)):
			return False
		if (p[1] >= len(self.board[p[0]])):
			return False
		return True
	
	def get(self, p):
		if (not self.inside(p)):
			return Object()
		return self.board[p[0]][p[1]]
	
	def ver(self, src):
		dx = [1, 0, -1, 0]
		dy = [0, 1, 0, -1]
		
		'''
		// The algorithm works in four stages: THE FOX / THE WOLF / THE DRUDE / THE PHOENIX
		
		// The first section denoted THE FOX begins by handling the more trivial matters.
		// Just as foxes are easily recognizable (and a common Fursona species), the tasks for this section are relatively self-explanatory and easy to check.
		// Namely, whether the path goes from a start to an end, whether all dots are covered, and whether triangles are solved.
		// Violations do not automatically return False. Instead, the locations of violated symbols are put into a set.
		// However, this is arguably the most important section because it establishes the trajectory of the path.
		'''
		
		if (not util.isStartingPoint(self.get(src))):
			return False
		
		o = self.get(src)
		if (not o.isPathOccupied):
			return False
		
		vis = set()
		q = Queue()
		q.put(src)
		
		reachedend = False
		
		while (q.qsize() > 0):
			p = q.get()
			vis.add(p)
			
			for i in range(4):
				nextone = (p[0] + dx[i], p[1] + dy[i])
				if (not self.inside(nextone)):
					continue
				n = self.get(nextone)
				if (util.isEndingPoint(n)):
					reachedend = True
				if ((not n.isPath) or (not n.isPathOccupied)):
					continue
				if (nextone in vis):
					continue
				vis.add(nextone)
				q.put(nextone)
		
		if not reachedend:
			return False
		
		self.violations = set()
		
		for i in self.dots:
			o = self.get(i)
			if not o.isPathOccupied:
				self.violations.add(i)
		
		for i in self.triangles:
			o = self.get(i)
			if not isinstance(o, Triangle):
				continue
			target = o.x
			count = 0
			for ii in range(4):
				xp = i[0] + dx[ii]
				yp = i[1] + dy[ii]
				if (not self.inside((xp, yp))):
					continue
				o2 = self.get((xp, yp))
				if o2.isPath and o2.isPathOccupied:
					count = count + 1
			
			# print(count, target)
			
			if count != target:
				self.violations.add(i)
		
		'''
		// In the same way that wolves are largely territorial animals...
		// The second section denoted THE WOLF ensures that colors and territories are properly partitioned.
		// Namely, the blobs and the stars. Blobs cannot exist with any other color blob.
		// And stars, while they can exist with other colors, must exist with exactly one other of their color.
		// Although dots can be set as color, they do not affect the results.
		// When we traverse this time, the dx/dy effect is doubled.
		// However at each step we must also check the in between to make sure we do not hit the path.
		// The SET only contains locations of non-path coordinates.
		// Violations are measured as the lesser violation. Thus if a region contains 2 blue and 3 white dots
		// Then the 2 blue dots are marked as violation. Cancellation symbols will also ``seek'' the blue dots.
		'''
		
		ding = dict() # list of colors
		selectedcolors = dict()
		collected = set()
		
		vis.clear()
		
		for ii in self.blobs:
			if ii in vis:
				continue
			ding.clear()
			selectedcolors.clear()
			collected.clear()
			q = Queue()
			q.put(ii)
			
			while (q.qsize() > 0):
				now = q.get()
				vis.add(now)
				
				cur = self.get(now)
				if isinstance(cur, Blob):
					if not cur.color in ding:
						ding[cur.color] = []
					ding[cur.color].append(cur)
					
					collected.add(now)
					if not cur.color in selectedcolors:
						selectedcolors[cur.color] = 0
					selectedcolors[cur.color] = selectedcolors[cur.color] + 1
					
				for i in range(4):
					mid = (now[0] + dx[i], now[1] + dy[i])
					nextone = (mid[0] + dx[i], mid[1] + dy[i])
					
					if (not self.inside(mid)) or (not self.inside(nextone)):
						continue
					
					between = self.get(mid)
					hit = self.get(nextone)
					if (between.isPathOccupied):
						continue
					if nextone in vis:
						continue
					vis.add(nextone)
					q.put(nextone)
			
			maxcolor = Color.NIL
			maxfreq = -1
			for i in selectedcolors.keys():
				if not i in ding:
					continue
				
				truefreq = len(ding[i])
				if truefreq > maxfreq:
					maxfreq = truefreq
					maxcolor = i
		
			hasmorecolors = False
			for i in ding.keys():
				if (i != maxcolor) and (i != Color.NIL):
					hasmorecolors = True
		
			for i in collected:
				o = self.get(i)
				if (o.color != maxcolor) and (o.color != Color.NIL):
					self.violations.add(i)
				elif (hasmorecolors):
					self.violations.add(i)
		
		# The other task of THE WOLF is to handle stars. Thankfully, these are easier.
		
		vis.clear()
		
		for ii in self.stars:
			if ii in vis:
				continue
			ding.clear()
			selectedcolors.clear()
			collected.clear()
			q = Queue()
			q.put(ii)
			
			while (q.qsize() > 0):
				now = q.get()
				vis.add(now)
				
				cur = self.get(now)
				
				if not cur.color in ding:
					ding[cur.color] = []
				ding[cur.color].append(cur)
				
				if isinstance(cur, Star):
					collected.add(now)
					if not cur.color in selectedcolors:
						selectedcolors[cur.color] = 0
					selectedcolors[cur.color] = selectedcolors[cur.color] + 1
					
				for i in range(4):
					mid = (now[0] + dx[i], now[1] + dy[i])
					nextone = (mid[0] + dx[i], mid[1] + dy[i])
					
					if (not self.inside(mid)) or (not self.inside(nextone)):
						continue
					
					between = self.get(mid)
					hit = self.get(nextone)
					if (between.isPathOccupied):
						continue
					if nextone in vis:
						continue
					vis.add(nextone)
					q.put(nextone)
			
			for i in collected:
				o = self.get(i)
				if not o.color in ding:
					self.violations.add(i)
				if len(ding[o.color]) != 2:
					self.violations.add(i)
		
		'''
		// In German folklore, drudes are the malevolent nocturnal spirits associated with nightmares.
		// This section aka. THE DRUDE was an absolute nightmare to implement. I am, of course, referring to polynominos.
		// The algorithm is so weird that it has its own header file called blockgroup.h
		// Simply put, it is brute force. After all, this problem is NP-complete.
		// What happens here is simply a partition of the board and a check.
		'''
		
		vis.clear()
		region = set()
		
		for ii in self.blocks:
			if ii in vis:
				continue
			region.clear()
			collected.clear()
			q = Queue()
			q.put(ii)
			
			while (q.qsize() > 0):
				now = q.get()
				vis.add(now)
				region.add(now)
				
				cur = self.get(now)
				
				if isinstance(cur, BlockGroup):
					collected.add(now)
					
				for i in range(4):
					mid = (now[0] + dx[i], now[1] + dy[i])
					nextone = (mid[0] + dx[i], mid[1] + dy[i])
					
					if (not self.inside(mid)) or (not self.inside(nextone)):
						continue
					
					between = self.get(mid)
					hit = self.get(nextone)
					if (between.isPathOccupied):
						continue
					if nextone in vis:
						continue
					vis.add(nextone)
					q.put(nextone)
			
			regionvec = []
			for p in region:
				regionvec.append((p[1] // 2, -1 * (p[0] // 2)))
			testregion = BlockGroup(True, False, regionvec)
			testregion.normalize()
			pieces = []
			
			for i in collected:
				pieces.append(self.get(i))
			
			'''
			print("\n", testregion.pairs)
			for i in pieces:
				print(i.pairs)
			'''
		
			if testregion.solve(pieces):
				pass
			else:
				for i in collected:
					self.violations.add(i)
		
		'''
		// THE PHOENIX is an immortal creature, one that rises from the ashes once its life is over.
		// Similarly,we must start again from scratch when we encounter a cancellation symbol.
		// The way we do this is by removing it and another symbol then rechecking our current path against the new grid.
		// If there are no cancels the method simply returns what we have now.
		'''
		
		# print("VIOLATIONS", violations)
		
		if (len(self.cancels) == 0 or len(self.cancels) == len(self.ignored)):
			return len(self.violations) == 0
			
		if len(self.violations) == 0:
			return False # There are cancels!!!
		
		vis.clear()
		
		for ii in self.cancels:
			if ii in self.ignored:
				continue
			collected.clear()
			q = Queue()
			q.put(ii)
			
			while (q.qsize() > 0):
				now = q.get()
				vis.add(now)
				region.add(now)
				
				cur = self.get(now)
				
				if util.isSymbol(cur) and not isinstance(cur, Cancel):
					if now in self.violations:
						collected.add(now)
					
				for i in range(4):
					mid = (now[0] + dx[i], now[1] + dy[i])
					nextone = (mid[0] + dx[i], mid[1] + dy[i])
					
					if (not self.inside(mid)) or (not self.inside(nextone)):
						continue
					
					between = self.get(mid)
					hit = self.get(nextone)
					if (between.isPathOccupied):
						continue
					if nextone in vis:
						continue
					vis.add(nextone)
					q.put(nextone)
					q.put(mid)
					vis.add(mid)
			
			self.ignored.add(ii)
			self.board[ii[0]][ii[1]].ignored = True
			
			retval = False
			for i in collected:
				o = self.get(i)
				self.board[i[0]][i[1]] = o.clear()
				if isinstance(o, Dot):
					self.dots.discard(i)
				if isinstance(o, Blob):
					self.blobs.discard(i)
				if isinstance(o, Star):
					self.stars.discard(i)
				if isinstance(o, Triangle):
					self.triangles.discard(i)
				if isinstance(o, BlockGroup):
					self.blocks.discard(i)
				
				self.violations.discard(i)
				
				if self.ver(src):
					retval = True
				
				self.board[i[0]][i[1]] = o
				if isinstance(o, Dot):
					self.dots.add(i)
				if isinstance(o, Blob):
					self.blobs.add(i)
				if isinstance(o, Star):
					self.stars.add(i)
				if isinstance(o, Triangle):
					self.triangles.add(i)
				if isinstance(o, BlockGroup):
					self.blocks.add(i)
				self.violations.add(i)
				if (retval):
					break
			
			if (retval):
				return True

			self.ignored.discard(ii)
			self.board[ii[0]][ii[1]].ignored = False
		
		return False
	
	def validateRegion(self, src, ban):
		banned = set(ban)
		dx = [1, 0, -1, 0]
		dy = [0, 1, 0, -1]

		blobs = set()
		triangles = set()
		dots = set()
		cancels = set()
		blocks = set()

		vis = set()
		q = Queue()
		q.put(src)

		while q.qsize() > 0:
			now = q.get()
			vis.add(now)

			o = self.get(now)

			if isinstance(o, Blob):
				blobs.add(now)
			if isinstance(o, Dot):
				dots.add(now)
			if isinstance(o, Triangle):
				triangles.add(now)
			if isinstance(o, BlockGroup):
				blocks.add(now)
			if isinstance(o, Cancel):
				cancels.add(now)
			
			for i in range(4):
				next = (now[0] + dx[i], now[1] + dy[i])
				if not self.inside(next):
					continue
				if next in banned or next in vis:
					continue
				hit = self.get(next)
				if hit.isPathOccupied:
					continue
				vis.add(next)
				q.put(next)
		
		colors = set()
		for i in blobs:
			colors.add(self.get(i).color)
		if (len(colors) > 1):
			return False
		
		for i in dots:
			if (not self.get(i).isPathOccupied) and (i in banned):
				return False
		
		for i in triangles:
			o = self.get(i)
			if not isinstance(o, Triangle):
				continue
			target = o.x

			cnt = 0
			for d in range(4):
				sus = (i[0] + dx[i], i[1] + dy[i])
				if not self.inside(sus):
					continue
				if (self.get(sus).isPathOccupied) or (sus in banned):
					cnt = cnt + 1
			
			if cnt != target:
				return False
			
		if len(blocks) <= 0:
			return True
		
		effectiveRegion = []
		for i in vis:
			if (i[1] % 2 == 1) and (i[0] % 2 == 1):
				effectiveRegion.append((i[1] // 2, -1 * (i[0] // 2)))
		
		boop = []
		for i in blocks:
			o = self.get(i)
			if not isinstance(o, BlockGroup):
				continue
			boop.append(o)
		
		bg = BlockGroup(1, 0, effectiveRegion)
		bg.normalize()
		
		if not bg.solve(boop):
			return False
		
		return True


				





		





