# WITNESS_CLONE
Clone of The Witness by Thekla Inc. as a portable interface. <br/>
This repo attempts to add as much of the puzzle mechanics as possible. <br/>

This is currently a work in progress.

# FEATURES
Polynomino region checker (supports fixed and arbitrary rotations but not subtractive regions). <br/>
Hexagon dots + colored blobs + triangles + stars + cancellations.<br/>
Verification of a given path. No custom inputs yet sorry... <br/>

# DEFINIITIONS【SPOILERS】

The grid is composed of Lines and enclosed Cells. <br/>
The solution line/curve is denoted as the Path. The Path will partition the grid into one or more Regions. <br/>
A Region is a set of grid Cells connected along the cardinal directions such that no two adjacent Cells have the Path as a border.<br/>
The Path must begin at a Starting Point (rendered as a circle) and end at an End Point (rendered as a rounded protrusion from the grid).

# THE RULES【SPOILERS】

【HEXAGON DOTS】- The Path must intersect every one. <br/>
【COLORED BLOBS】- The Region containing the blob cannot have anything of a different color.<br/>
【COLORED STARS】- The Region containing the star must have exactly one other object of the star's color. Other colors are disregarded.<br/>
【POLYNOMINOS】- The Region's shape (i.e. the shape of the Cells) must be composed exactly of the Polynominos contained within. No overlaps or excess Cells.<br/>
【TRIANGLES】- The number of triangles denotes the number of sides of the Cell the symbol lies in that must be traversed by the Path.<br/>
【CANCELLATION】- A Region containing N cancellations must contain exactly N symbols that would render the solution invalid otherwise. Cancellations cannot nullify each other. Dots are considered symbols if they lie inside the same Region as a cancellation.

# TO DO

Implement (or find) a more efficient algorithm for Polynomino region checker.
