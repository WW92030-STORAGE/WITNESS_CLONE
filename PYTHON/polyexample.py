from blockgroup import BlockGroup

points = [(0, 0), (0, 1), (0, 2), (1, 2)]
bg = BlockGroup(False, False, points)

# Unit tests

print(bg.pairs, bg.n, bg.bottomleft, bg.topright, bg.boundingbox)

for i in range(0):
    for j in range(4):
        pairx0 = (i, j)
        print(pairx0, bg.contains(pairx0))

bg2 = bg.clone()
print(bg2.pairs, bg2.n, bg2.bottomleft, bg2.topright, bg2.boundingbox)

bg2.rotate(1)
print(bg2.pairs, bg2.n, bg2.bottomleft, bg2.topright, bg2.boundingbox)
bg2.normalize()
print(bg2.pairs, bg2.n, bg2.bottomleft, bg2.topright, bg2.boundingbox)

bg2.addRegion(bg)
print(bg2.pairs, bg2.n, bg2.bottomleft, bg2.topright, bg2.boundingbox)

# Block Tests

craftingtable = [(0, 0), (0, 1), (0, 2), (1, 0), (1, 1), (1, 2), (-1, 0), (-1, 1), (-1, 2)]
tetromino = [(100, 100), (100, 101), (101, 100), (102, 100)]
corner = [(0, 0), (0, 1), (1, 0)]
bar = [(62, 62), (62, 63)]
cell = [(420, 69)]

threegrid = BlockGroup(True, False, craftingtable)
knight = BlockGroup(True, False, tetromino)
lshape = BlockGroup(False, False, corner)
domino = BlockGroup(False, False, bar)
unit = BlockGroup(True, False, cell)

test = [lshape, lshape, unit, unit, unit]
print(threegrid.solve(test))

test = [domino, domino]
print(knight.solve(test))
