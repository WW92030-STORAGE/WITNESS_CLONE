from object import Object, Color
from miscsymbols import Endpoint, Dot, Blob, Star, Triangle, Cancel
from blockgroup import BlockGroup
from grid import Grid
import util

import sys
import math
from PIL import Image, ImageDraw


# Diameter always means the distance across in the cardinal directions.
# If there is an angle then the diameter is rotated around the same angle

def generateHexagon(pos, diameter):
	list = []
	for i in range(6):
		list.append((pos[0] + diameter * 0.5 * math.cos(math.pi * i / 3), pos[1] + diameter * 0.5 * math.sin(math.pi * i / 3)))
	
	return list

def generateTriangle(pos, diameter):
	list = []
	for i in range(3):
		list.append((pos[0] + diameter * 0.5 * math.cos(math.pi * (2 * i / 3 + 1.5)), pos[1] + diameter * 0.5 * math.sin(math.pi * (2 * i / 3 + 1.5))))
	
	return list

def generateBlob(pos, diameter):
	list = []
	dx = [1, -1, -1, 1]
	dy = [1, 1, -1, -1]
	for i in range(8):
		xp = (pos[0] + diameter * 0.5 * dx[i % 4])
		yp = (pos[1] + diameter * 0.5 * dy[i % 4])
		list.append((xp, yp))
	return list

def generateStar(pos, diameter):
	list = [[], []]
	dx = [1, 0, -1, 0]
	dy = [0, 1, 0, -1]
	for i in range(4):
		xp = (pos[0] + diameter * 0.5 * dx[i % 4])
		yp = (pos[1] + diameter * 0.5 * dy[i % 4])
		list[0].append((xp, yp))

	dx = [1, -1, -1, 1]
	dy = [1, 1, -1, -1]
	for i in range(4):
		xp = (pos[0] + diameter * math.sqrt(0.125) * dx[i % 4])
		yp = (pos[1] + diameter * math.sqrt(0.125) * dy[i % 4])
		list[1].append((xp, yp))

	return list

def generateSquare(pos, diameter, angle):
	list = []
	for i in range(4):
		theta = math.pi * (0.25 + (i / 2)) + angle
		xp = pos[0] + (diameter * 0.5) * math.cos(theta)
		yp = pos[1] + (diameter * 0.5) * math.sin(theta)
		list.append((xp, yp))
	return list

def generateDrude(pos, element, block_spacing, angle):
	element.normalize()
	list = []
	if not isinstance(element, BlockGroup):
		return list
	
	bb = element.boundingbox
	s = math.sin(angle)
	c = math.cos(angle)
	
	for block in element.pairs:
		xp = (block[0] - (bb[0] - 1) / 2) * block_spacing
		yp = (block[1] - (bb[1] - 1) / 2) * block_spacing

		xpos = pos[0] + xp * c - yp * s
		ypos = pos[1] - xp * s - yp * c
		list.append(generateSquare((xpos, ypos), block_spacing * 0.9, -1 * angle))
	
	return list

# Right now bg, line, path are solid colors. Filter is a solid color that dictates the light filter.

def colorize(grid, pos):
	element = grid.get(pos)
	
	# print(element, " / ", grid.violations)
	if (element in grid.violations or pos in grid.violations):
		return Color.RED.value if (element.color != Color.RED) else (127, 0, 0)

	if (element.color == Color.NIL):
		return 0
	
	return element.color.value

