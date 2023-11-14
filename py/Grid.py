from Line import Line
class Grid:
    size = 25
    lines = []

    def __init__(self, inputLines: list[Line]):
        self.lines = inputLines
        self.state = ""
        self.updatedPixels = []
    
    def updatePixel(self, x, y, c):
        self.lines[x].updatePixel(y, c)
        self.lines[y + (0 if x >= Line.size else 25)].updatePixel(x % 25, c)
        self.updatedPixels.append((x, y, c))
    
    def getState(self):
        return self.state

    def setState(self, state: str):
        self.state = state

    def getUnknowPixels(self):
        returnList = []
        for i in range(Line.size):
            line = self.lines[i]
            for j in range(Line.size):
                if line.s[j] == "u":
                    returnList.append((i, j))
        return returnList

    def getLines(self):
        return self.lines

    def getSameValue(self, compareGrid, original):
        returnList = []
        for i in range(Line.size):
            la = self.lines[i]
            lb = compareGrid.lines[i]
            lo = original.lines[i]
            for j in range(Line.size):
                if la.s[j] == lb.s[j] and la.s[j] != lo.s[j]:
                    returnList.append((i, j, la.s[j]))
        return returnList

    def toString(self):
        s = ""
        for line in self.lines[Line.size::]:
            s += line.toString()
            s += "\n"
        return s
    
    def toStringCol(self):
        s = ""
        for line in self.lines[:Line.size:]:
            s += line.toString()
            s += "\n"
        return s