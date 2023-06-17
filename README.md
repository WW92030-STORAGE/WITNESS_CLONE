# WITNESS_CLONE
- Clone of The Witness by Thekla Inc. as a portable interface.
- This repo attempts to add as much of the puzzle mechanics as possible.
  <br/>
- This is currently a work in progress.

# FEATURES
- Polynomino region checker (supports fixed and arbitrary rotations but not subtractive regions).
- Hexagon dots + colored blobs + triangles + stars + cancellations
- Verification of a given path. No custom inputs yet sorry...
- Grid solver - Automatically solves puzzle grids. Be warned that the current algorithm takes a long time to solve boards (grid cells) larger than 4x4.

# DEFINIITIONS【SPOILERS】

- The grid is composed of Lines and enclosed Cells.
- The solution line/curve is denoted as the Path. The Path will partition the grid into one or more Regions.
- A Region is a set of grid Cells connected along the cardinal directions such that no two adjacent Cells have the Path as a border.
- The Path must begin at a Starting Point (rendered as a circle) and end at an End Point (rendered as a rounded protrusion from the grid).

# THE RULES【SPOILERS】

- 【HEXAGON DOTS】- The Path must intersect every one.
- 【COLORED BLOBS】- The Region containing the blob cannot have anything of a different color.
- 【COLORED STARS】- The Region containing the star must have exactly one other object of the star's color. Other colors are disregarded.
- 【POLYNOMINOS】- The Region's shape (i.e. the shape of the Cells) must be composed exactly of the Polynominos contained within. No overlaps or excess Cells.
- 【TRIANGLES】- The number of triangles denotes the number of sides of the Cell the symbol lies in that must be traversed by the Path.
- 【CANCELLATION】- A Region containing N cancellations must contain exactly N symbols that would render the solution invalid otherwise. Cancellations cannot nullify each other. Dots are considered symbols if they lie inside the same Region as a cancellation.

# INTERNAL REPRESENTATION

- The grid is internally represented as a (2n + 1) ⋅ (2m + 1) array, where the grid cells are n rows and m columns.
- All points with both coordinates odd cannot have paths and must be empty or contain symbols instead.

# TO DO

- Implement (or find) a more efficient algorithm for Polynomino region checker.
- Implement subtractive blocks.
- Create a random puzzle generator (most likely based on a certain Challenge).
