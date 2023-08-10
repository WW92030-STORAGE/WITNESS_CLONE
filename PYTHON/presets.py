from object import Object, Color
from miscsymbols import Endpoint, Dot, Blob, Star, Triangle, Cancel
from blockgroup import BlockGroup
from grid import Grid
import util

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