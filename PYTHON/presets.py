from object import Object, Color
from miscsymbols import Endpoint, Dot, Blob, Star, Triangle, Cancel
from blockgroup import BlockGroup
from grid import Grid
import util

# Greenhouse Elevator
def legosi():
	graywolf = []
	for i in range(9):
		gh_row = []
		for j in range(11):
			gh_row.append(Object())
		graywolf.append(gh_row)

	graywolf[8][0] = Endpoint(True)
	graywolf[0][10] = Endpoint(False)

	graywolf[1][1] = graywolf[3][1] = graywolf[5][1] = Blob(Color.CYAN)
	graywolf[1][3] = graywolf[1][5] = Blob(Color.WHITE)
	graywolf[3][5] = graywolf[5][5] = Blob(Color.MAGENTA)
	graywolf[7][7] = graywolf[7][9] = Blob(Color.WHITE)
	graywolf[1][9] = graywolf[3][9] = graywolf[5][9] = Blob(Color.YELLOW)

	legosi = Grid(graywolf)
	legosi.defaultGrid()
	return legosi

# Swampy Boots puzzle
def swampyboots():
	swampboots = []
	for i in range(9):
		row = []
		for j in range(9):
			row.append(Object())
		swampboots.append(row)

	boots = BlockGroup(True, False, [(0, 0), (0, 1), (3, 0), (3, 1)])
	lshape = BlockGroup(True, False, [(0, 0), (0, 1), (0, 2), (-1, 2), (-2, 2)])
	bar = BlockGroup(True, False, [(0, 0), (0, 1), (0, 2)])

	swampboots[8][0] = Endpoint(True)
	swampboots[0][8] = Endpoint(False)
	swampboots[5][3] = boots
	swampboots[1][1] = lshape
	swampboots[3][7] = bar

	swampyboots = Grid(swampboots)
	swampyboots.defaultGrid()
	
	return swampyboots

# The control panel
def prism():
	arr = []
	for i in range(7):
		row = []
		for j in range(11):
			row.append(Object())
		arr.append(row)
	
	bar = BlockGroup(False, False, [(0, 0), (1, 0), (2, 0)], Color.WHITE)
	bar.normalize()
	arr[6][0] = Endpoint(True)
	arr[0][10] = Endpoint(False)
	arr[1][5] = Blob(Color.GREEN)
	arr[5][1] = Blob(Color.BLUE)
	arr[5][9] = Blob(Color.RED)
	arr[5][5] = bar
	puz = Grid(arr)
	puz.defaultGrid()
	return puz


# The square puzzle in the RGB room
def azion():
	arr = []
	for i in range(13):
		row = []
		for j in range(13):
			row.append(Object())
		arr.append(row)
	
	arr[12][0] = Endpoint(True)
	arr[0][12] = Endpoint(False)
	arr[1][1] = Blob(Color.MAGENTA)
	arr[1][3] = arr[3][1] = arr[3][3] = Blob(Color.YELLOW)
	arr[1][9] = arr[1][11] = arr[3][9] = arr[3][11] = Blob(Color.WHITE)
	arr[9][1] = arr[11][1] = arr[11][3] = Blob(Color.BLACK)
	arr[9][3] = Blob(Color.RED)
	arr[9][9] = arr[11][9] = arr[11][11] = Blob(Color.GREEN)
	arr[9][11] = Blob(Color.CYAN)

	puz = Grid(arr)
	puz.defaultGrid()
	return puz

# The star puzzle in the RGB room
def nexie():
	arr = []
	for i in range(13):
		row = []
		for j in range(13):
			row.append(Object())
		arr.append(row)
	
	arr[12][0] = Endpoint(True)
	arr[0][12] = Endpoint(False)
	arr[1][1] = arr[1][3] = arr[3][3] = arr[9][11] = Star(Color.MAGENTA)
	arr[1][11] = arr[9][1] = arr[9][3] = arr[11][3] = Star(Color.BLACK)
	arr[1][9] = arr[3][9] = arr[3][11] = arr[11][1] = Star(Color.RED)
	arr[3][1] = arr[9][9] = arr[11][9] = arr[11][11] = Star(Color.WHITE)

	puz = Grid(arr)
	puz.defaultGrid()
	return puz