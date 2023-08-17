# PLEASE READ

- IF YOU OWN A PRIMAGEN OR RARE PROTOGEN (AS DEFINED BY ZOR) YOU ARE PROHIBITED FROM USING THIS CODE UNTIL FURTHER NOTICE.

# WITNESS_CLONE
- Clone of The Witness by Thekla Inc. as a portable interface.
- This repo attempts to add as much of the puzzle mechanics as possible.
  <br/>
- This is currently a work in progress.
- But one thing will remain certain: this is an interface. If you wish to attach this to some display you will have to figure that part out on your own.
- **THIS REPOSITORY CONTAINS SPOILERS FOR THE WITNESS.**

# FEATURES

- Polynomino region checker (supports fixed and arbitrary rotations but not subtractive regions).
- Hexagon dots + colored blobs + triangles + stars + cancellations
- Verification of a given path - As this is an interface there is no custom input system implemented.
- Grid solver - Automatically solves puzzle grids. Be warned that the current algorithm takes a long time to solve boards (grid cells) larger than 4x4.
- Random puzzle generator - Choose among certain presets.

# DEFINITIONS

- The grid is composed of Lines and enclosed Cells.
- The solution line/curve is denoted as the Path. The Path will partition the grid into one or more Regions.
- A Region is a set of grid Cells connected along the cardinal directions such that no two adjacent Cells have the Path as a border.
- The Path must begin at a Starting Point (usually denoted by a circle) and end at an End Point (usually denoted by a rounded protrusion from the grid).

# THE RULES

- 【HEXAGON DOTS】- The Path must intersect every one.
- 【COLORED BLOBS】- The Region containing the blob cannot have anything of a different color.
- 【COLORED STARS】- The Region containing the star must have exactly one other object of the star's color. Other colors are disregarded.
- 【POLYOMINOS】- The Region's shape (i.e. the shape of the Cells) must be composed exactly of the Polyominos contained within. No overlaps or excess Cells.
- 【TRIANGLES】- The number of triangles denotes the number of sides of the Cell the symbol lies in that must be traversed by the Path.
- 【CANCELLATION】- A Region containing N cancellations must contain exactly N symbols that would render the solution invalid otherwise. Cancellations cannot nullify each other. Dots are considered symbols if they lie inside the same Region as a cancellation.

# INTERNAL REPRESENTATION

- The grid is internally represented as a (2n + 1) ⋅ (2m + 1) array, where the grid cells are n rows and m columns.
- All points with both coordinates odd cannot have paths and must be empty or contain symbols instead.
- Polyominoes are checked by reducing the region and the constituents into the Exact Cover problem and then using Knuth's DLX algorithm.

# IMPORTANT

- YOU MUST CREDIT ME (WW92030-STORAGE/WW92030/NORMALEXISTING) WHENEVER YOU USE THIS CODE.
- IF YOU OWN A PRIMAGEN OR RARE PROTOGEN (AS DEFINED BY ZOR) YOU ARE PROHIBITED FROM USING THIS CODE UNTIL FURTHER NOTICE.

# TO DO【IN NO PARTICULAR ORDER】

- Implement subtractive blocks.
- Make the random puzzle generator more customizable.
---

![Example!](examples/triangles.jpg?raw=true)