def render(output, puzzle, width = 1024, height = 1024, margin = 96, thickness = 48, bg = 0x808080, line = 0x404040, path = 0xFFFFFF, filter = 0xFFFFFF):
	image = Image.new("RGB", (width, height), bg)
	
	# Number of pixels between grid ranks and files (including symbols and paths)

	grid_height = (width - 2 * margin) / (len(puzzle.board) - 1)
	grid_width = (height - 2 * margin) / (len(puzzle.board[0]) - 1)

	grid_spacing = min(grid_width, grid_height)
	margin_x = (width - grid_spacing * (len(puzzle.board[0]) - 1)) / 2
	margin_y = (height - grid_spacing * (len(puzzle.board) - 1)) / 2

	# Constants

	length = margin / 2 # Length of endpoint protrusions
	rad = thickness / 2

	draw = ImageDraw.Draw(image)

	# Draw the lines

	for i in range(0, len(puzzle.board), 2):
		a = (margin_x, margin_y + grid_spacing * i)
		b = (width - margin_x, margin_y + grid_spacing * i)
		draw.line([a, b], width=thickness, fill=line, joint="curve")
	
	for i in range(0, len(puzzle.board[0]), 2):
		a = (margin_x + grid_spacing * i, margin_y)
		b = (margin_x + grid_spacing * i, height - margin_y)
		draw.line([a, b], width=thickness, fill=line, joint="curve")

	# Draw the ending points

	for i in range(len(puzzle.board)):
		for j in range(len(puzzle.board[i])):
			xpos = margin_x + grid_spacing * j
			ypos = margin_y + grid_spacing * i
			element = puzzle.get((i, j))

			if util.isEndingPoint(element):
				if (i == 0 and j == 0):
					draw.line((xpos - length, ypos - length, xpos, ypos), width=thickness, fill=line, joint="curve")
					draw.ellipse((xpos - length - rad, ypos - length - rad, xpos - length + rad, ypos - length + rad), fill=line)
				elif (i == 0 and j == len(puzzle.board[i]) - 1):
					draw.line((xpos + length, ypos - length, xpos, ypos), width=thickness, fill=line, joint="curve")
					draw.ellipse((xpos + length - rad, ypos - length - rad, xpos + length + rad, ypos - length + rad), fill=line)
				elif (i == len(puzzle.board) - 1 and j == len(puzzle.board[i]) - 1):
					draw.line((xpos + length, ypos + length, xpos, ypos), width=thickness, fill=line, joint="curve")
					draw.ellipse((xpos + length - rad, ypos + length - rad, xpos + length + rad, ypos + length + rad), fill=line)
				elif (i == len(puzzle.board) - 1 and j == 0):
					draw.line((xpos - length, ypos + length, xpos, ypos), width=thickness, fill=line, joint="curve")
					draw.ellipse((xpos - length - rad, ypos + length - rad, xpos - length + rad, ypos + length + rad), fill=line)

	# Draw the rounded corners

	for i in range(0, len(puzzle.board), 2):
		for j in range(0, len(puzzle.board[i]), 2):
			xpos = margin_x + grid_spacing * j
			ypos = margin_y + grid_spacing * i
			draw.ellipse((xpos - rad, ypos - rad, xpos + rad, ypos + rad), fill=line)
			if (util.isStartingPoint(puzzle.get((i, j)))):
				draw.ellipse((xpos - thickness, ypos - thickness, xpos + thickness, ypos + thickness), fill=line)
	
	for i in range(0, len(puzzle.board)):
		for j in range(0, len(puzzle.board[i])):
			xpos = margin_x + grid_spacing * j
			ypos = margin_y + grid_spacing * i

			element = puzzle.get((i, j))

			cut_radius = grid_spacing - thickness * 1.5
			# Draw cuts in the path
			if (i % 2 == 0 or j % 2 == 0) and not element.isPath:
				# print((i, j))
				draw.rectangle((xpos - cut_radius, ypos - cut_radius, xpos + cut_radius, ypos + cut_radius), fill=bg)

			# Draw dots on the path
			if (isinstance(element, Dot)):
				hex = generateHexagon((xpos, ypos), thickness * 0.9)
				draw.polygon(hex, fill = colorize(puzzle, (i, j)))
			
			# Draw blobs
			if (isinstance(element, Blob)):
				shape = generateBlob((xpos, ypos), grid_spacing / 4)
				# print(shape)
				draw.line(shape, width = int(grid_spacing) // 2, fill = colorize(puzzle, (i, j)), joint="curve")
			
			# Draw stars
			if (isinstance(element, Star)):
				shape = generateStar((xpos, ypos), grid_spacing)
				# print(shape)
				draw.polygon(shape[0], fill = colorize(puzzle, (i, j)))
				draw.polygon(shape[1], fill = colorize(puzzle, (i, j)))
			
			# Draw triangles

			if (isinstance(element, Triangle)):
				for ii in range(element.x):
					xpos2 = xpos + (ii + 0.5 - element.x / 2) * (grid_spacing / 2)
					shape = generateTriangle((xpos2, ypos), grid_spacing / 2)
					draw.polygon(shape, fill = colorize(puzzle, (i, j)))
			
			# Draw blocks

			if (isinstance(element, BlockGroup)):
				angle = 0 if element.oriented else -1 * math.pi / 6
				dimension = max(element.boundingbox[0], element.boundingbox[1])

				shape = generateDrude((xpos, ypos), element, grid_spacing / dimension, angle)
				for s in shape:
					draw.polygon(s, fill = colorize(puzzle, (i, j)))



			# Draw the path
			if (element.isPathOccupied):
				if (util.isStartingPoint(element)):
					draw.ellipse((xpos - thickness, ypos - thickness, xpos + thickness, ypos + thickness), fill=path)
				elif util.isEndingPoint(element):
					draw.ellipse((xpos - rad, ypos - rad, xpos + rad, ypos + rad), fill=path)
					if (i == 0 and j == 0):
						draw.line((xpos - length, ypos - length, xpos, ypos), width=thickness, fill=path, joint="curve")
						draw.ellipse((xpos - length - rad, ypos - length - rad, xpos - length + rad, ypos - length + rad), fill=path)
					elif (i == 0 and j == len(puzzle.board[i]) - 1):
						draw.line((xpos + length, ypos - length, xpos, ypos), width=thickness, fill=path, joint="curve")
						draw.ellipse((xpos + length - rad, ypos - length - rad, xpos + length + rad, ypos - length + rad), fill=path)
					elif (i == len(puzzle.board) - 1 and j == len(puzzle.board[i]) - 1):
						draw.line((xpos + length, ypos + length, xpos, ypos), width=thickness, fill=path, joint="curve")
						draw.ellipse((xpos + length - rad, ypos + length - rad, xpos + length + rad, ypos + length + rad), fill=path)
					elif (i == len(puzzle.board) - 1 and j == 0):
						draw.line((xpos - length, ypos + length, xpos, ypos), width=thickness, fill=path, joint="curve")
						draw.ellipse((xpos - length - rad, ypos + length - rad, xpos - length + rad, ypos + length + rad), fill=path)
				elif (i % 2 == 0 and j % 2 == 0):
					draw.ellipse((xpos - rad, ypos - rad, xpos + rad, ypos + rad), fill=path)
				elif (j % 2 == 1):
					draw.line((xpos - grid_spacing, ypos, xpos + grid_spacing, ypos), fill=path, width=thickness)
				elif (i % 2 == 1):
					draw.line((xpos, ypos - grid_spacing, xpos, ypos + grid_spacing), fill=path, width=thickness)

	image.save(output)
	