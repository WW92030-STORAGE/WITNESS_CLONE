from object import Object, Color
from miscsymbols import Endpoint, Dot, Blob, Star, Triangle, Cancel
from blockgroup import BlockGroup
from grid import Grid
import util
import presets
from randgrid import RandGrid

from solver import Solver
from render import render
from witnessgame import WitnessGame

'''

thegroup = BlockGroup(True, False, {(0, 0), (-1, 0), (0, -1)})

board = [[Endpoint(True), Dot(), Object(), Object(), Object()], [Object(), Star(Color.BLACK), Object(), Cancel(), Object()], [Object(), Object(), Object(), Object(), Object()], [Object(), Triangle(2), Object(), Star(Color.BLACK), Object()], [Object(), Object(), Object(), Object(), Endpoint(False)]]

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

render("examples/image.png", grid)

legosi = presets.legosi()

legosi.drawPath([(8, 0), (0, 0), (0, 2), (8, 2), (8, 4), (2, 4), (2, 6), (8, 6), (8, 10), (6, 10), (6, 8), (0, 8), (0, 10)])

legosi.ver((8, 0))
render("examples/legosi.png", legosi)

swampyboots = presets.swampyboots()

# swampyboots.drawPath([(8, 0), (8, 2), (6, 2), (6, 8), (2, 8), (2, 6), (0, 6), (0, 8)])
swampyboots.drawPath([(8, 0), (6, 0), (6, 2), (8, 2), (8, 4), (6, 4), (6, 8), (2, 8), (2, 6), (0, 6), (0, 8)])
swampyboots.ver((8, 0))

render("examples/swampboots.png", swampyboots, filter = (0, 255, 0))

dedication = presets.prism()
render("examples/prism0.png", dedication, filter = Color.RED.value)
render("examples/prism1.png", dedication, filter = Color.GREEN.value)
render("examples/prism2.png", dedication, filter = Color.BLUE.value)

allegiance = presets.azion()
render("examples/azion0.png", allegiance, filter = Color.RED.value)
render("examples/azion1.png", allegiance, filter = Color.GREEN.value)
render("examples/azion2.png", allegiance, filter = Color.BLUE.value)

renevant = presets.nexie()
render("examples/nexie0.png", renevant, filter = Color.RED.value)
render("examples/nexie1.png", renevant, filter = Color.GREEN.value)
render("examples/nexie2.png", renevant, filter = Color.BLUE.value)

rand = RandGrid()
rand.pathfind()

render("examples/random.png", rand.randBlocks(4, 2))

rand = RandGrid()

blockgrid = presets.swampyboots()
blockgrid = rand.randBlocks(4, 0)
solver = Solver(blockgrid)
sol = solver.solve()
for i in sol:
	blockgrid.board[i[0]][i[1]].isPathOccupied = True
render("examples/blocktest.png", blockgrid)

# sb = presets.swampyboots()
# solver.set(sb)
# sol = solver.solve()
# print(str(solver.callstopath), "CALLS TO PATH!")

solver.set(rand.randBlobs(15, 3, 0))
solver.solve()
print(str(solver.callstopath), "CALLS TO PATH!")

'''

random = RandGrid()
random.pathfind()
game = WitnessGame(random.randMaze(12))
render("examples/game.png", game.grid)

while True:
    char = input()
    if (char == "X"):
        game.clear()
        render("examples/game.png", game.grid)
        continue
    game.processInput(char)
    render("examples/game.png", game.grid)
    if (game.reachedEnd()):
        if (game.grid.ver(game.origin)):
            print("YOU WIN!!!!")
            break
        else:
            print("YOU LOSE!!!!")
            render("examples/game.png", game.grid)
            game.clear()
    