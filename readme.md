Bpoopeep
# Garden Path

Finds the best way to place a looped garden path in a space to maximise the number of accessible raised beds.

### The problem

Say you have an area to place a garden in and the area can be viewed as a grid. Say each tile is a 2x2 ft square and it can either be a path square or a bed square. A path square is one which you can step on. A bed square is a raised bed area.

Each raised bed square has an orientation. it's either north-south or east-west. You can access a north-south bed square from the north side or the south side but not from the east or west side. Similar rules for the east-west bed.

You want a path that starts at one point on the grid, moves around the garden space and ends up back at the beginning. You can place bed tiles to the left and right of path tiles as you move.

But a bed tile has to have path tiles on both sides to be valid. Also, you can't place a path tile next to another path tile (except the one you just moved from, obviously.)

With these rules, what's the best path to maximise the number of valid beds?

### Solution

This program starts at a given grid at (0,0). It evaluates the possible moves from the current position and tries each of them. If it hits a dead end, it backtracks. Along the way, it lays path tiles and adjacent bed tiles where possible; it won't place a bed tile where there is an existing path tile nor where there is a bed tile that's already connected to two path tiles. When it finds its way back to the origin, it elimates the invalid beds, counts the valid ones and shows the result if it's better than the previous result.

### Usage
./garden_path

## Examples

Finding a solution for a 16 x 16 grid.

```
    p p p p p p p p p p p
    p   | | | | | | |   p
    p — p p p p p p p — p
    p — p   | | |   p — p
    p — p — p p p — p — p
    p — p — p — p — p — p
    p — p — p — p — p — p
    p — p — p — p — p — p
    p — p p p — p — p — p
    p   | | |   p — p — p
    p p p p p p p — p — p
    | | | | | | |   p — p
    p p p p p p p p p — p
    p | | | | | | | |   p
    p p p p p p p p p p p
```

(2022-6-7: Haven't finished the code yet so the solution above is one I solved by hand. The program when finished will be able to generate these solutions...once I find my bugs.)
