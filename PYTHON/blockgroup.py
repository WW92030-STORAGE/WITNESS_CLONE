import sys

'''
In German folklore, a drude (German: Drude, pl. Druden[1]) is a kind of malevolent nocturnal spirit 
(an elf (Alp) or kobold or a hag) associated with nightmares, prevalent especially in Southern 
Germany. Druden were said to participate in the Wild Hunt and were considered a particular 
class of demon in Alfonso de Spina's hierarchy[citation needed]. The word also 
came to be used as a generic term for "witch" in the 16th century (Hans Sachs). -Wikipedia
'''

# BlockGroup class to represent drudes (+/- polyominos) and their methods.

class BlockGroup:
	oriented = True # is this oriented
	sub = False # is this subtractive
	pairs = {} # set of blocks
	n = 0 # number of blocks

	bottomleft = [0, 0]
	topright = [0, 0]
	boundingbox = (0, 0)

	BOUND = sys.maxsize
	NIL = (BOUND, BOUND)

	def __init__(self, orientation, subtractive, v): # oriented, sub, pairs
		self.oriented = orientation
		self.sub = subtractive
		self.pairs = set(v) # directly create the set of pairs from the list of pairs (tuples)
		self.n = len(self.pairs)

		self.bottomleft = [BlockGroup.BOUND, BlockGroup.BOUND]
		self.topright = [-1 * BlockGroup.BOUND, -1 * BlockGroup.BOUND]

		for pair in self.pairs:
			self.bottomleft[0] = min(self.bottomleft[0], pair[0])
			self.bottomleft[1] = min(self.bottomleft[1], pair[1])
			self.topright[0] = max(self.topright[0], pair[0])
			self.topright[1] = max(self.topright[1], pair[1])

		self.boundingbox = (self.topright[0] - self.bottomleft[0] + 1, self.topright[1] - self.bottomleft[1] + 1)
	
	def updateBounds(self):
		self.topright = [-1 * BlockGroup.BOUND, -1 * BlockGroup.BOUND]
		self.bottomleft = [BlockGroup.BOUND, BlockGroup.BOUND]

		for pair in self.pairs:
			self.bottomleft[0] = min(self.bottomleft[0], pair[0])
			self.bottomleft[1] = min(self.bottomleft[1], pair[1])
			self.topright[0] = max(self.topright[0], pair[0])
			self.topright[1] = max(self.topright[1], pair[1])

		self.boundingbox = (self.topright[0] - self.bottomleft[0] + 1, self.topright[1] - self.bottomleft[1] + 1)
	 
	# Utility Functions
	
	def contains(self, pair):
	    return pair in self.pairs
	
	def add(self, pair):
	    if self.contains(pair):
	        return
	    self.n = self.n + 1
	    self.pairs.add(pair)
	
	def remove(self, pair):
	    self.pairs.discard(pair)
	    self.n = self.n - 1
	    
	def reset(self, v):
		self.pairs = set(v) # directly create the set of pairs from the list of pairs (tuples)
		self.n = len(self.pairs)

		self.bottomleft = [BlockGroup.BOUND, BlockGroup.BOUND]
		self.topright = [-1 * BlockGroup.BOUND, -1 * BlockGroup.BOUND]

		for pair in self.pairs:
			self.bottomleft[0] = min(self.bottomleft[0], pair[0])
			self.bottomleft[1] = min(self.bottomleft[1], pair[1])
			self.topright[0] = max(self.topright[0], pair[0])
			self.topright[1] = max(self.topright[1], pair[1])

		self.boundingbox = (self.topright[0] - self.bottomleft[0] + 1, self.topright[1] - self.bottomleft[1] + 1)
	
	def clone(self):
	    newpoints = []
	    for pair in self.pairs:
	        newpoints.append(pair)
	    
	    return BlockGroup(self.oriented, self.sub, newpoints)
	
	# General Functions
	
	def rotate(self, x):
	    while x < 0:
	        x = x + 1000
	    x = x % 4
	    if x <= 0:
	        return
	    
	    self.rotate(x - 1)
	    
	    res = []
	    for pair in self.pairs:
	        res.append((-1 * pair[1], pair[0]))
	    
	    self.reset(res)
	
	def move(self, disp):
	    res = []
	    for pair in self.pairs:
	        res.append((pair[0] + disp[0], pair[1] + disp[1]))
	    
	    self.reset(res)
	
	def invmov(self, disp):
	    res = []
	    for pair in self.pairs:
	        res.append((pair[0] - disp[0], pair[1] - disp[1]))
	    
	    self.reset(res)
	
	def normalize(self):
	    self.invmov(self.bottomleft)
	
	def removeRegion(self, other):
	    for o in other.pairs:
	        self.remove(o)
	
	def addRegion(self, other):
	    for o in other.pairs:
	        self.add(o)
	
	# Region Testing
	
	def containsbb(self, b):
	    if self.boundingbox[0] < b.boundingbox[0]:
	        return False
	    if self.boundingbox[1] > b.boundingbox[1]:
	        return False
	    return True
	
	def directoverlay(self, b):
	    if self.n < b.n:
	        return False
	    for p in b.pairs:
	        if not self.contains(p):
	            return False
	    
	    return True
	
	def fixedoverlay(self, b):
	    res = []
	    if self.n < b.n:
	        return res
	    
	    dx = self.bottomleft[0] - b.bottomleft[0]
	    dy = self.bottomleft[1] - b.bottomleft[1]
	    
	    test = b.clone()
	    test.move((dx, dy))
	    
	    width = abs(self.boundingbox[0] - b.boundingbox[0])
	    height = abs(self.boundingbox[1] - b.boundingbox[1])
	    
	    for i in range(width + 1):
	        for j in range(height + 1):
	            if self.directoverlay(test):
	                res.append((i, j))
	            test.move((0, 1))
	        test.move((1, -1 * (height + 1)))
	    
	    return res
	
	def overlay(self, b):
	    res = [[], [], [], []]
	    res[0] = self.fixedoverlay(b)
	    
	    if not b.oriented:
	        test = b.clone()
	        test.normalize()
	        for i in range(3):
	            test.rotate(1)
	            test.normalize()
	            res[i + 1] = self.fixedoverlay(test)
	    
	    return res
	
	def dfsUtil(self, region, v, index):
	    if (index >= len(v)):
	        return region.n == 0
	    
	    group = v[index].clone()
	    group.normalize()
	    options = region.overlay(group)
	    
	    res = False
	    for i in range(4):
	        group.move(region.bottomleft)
	        for op in options[i]:
	            group.move(op)
	            region.removeRegion(group)
	            
	            res = self.dfsUtil(region, v, index + 1) or res
	            if (res):
	                return True
	                
	            group.addRegion(group)
	            group.invmov(op)
	        
	        group.rotate(1)
	        group.normalize()
	    
	    return res
	
	def solve(self, v):
	    return self.dfsUtil(self.clone(), v, 0)
