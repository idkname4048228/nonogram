import copy
import time
from Line import Line
from Grid import Grid 
Line.size = 25
solve = False

grid = None

def fileInput():
    global grid, solve
    lines = []
    garbage = input()
    for i in range(Line.size * 2):
        newLine = Line(list(map(int, input().split())))
        lines.append(newLine)
    grid = Grid(lines)
    solve = False

def isSolve(G: Grid):
    tmp = True
    for line in G.lines:
        if not line.solved:
            tmp = False
    return tmp

def propagate(G: Grid):
    lines = G.lines
    G.updatedPixels = []
    lineOfG = [i for i in range(Line.size * 2)]
    
    while lineOfG:
        lineNumber = lineOfG.pop(0)
        line = lines[lineNumber]

        if not line.pureFix():
            G.setState("CONFLICT")
            return 

        painted = line.paint()
        diffs = line.diff(painted)
        line.painted()

        for number in diffs:
            changeLineNumber = number + (Line.size if lineNumber < Line.size else 0)
            changeLine = lines[changeLineNumber]
            updatingPixel = painted[number]
            updatingRow = lineNumber % Line.size

            G.updatedPixels.append((lineNumber, number, updatingPixel))
            # print(f'updating {changeLineNumber}, number {updatingRow}, pixel {updatingPixel}')

            changeLine.updatePixel(updatingRow, updatingPixel)
            
            if not changeLineNumber in lineOfG:
                lineOfG.append(changeLineNumber)
    
    if isSolve(G):
        G.setState("SOLVED")
    else:
        G.setState("INCOMPLETE")

def probeG(grid, x, y, c) -> Grid:
    newGrid = copy.deepcopy(grid)
    newGrid.updatePixel(x, y, c)

    propagate(newGrid)
    
    return newGrid

def probe(grid: Grid, x, y):
    g0 = probeG(grid, x, y, "0")
    g1 = probeG(grid, x, y, "1")

    if g0.getState() == "CONFLICT" and g1.getState() == "CONFLICT":
        grid.setState("CONFLICT")

    pixels = []
    if g0.getState() == "CONFLICT":
        pixels = g1.updatedPixels
    elif g1.getState() == "CONFLICT":
        pixels = g0.updatedPixels
    else:
        pixels = g0.getSameValue(g1, grid)
    
    if pixels:
        updateOnAllG(grid, pixels)
        grid.state = "PAINTED"
    else:
        grid.state = "INCOMPLETE"

def updateOnAllG(G: Grid, pixels = None):
    if not pixels:
        G.updatedPixels = []
        return
    for x, y, c in pixels:
        G.updatePixel(x, y, c)

def FP1(G: Grid):
    do = True
    while do or G.updatedPixels:
        do = False
        propagate(G)

        if G.getState() == "CONFLICT" or G.getState() == "SOLVED":
            return

        updateOnAllG(G)

        for i, j in G.getUnknowPixels():

            probe(G, i, j)
            if G.getState() == "CONFLICT" or G.getState() == "SOLVED":
                return
            if G.getState() == "PAINTED":
                break

def backtracking(G: Grid):
    global solve
    if solve:
        return 

    FP1(G)
    if G.getState() == "CONFLICT":
        return

    if G.getState() == "SOLVED":
        solve = True
        return

    x, y = G.getUnknowPixels()[0]
    G.updatePixel(x, y, "0")
    backtracking(G)
    G.updatePixel(x, y, "1")
    backtracking(G)


case = 0
while 1:
    try:
        case += 1
        start_time = time.time()
        fileInput()
        backtracking(grid)
        end_time = time.time()
        print(grid.toString())
        print(f'case {case} take {end_time - start_time}s')
    except EOFError:
        break

