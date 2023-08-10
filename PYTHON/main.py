from object import Object, Color
from miscsymbols import Endpoint, Dot, Blob, Star, Triangle, Cancel
from blockgroup import BlockGroup
from grid import Grid
import util

from render import render

thegroup = BlockGroup(False, False, {(0, 0), (-1, 0), (0, -1)})

board = [[Endpoint(True), Dot(), Object(), Object(), Object()], [Object(), Star(Color.BLACK), Object(), thegroup, Object()], [Object(), Object(), Object(), Object(), Object()], [Object(), Triangle(2), Object(), Star(Color.BLACK), Object()], [Object(), Object(), Object(), Object(), Endpoint(False)]]

for i in range(5):
	for j in range(5):
		if (i % 2 == 0 or j % 2 == 0):
			board[i][j].isPath = True

board[0][3].isPath = False

grid = Grid(board)

incorrect_sol = True

if (incorrect_sol):
	grid.drawLine((0, 0), (2, 0))
	grid.drawLine((2, 2), (2, 0))
	grid.drawLine((2, 2), (4, 2))
	grid.drawLine((4, 2), (4, 4))
else:
	grid.drawLine((0, 0), (0, 4))
	grid.drawLine((0, 4), (4, 4))

print(grid.ver((0, 0)))
print((grid))
print(grid.violations)

render("image.png", grid)

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

legosi.drawLine((0, 0), (8, 0))
legosi.drawLine((0, 0), (0, 2))
legosi.drawLine((0, 2), (8, 2))
legosi.drawLine((8, 2), (8, 4))
legosi.drawLine((8, 4), (2, 4))
legosi.drawLine((2, 4), (2, 6))
legosi.drawLine((2, 6), (8, 6))
legosi.drawLine((8, 6), (8, 10))
legosi.drawLine((8, 10), (6, 10))
legosi.drawLine((6, 10), (6, 8))
legosi.drawLine((6, 8), (0, 8))
legosi.drawLine((0, 8), (0, 10))

legosi.ver((8, 0))
render("legosi.png", legosi)
